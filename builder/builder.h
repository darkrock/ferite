/*
 * Copyright (C) 2000-2002 Chris Ross and various contributors
 * Copyright (C) 1999-2000 Chris Ross
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * o Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * o Neither the name of the ferite software nor the names of its contributors may
 *   be used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __BUIDLER_H__
#define __BUIDLER_H__

typedef struct __builder_module {
   char  name[1024];
   FILE *core;
   FILE *general;
   FILE *header;
   FILE *Makefile;
   FILE *flags;
   FILE *misc;
   FeriteStack *name_stack;
   FeriteStack *current_state;
} BuilderModule;

#define MakefileTemplate \
  "## Process this file with automake to produce Makefile.in\n"                      \
  "\n"                                                                               \
  "AUTOMAKE_OPTIONS     = 1.4 foreign\n\n"                                           \
  "# A list of all the files in the current directory which can be regenerated\n"    \
  "MAINTAINERCLEANFILES = %s*.h %s*.c %s*~\n\n"                                      \
  "CLEANFILES       = \n"                                                            \
  "\n"                                                                               \
  "if NEED_FERITE_LIB\n"                                                             \
  "libferite = -L${top_builddir}/src -lferite\n"                                     \
  "endif\n\n"                                                                        \
  "LDFLAGS          = $(libferite) -L${libdir} @%s_LIBS@\n"                          \
  "INCLUDES         = -I$(top_srcdir)/include -I$(prefix)/include -I. @%s_CFLAGS@\n" \
  "DEFS             = @thread_defs@\n"                                               \
  "\nscripts_DATA   = %s\nscriptsdir       = @FE_XPLAT_LIBRARY_PATH@\n\n"            \
  "\nmodxml_DATA    = %s.xml\nmodxmldir       = @FE_LIBRARY_PATH@/module-descriptions\n" \
  "EXTRA_DIST       = $(scripts_DATA) $(modxml_DATA)\n"                              \
  "pkgdir           = @FE_NATIVE_LIBRARY_PATH@\n"                                    \
  "pkg_LTLIBRARIES  = %s.la\n"                                                       \
  "\n"                                                                               \
  "%s_la_SOURCES    = %s %s\n"                                                       \
  "%s_la_LDFLAGS    = -no-undefined -module -avoid-version\n"                        \
  "%s_la_LIBADD     =\n"                                                             \
  "\n"                                                                               \
  "%s_core.c: %s\n"                                                                  \
  "\t%s -m %s %s/%s/%s\n\n%s"    
  
  /* modulename, modulename, modulefiles, modulename, modulename */

#define ConfigM4Template \
   "%s_LIBS=\"\"\n" \
   "%s_CFLAGS=\"\"\n" \
   "AC_SUBST(%s_LIBS)\n" \
   "AC_SUBST(%s_CFLAGS)\n" \
   "\nmodules=\"$modules %s\"\n"

#define ModuleXMLTemplateStart \
   "<?xml version=\"1.0\" ?>\n" \
   "<module>\n" \
   "    <name>%s</name>\n" \
   "    <documentation-list>\n" 
#define ModuleXMLTemplateEnd \
   "    </documentation-list>\n" \
   "    <dependance-list>\n" \
   "    </dependance-list>\n" \
   "</module>\n"

#define ModuleXMLIncludeTemplate \
   "        <include>%s</include>\n"

void print_version();
void print_copyright();
void print_usage();
void show_help();
void parse_args( int argc, char **argv );
char *builder_generate_current_name( int to_end, int want_fe_prefix  );
char *builder_generate_current_function_name();
void builder_process_variable( FeriteScript *script, FeriteVariable *var, char *parent );
void builder_process_closed_function( FeriteScript *script, FeriteFunction *fnc, char *parent );
void builder_process_open_function( FeriteScript *script, FeriteFunction *fnc, char *parent );
void builder_process_function( FeriteScript *script, FeriteFunction *fnc, char *parent );
void builder_process_open_class( FeriteScript *script, FeriteClass *cls, char *parent );
void builder_process_closed_class( FeriteScript *script, FeriteClass *cls, char *parent );
void builder_process_class( FeriteScript *script, FeriteClass *cls, char *parent );
void builder_process_closed_namespace_element( FeriteScript *script, FeriteNamespaceBucket *nsb, char *parent );
void builder_process_open_namespace_element( FeriteScript *script, FeriteNamespaceBucket *nsb, char *parent );
void builder_process_namespace_element( FeriteScript *script, FeriteNamespaceBucket *nsb, char *parent );
void builder_dump_plan( FeriteScript *script );

#endif /* __BUIDLER_H__ */
