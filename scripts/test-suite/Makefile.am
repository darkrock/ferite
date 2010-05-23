testscripts = \
  array.fe \
  console.fe \
  date.fe \
  filesystem.fe \
  ipc.fe \
  math.fe \
  number.fe \
  network.fe \
  posix.fe \
  reflection.fe \
  regexp.fe \
  rmi.fe \
  serialize.fe \
  stream.fe \
  string.fe \
  sys.fe \
  thread.fe \
  unix.fe \
  xml.fe 

testscripts_DATA = \
  $(testscripts) \
  README

testscriptsdir = $(prefix)/share/ferite/test-suite

EXTRA_DIST = $(testscripts_DATA)

test:
	@(FCOUNT=0; TCOUNT=0; for file in $(testscripts); do \
	  echo -n "Running test on $$file... "; \
	  SOMEVALUE=$$((TCOUNT++)); \
	  ferite $$file > /dev/null 2> /dev/null; RES=$$?; \
	  if [ $$RES -gt 0 ]; then \
	    echo "FAILED!"; \
	    SOMEVALUE=$$((FCOUNT++)); \
	  else echo "PASSED!"; \
	  fi; \
	done; echo ""; echo "Result: $$FCOUNT out of $$TCOUNT tests failed")

test-verbose:
	@(FCOUNT=0; TCOUNT=0; for file in $(testscripts); do \
	  echo -n "Running test on $$file... "; \
	  SOMEVALUE=$$((TCOUNT++)); \
	  ferite $$file; RES=$$?; \
	  if [ $$RES -gt 0 ]; then \
	    echo "FAILED!"; \
	    SOMEVALUE=$$((FCOUNT++)); \
	  else echo "PASSED!"; \
	  fi; \
	done; echo ""; echo "Result: $$FCOUNT out of $$TCOUNT tests failed")
