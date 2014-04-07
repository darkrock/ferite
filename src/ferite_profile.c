#ifdef FERITE_PROFILE
#ifdef HAVE_CONFIG_HEADER
#include "../config.h"
#endif
#include <stdio.h>
#include "ferite.h"
#include "aphex.h"
#include <libgen.h>
#include <errno.h>

AphexMutex *ferite_profile_mutex = NULL;
#define INIT_PROFILE_LOCK() if (ferite_profile_mutex == NULL) \
         ferite_profile_mutex = aphex_mutex_recursive_create()
#define LOCK_PROFILE() aphex_mutex_lock(ferite_profile_mutex)
#define UNLOCK_PROFILE() aphex_mutex_unlock(ferite_profile_mutex)

#define DIE(reason) do { fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, reason); exit(1); } while(0)
#define ONE_BILLION 1000000000L
#define ONE_MILLION 1000000L
#define TIMESPEC_TO_MILLISECONDS(ts) \
    (((long double)((ts).tv_sec) * ONE_BILLION + (ts).tv_nsec) / ONE_MILLION)
#define calculate_average_in_milliseconds(ts, freq) \
	(((long)((ts).tv_sec) * ONE_BILLION \
	 + (ts).tv_nsec) / (long double)(freq) / ONE_MILLION)

/* We don't expect many functions to be defined on the same line */
#define FE_FUNCTIONS_HASH_SIZE 3

#define FE_CALLER_HASH_SIZE 256
#define FERITE_PROFILE_NHASH 8192
#define FERITE_PROFILE_STACK_SIZE 5
static char *FILENAME_EVAL = "eval()";
int ferite_profile_enabled = FE_FALSE;
static char profile_output[PATH_MAX] = { 0 };
static FeriteProfileEntry *profile_entries[FERITE_PROFILE_NHASH] = { NULL };
static struct timespec profile_start_ts = { 0, 0 };

//#define FERITE_FAKE_CLOCK
#ifdef FERITE_FAKE_CLOCK
#  define CLOCK_GETTIME(p) fake_clock_gettime(p)
static int fake_clock_gettime(struct timespec *ts)
{
	static int fake_clock_counter = 0;
	ts->tv_sec = fake_clock_counter;
	ts->tv_nsec = 0;
	fake_clock_counter++;
	return 0;
}
#else
#  define CLOCK_TYPE CLOCK_THREAD_CPUTIME_ID
#  define CLOCK_GETTIME(p) clock_gettime(CLOCK_TYPE, p)
#endif

static size_t hash(const char *key, size_t limit)
{
	unsigned int hash = 0;
	while (*key) {
		hash = *key + 31 * hash;
		key++;
	}
	return hash % limit;
}

static int number_width(unsigned int num)
{
	unsigned int width = 0;

	do {
		width += 1;
		num /= 10;
	} while (num > 0);

	return width;
}

static int get_line_count(const char *filename, size_t *count)
{
	FILE *f = fopen(filename, "r");
	int ch;
	int got_ch = 0;
	int lines = 0;

	if (f == NULL) {
		perror(filename);
		return 1;
	}

	while (EOF != (ch=fgetc(f))) {
		got_ch = 1;
		if (ch=='\n')
			lines++;
	}
	if (lines == 0 && got_ch) {
		/* File with no line terminators */
		lines = 1;
	}
	*count = lines;

	if (fclose(f) == EOF) {
		perror(filename);
	}
	return 0;
}

static int file_exists(const char *filename)
{
	struct stat st;
	const int r = stat(filename, &st);
	if (r == 0)
		return 1;
	if (errno == ENOENT)
		return 0;

	fprintf(stderr, "Error stat'ing file %s\n", filename);
	perror(filename);
	return 0;
}

static int profile_line_entry_init_for_eval(FeriteProfileEntry * const pe)
{
	pe->is_file = 0;
	pe->lines = fcalloc_ngc(sizeof(FeriteProfileLineEntry),
				3 /* 1-based indexing + 1 line + 1 EOF line */
				);
	pe->line_count = 2; /* line 1 + EOF line */

	return 0;
}

static int profile_line_entry_init_for_file(FeriteProfileEntry * const pe)
{
	size_t line_count;

	if (get_line_count(pe->filename, &line_count) != 0) {
		fprintf(stderr, "Error getting line count for %s", pe->filename);
		return 1;
	}

	pe->is_file = 1;
	pe->lines = fcalloc_ngc(sizeof(FeriteProfileLineEntry),
			line_count
			+ 1 /* 1-based indexing, for unused lines[0] */
			+ 1 /* Count in EOF too */
			);
	pe->line_count = line_count + 1 /* EOF */;

	return 0;
}

static int profile_line_entry_init_for_unknown(FeriteProfileEntry * const pe)
{
	size_t line_count = 1;

	pe->is_file = 0;
	pe->lines = fcalloc_ngc(sizeof(FeriteProfileLineEntry),
			line_count
			+ 1 /* 1-based indexing, for unused lines[0] */
			+ 1 /* Count in EOF too */
			);
	pe->line_count = line_count + 1 /* EOF */;

	return 0;
}

static int profile_line_entry_init(FeriteProfileEntry *pe)
{
	if (file_exists(pe->filename)) {
		return profile_line_entry_init_for_file(pe);
	}

	if (pe->filename == FILENAME_EVAL) {
		return profile_line_entry_init_for_eval(pe);
	}

	return profile_line_entry_init_for_unknown(pe);
}

static FeriteProfileEntry *profile_init(const char *filename)
{
	FeriteProfileEntry *pe;

	pe = fmalloc_ngc(sizeof(FeriteProfileEntry));
	if (strncmp(filename, "eval()", 6) == 0) {
		pe->filename = FILENAME_EVAL;
	} else {
		pe->filename = ferite_strdup((char *)filename, __FILE__, __LINE__);
	}
	if (profile_line_entry_init(pe)) {
		return NULL;
	}

	pe->next = NULL;

	return pe;
}

static int is_profile_for(const char *filename, const FeriteProfileEntry *pe)
{
	return strcmp(pe->filename, filename) == 0;
}

static FeriteProfileEntry *find_in_linked_list(const char *filename, FeriteProfileEntry *pe)
{
	while (pe) {
		if (is_profile_for(filename, pe))
			return pe;
		pe = pe->next;
	}
	return NULL;
}

static FeriteProfileEntry *get_profile_entry_in_hash(const char *filename)
{
	unsigned int idx = hash(basename((char *)filename), FERITE_PROFILE_NHASH);

	return find_in_linked_list(filename, profile_entries[idx]);
}

static FeriteProfileEntry *get_or_create_profile_entry_in_hash(const char *filename)
{
	FeriteProfileEntry *p = NULL;
	unsigned int idx = hash(basename((char *)filename), FERITE_PROFILE_NHASH);
	FeriteProfileEntry *pe = profile_entries[idx];

	if (pe) {
		p = find_in_linked_list(filename, pe);
		if (p)
			return p;
	}

	p = profile_init(filename);
	p->next = profile_entries[idx];
	profile_entries[idx] = p;

	return p;
}

void save_profile(void) {
	ferite_profile_save(0);
}

void ferite_profile_toggle(const int state)
{
	static int atexit_registered = 0;
	INIT_PROFILE_LOCK();

#ifdef FERITE_FAKE_CLOCK
	fprintf(stderr, "Note: ferite profiling is using fake clock\n");
#endif

	LOCK_PROFILE();

	ferite_profile_enabled = state;
	if (ferite_profile_enabled) {
		if (profile_start_ts.tv_sec == 0 && profile_start_ts.tv_sec == 0) {
#ifdef FERITE_FAKE_CLOCK
			CLOCK_GETTIME(&profile_start_ts);
#else
			clock_gettime(CLOCK_REALTIME, &profile_start_ts);
#endif
		}
		if (! atexit_registered) {
			atexit_registered = 1;
			atexit(save_profile);
		}
	} else {
		profile_start_ts.tv_sec = 0;
		profile_start_ts.tv_nsec = 0;
	}

	UNLOCK_PROFILE();
}

static struct timespec timespec_diff(const struct timespec *old, struct timespec *new)
{
	struct timespec d;
	if (new->tv_nsec < old->tv_nsec) {
		new->tv_sec -= 1;
		new->tv_nsec += ONE_BILLION;
	}
	d.tv_nsec = new->tv_nsec - old->tv_nsec;
	d.tv_sec = new->tv_sec - old->tv_sec;
	return d;
}

static void timespec_add(struct timespec *t, const struct timespec delta)
{
	t->tv_nsec += delta.tv_nsec;
	if (t->tv_nsec >= ONE_BILLION) {
		t->tv_nsec %= ONE_BILLION;
		t->tv_sec += 1;
	}
	t->tv_sec += delta.tv_sec;
}

static void grow_pe_line_count(FeriteProfileEntry *pe, int line)
{
	size_t add = line - pe->line_count;

	pe->lines = frealloc_ngc(pe->lines, sizeof(FeriteProfileLineEntry) * (pe->line_count + add + 1));
	if (pe->lines == NULL) {
		fprintf(stderr, "OOM reallocating profile lines record\n");
		UNLOCK_PROFILE();
		exit(1);
	}
	memset(pe->lines + pe->line_count + 1, 0, sizeof(FeriteProfileLineEntry) * add);
	pe->line_count += add;
}

static FeriteProfileLineEntry *get_line_entry(FeriteProfileEntry *pe, const size_t line)
{
	FeriteProfileLineEntry *le;

	if (!pe->is_file && line > pe->line_count )
		grow_pe_line_count(pe, line);


	FE_ASSERT(line <= pe->line_count);

	le = &pe->lines[line];
	return le;
}

static char *get_container_fqn(void *container, FeriteFunction *function)
{
	if (strncmp(function->name, "!__start__", 10) == 0) {
		return "[main]";
	}

	if (strncmp(function->name, "!closure:", 9) == 0) {
		fprintf(stderr, "FIXME: we shouldn't reach here");
		exit(1);
	}

	if (function->klass != NULL) {
		if (strncmp(function->klass->name, "!closure:", 9) == 0) {
			return fstrdup(function->klass->name);
		}
		return ferite_generate_class_fqn(NULL, function->klass);
	}

	if (container != NULL) {
		FeriteNamespace *ns = (FeriteNamespace*) container;
		if (ns->name != NULL && strncmp(ns->name, "!closure:", 9) == 0) {
			fprintf(stderr, "FIXME: we shouldn't reach here");
			exit(1);
		}
		return ferite_generate_namespace_fqn(NULL, (FeriteNamespace*)container);
	}

	return "[container unknown]";
}

static void get_filename_and_start_line(FeriteScript *script, FeriteFunction *f, char **filename, int *start_line)
{
	if (f->native_information != NULL) {
		*filename = ferite_strdup(f->native_information->file, __FILE__, __LINE__);
		*start_line = f->line;
	} else if (f->bytecode != NULL) {
		*filename = ferite_strdup(f->bytecode->filename, __FILE__, __LINE__);
		*start_line = f->line;
	} else {
		// Both filename and bytecode are null - assume builtin obj methods
		if (f->klass != NULL) {
			int len = strlen(f->name);
			*filename = ferite_generate_class_fqn(NULL, f->klass);
			*filename = frealloc_ngc(*filename, len + 2 );
			strcat(*filename, ".");
			strcat(*filename, f->name);
		} else {
			FE_ASSERT(0 /* We shouldn't reach here */);
		}
		*start_line = 1;
	}

	if (strncmp(*filename, "eval()", 6) == 0 || strncmp(*filename, "entry:eval()", 12) == 0) {
		ffree_ngc(*filename)
		*filename = FILENAME_EVAL;
		*start_line = f->bytecode->list[0]->line;
	}
}

FeriteProfileFunction *ferite_profile_get_function_profile(FeriteScript *script, void *container, FeriteFunction *function)
{
	FeriteProfileEntry *pe;
	FeriteProfileLineEntry *le;
	FeriteProfileFunction *fpf = NULL;
	char *filename = NULL;
	int start_line = -1;

	LOCK_PROFILE();

	get_filename_and_start_line(script, function, &filename, &start_line);

	pe = get_or_create_profile_entry_in_hash(filename);
	FE_ASSERT(pe != NULL);

	le = get_line_entry(pe, start_line);

	if (le->functions == NULL) {
		le->functions = ferite_create_hash(NULL, FE_FUNCTIONS_HASH_SIZE);
	} else {
		fpf = ferite_hash_get(NULL, le->functions, function->name);
	}

	if (fpf == NULL) {
		fpf = fcalloc_ngc(sizeof(FeriteProfileFunction), 1);
		fpf->filename = (char *)filename;
		fpf->start_line = start_line;
		fpf->is_native = function->native_information != NULL;
		fpf->container = get_container_fqn(container, function);
		fpf->name = fstrdup(function->name);
		ferite_hash_add(NULL, le->functions, fpf->name, fpf);
	}

	UNLOCK_PROFILE();

	return fpf;
}

void ferite_profile_set_caller(FeriteScript *script, void *container, FeriteFunction *function)
{
	FE_ASSERT(script->caller == NULL);
	script->caller = ferite_profile_get_function_profile(script, container, function);
	script->caller->calling_line = script->current_op_line;
}

struct timespec ferite_profile_function_begin(FeriteScript *script, void *container, FeriteFunction *function, FeriteProfileFunction **fpf)
{
	struct timespec begin_ts;

	CLOCK_GETTIME(&begin_ts);

	LOCK_PROFILE();

	*fpf = ferite_profile_get_function_profile(script, container, function);
	(*fpf)->ncalls++;

	UNLOCK_PROFILE();

	return begin_ts;
}

struct timespec ferite_profile_function_end(FeriteScript *script, FeriteProfileFunction *fpf, int end_line, struct timespec *begin_ts)
{
	struct timespec end_ts, duration;

	CLOCK_GETTIME(&end_ts);

	LOCK_PROFILE();

	// TODO handle two functions defined on the same line
	if (fpf->end_line < end_line) {
	  fpf->end_line = end_line;
	}

	duration = timespec_diff(begin_ts, &end_ts);
	timespec_add(&fpf->inclusive_duration, duration);

	UNLOCK_PROFILE();

	return duration;
}

struct timespec ferite_profile_line_begin(FeriteScript *script, const char *filename, const size_t line)
{
	FeriteProfileEntry *pe;
	FeriteProfileLineEntry *le;
	struct timespec begin_ts = {0,0};

	CLOCK_GETTIME(&begin_ts);

	LOCK_PROFILE();

	pe = get_or_create_profile_entry_in_hash(filename);
	if (pe == NULL) {
		fprintf(stderr,
			"Error creating profile entry for file %s\n", filename);
		UNLOCK_PROFILE();
		return begin_ts;
	}

	le = get_line_entry(pe, line);
	if (le == NULL) {
		UNLOCK_PROFILE();
		return begin_ts;
	}

	le->ncalls++;

	UNLOCK_PROFILE();

	return begin_ts;
}

void ferite_profile_line_end(FeriteScript *script, const char *filename, const size_t line, struct timespec *begin_ts)
{
	FeriteProfileEntry *pe;
	FeriteProfileLineEntry *le;
	struct timespec end_ts, duration;

	// TODO either get rid of these getpid()s or use them at least once
	// to verify locking works as intended
	static pid_t pid = 0;
	if (pid == 0) {
		pid = getpid();
	}
	pid_t other_pid  = getpid();
	if (pid != other_pid) {
		fprintf(stderr, "pid %d, other pid = %d\n", pid, other_pid);
	}

	CLOCK_GETTIME(&end_ts);

	LOCK_PROFILE();

	pe = get_profile_entry_in_hash(filename);
	if (pe == NULL) {
		fprintf(stderr, "FIXME No hash for file %s ???\n", filename);
		UNLOCK_PROFILE();
		return;
	}
	if (pe->line_count < line) {
		if (pe->is_file) {
			fprintf(stderr,
				"Error: Line number %lu exceeds the one we "
				"counted initially (%lu) for file %s",
				line, pe->line_count, filename);
			UNLOCK_PROFILE();
			exit(1);
		} else {
			grow_pe_line_count(pe, line);
		}
	}

	le = &pe->lines[line];

	duration = timespec_diff(begin_ts, &end_ts);
	timespec_add(&le->total_duration, duration);

	UNLOCK_PROFILE();
}

static void get_caller_key(FeriteProfileFunction *caller, int caller_line, FeriteProfileFunction *callee, char *buf, size_t size)
{
	int nprinted;

	nprinted = snprintf(buf, size, "%s:%d:%s:%d,%s:%d:%s",
			    caller->filename, caller->start_line, caller->name,
			    caller_line,
			    callee->filename, callee->start_line, callee->name);
	if (nprinted >= size) {
		fprintf(stderr, "FIXME: ferite profile: get_caller_key(): failed "
				"assumption: bufsize %lu is to small\n", size);
		exit(1);
	}
}

void ferite_profile_add_caller(FeriteProfileFunction *callee, FeriteProfileFunction *caller, int caller_line, struct timespec duration_ts)
{
	FeriteProfileFunctionCaller *fpfc;
	char caller_key[PATH_MAX*3];

	LOCK_PROFILE();

	if (callee->callers == NULL) {
		callee->callers = ferite_create_hash(NULL, FE_CALLER_HASH_SIZE);
	}

	get_caller_key(caller, caller_line, callee, caller_key, PATH_MAX*2);
	fpfc = ferite_hash_get(NULL, callee->callers, caller_key);
	if (fpfc == NULL) {
		fpfc = fcalloc_ngc(sizeof(FeriteProfileFunctionCaller), 1);
		fpfc->caller = caller;
		fpfc->caller_line = caller_line;
		ferite_hash_add(NULL, callee->callers, caller_key, fpfc);
	}

	fpfc->frequency += 1;
	timespec_add(&fpfc->total_duration, duration_ts);
	timespec_add(&caller->exclusive_duration, duration_ts);

	UNLOCK_PROFILE();
}

static int append_pid(char *buf, pid_t pid)
{
	unsigned int len;
	unsigned int pid_width;
	char pid_str[10];

	pid_width = number_width(pid);
	if (pid_width > 8) {
		fprintf(stderr, "FIXME: pid too large? %d (%s:%d)\n",
				pid, __FILE__, __LINE__);
		return 0;
	}
	snprintf(pid_str, 10, ".%d", pid);

	len = strlen(buf) + pid_width + 1;
	if (len > PATH_MAX - 1) {
		fprintf(stderr,
			"Error: profile output '%s.{pid} exceeds PATH_MAX\n",
			profile_output);
		return 0;
	}
	strncat(buf, pid_str, 10);
	return 1;
}

static int format_profile_filename(char *format, char *buf, pid_t pid)
{
	struct tm now;
	time_t t;

	(void)time(&t);
	(void)localtime_r(&t, &now);

	if (strftime(buf, PATH_MAX, format, &now) == 0) {
		fprintf(stderr, "Error: profile output pattern '%s' results in "
				"empty filename\n", profile_output);
		return 0;
	}

	if (pid != 0 && append_pid(buf, pid) == 0)
		return 0;

	return 1;
}

static void json_comma(FILE *f)
{
	fprintf(f, ",");
}

static void json_key(FILE *f, char *key)
{
	fprintf(f, "\"%s\":", key);
}

static void json_array_open(FILE *f, char *key)
{
	if (key != NULL)
		json_key(f, key);
	fprintf(f, "[");
}

static void json_array_close(FILE *f)
{
	fprintf(f, "]");
}

static void json_dict_open(FILE *f, char *key)
{
	if (key != NULL)
		json_key(f, key);
	fprintf(f, "{");
}

static void json_dict_close(FILE *f)
{
	fprintf(f, "}");
}

static void json_string(FILE *f, char *key, char *value)
{
	fprintf(f, "\"%s\":\"%s\"", key, value);
}

static void json_positive_number(FILE *f, char *key, size_t value)
{
	fprintf(f, "\"%s\":%lu", key, value);
}

static void json_bool(FILE *f, char *key, int value)
{
	fprintf(f, "\"%s\":%s", key, value ? "true":"false");
}

static void json_null(FILE *f, char *key)
{
	if (key) {
		fprintf(f, "\"%s\":", key);
	}
	fprintf(f, "null");
}

static void json_timespec(FILE *f, char *key, struct timespec value)
{
	json_dict_open(f, key);
	json_positive_number(f, "sec", value.tv_sec);
	json_comma(f);
	json_positive_number(f, "nsec", value.tv_nsec);
	json_dict_close(f);
}

static void write_json_name_and_namespace(FILE *f, FeriteProfileFunction *function)
{
	json_string(f, "name", function->name);
	json_comma(f);
	if (function->container != NULL) {
		json_string(f, "namespace", function->container);
	} else {
		json_null(f, "namespace");
	}
}

static void write_json_one_function_caller(FILE *f, FeriteProfileFunctionCaller *fpfc)
{
	char *realname;
	FeriteProfileFunction *caller = fpfc->caller;

	json_dict_open(f, NULL);
	write_json_name_and_namespace(f, caller);
	json_comma(f);
	if (file_exists(caller->filename)) {
		realname = realpath(caller->filename, NULL);
		json_string(f, "file", realname);
		free(realname);
	} else {
		json_string(f, "file", caller->filename);
	}
	json_comma(f); json_positive_number(f, "at", fpfc->caller_line);
	json_comma(f); json_positive_number(f, "frequency", fpfc->frequency);
	json_comma(f); json_timespec(f, "total_duration", fpfc->total_duration);
	json_dict_close(f);
}

static void write_json_callers(FILE *f, FeriteHash *callersHash)
{
	FeriteHashBucket *fhb;
	FeriteIterator iter;
	iter.curbucket = NULL;
	iter.curindex = 0;
	int has_previous_entry = 0;

	if (callersHash == NULL) {
		json_null(f, "callers");
		return;
	}

	json_array_open(f, "callers");
	while ((fhb = ferite_hash_walk(NULL, callersHash, &iter)) != NULL) {
		FeriteProfileFunctionCaller *fpfc = fhb->data;
		if (has_previous_entry) {
			json_comma(f);
		}
		write_json_one_function_caller(f, fpfc);
		has_previous_entry = 1;
	}
	json_array_close(f);
}

#define ZERO_TIME(ts) ((ts)->tv_sec == 0 && (ts)->tv_nsec == 0)
static void write_json_one_function_callee(FILE *f, FeriteProfileFunction *callee) {
	struct timespec *inclusive;

	json_dict_open(f, NULL);
	json_string(f, "name", callee->name);
	json_comma(f); json_positive_number(f, "start_line", callee->start_line);
	json_comma(f);
	if (callee->container != NULL) {
		json_string(f, "namespace", callee->container);
	} else {
		json_null(f, "namespace");
	}
	json_comma(f); json_positive_number(f, "hits", callee->ncalls);
	json_comma(f); json_bool(f, "is_native", callee->is_native);

	inclusive = &callee->inclusive_duration;
	if (ZERO_TIME(inclusive) && !ZERO_TIME(&callee->exclusive_duration)) {
		/* We have eclusive duration but zero inclusive duration.  This
		 * may happen if this function is called from a native function
		 * so we have no record of the total time it spent doing its
		 * stuff (inclusive time). We know that it calls some other
		 * function because its exclusive_duration is not zero.  So set
		 * its inclusive_duration to be equal to its exclusive_duration
		 */
		inclusive = &callee->exclusive_duration;
	}

	json_comma(f); json_timespec(f, "inclusive_duration", *inclusive);
	json_comma(f); json_timespec(f, "exclusive_duration", callee->exclusive_duration);
	json_comma(f);

	write_json_callers(f, callee->callers);

	json_dict_close(f);
}

static void write_json_functions(FILE *f, FeriteHash *hash)
{
	FeriteHashBucket *fhb;
	int has_previous_entry = 0;
	FeriteIterator iter;
	iter.curbucket = NULL;
	iter.curindex = 0;

	if (hash == NULL) {
		json_null(f, "functions");
		return;
	}

	json_array_open(f, "functions");
	while ((fhb = ferite_hash_walk(NULL, hash, &iter)) != NULL) {
		FeriteProfileFunction *callee = fhb->data;
		if (has_previous_entry) {
			json_comma(f);
		}
		write_json_one_function_callee(f, callee);
		has_previous_entry = 1;
	}

	json_array_close(f);
}

static void write_line_profile(FILE *f, FeriteProfileLineEntry *le)
{
	if (le->ncalls == 0 && le->functions == NULL) {
		json_null(f, NULL);
		return;

	}

	json_dict_open(f, NULL);
	json_positive_number(f, "hits", le->ncalls);
	json_comma(f); json_timespec(f, "total_duration", le->total_duration);
	json_comma(f);

	write_json_functions(f, le->functions);
	json_dict_close(f);
}

static void write_json_profile_line_entries_for_one_file(FILE *f, FeriteProfileEntry *pe)
{
	FeriteProfileLineEntry *le;
	char path[PATH_MAX];
	size_t line_no;
	char *filename = pe->filename;
	#define LINE_METRIC_SZ 128
	int has_previous_entry = 0;

	if (realpath(pe->filename, path) != NULL)
		filename = path;
	else {
		if (ENOENT != errno) {
			perror(pe->filename);

		}
	}

	json_array_open(f, filename);

	for (line_no = 1; line_no <= pe->line_count; line_no++) {
		le = &pe->lines[line_no];
		if (has_previous_entry) {
			json_comma(f);
		}
		write_line_profile(f, le);
		has_previous_entry = 1;
	}

	json_array_close(f);
}

static void save_json_profile(const pid_t pid)
{
	int i;
	FILE *f;
	char filename[PATH_MAX];
	int has_previous_entry = 0;
	struct timespec total_duration;
	struct timespec profile_end_ts;

	LOCK_PROFILE();

#ifdef FERITE_FAKE_CLOCK
	CLOCK_GETTIME(&profile_end_ts);
#else
	clock_gettime(CLOCK_REALTIME, &profile_end_ts);
#endif
	total_duration = timespec_diff(&profile_start_ts, &profile_end_ts);

	if (format_profile_filename(profile_output, filename, pid) == 0) {
		UNLOCK_PROFILE();
		return;
	}

	f = fopen(filename, "w");
	if (f == NULL) {
		perror(filename);
		UNLOCK_PROFILE();
		return;
	}

	json_dict_open(f, NULL);
	json_timespec(f, "start", profile_start_ts);
	json_comma(f); json_timespec(f, "stop", profile_end_ts);
	json_comma(f); json_timespec(f, "duration", total_duration);
	json_comma(f); json_dict_open(f, "files");

	for (i = 0; i < FERITE_PROFILE_NHASH; i++) {
		FeriteProfileEntry *pe = profile_entries[i];
		while (pe) {
			if (has_previous_entry) {
				json_comma(f);
			}
			write_json_profile_line_entries_for_one_file(f, pe);
			has_previous_entry = 1;
			pe = pe->next;
		}
	}

	json_dict_close(f);
	json_dict_close(f);

	UNLOCK_PROFILE();

	if (fclose(f) == EOF)
		perror(filename);

}

void ferite_profile_save(const pid_t pid)
{
  save_json_profile(pid);
}

void ferite_profile_set_filename_format(const char *filename)
{
	LOCK_PROFILE();

	if (strncmp(profile_output, filename, PATH_MAX) == 0) {
		UNLOCK_PROFILE();
		return;
	}
	strncpy(profile_output, filename, PATH_MAX);
	profile_output[PATH_MAX-1] = '\0';

	UNLOCK_PROFILE();
}
#endif
