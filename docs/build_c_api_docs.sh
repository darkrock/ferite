#!/bin/sh

if test ! -d generated; then
    mkdir generated;
fi
cd generated


root="../.."
rm -rf capi/
mkdir capi/

"$1""feritedoc" --file $root/src/ferite.c \
	--extra-file $root/src/ferite_amt.c \
    --extra-file $root/src/ferite_buffer.c \
    --extra-file $root/src/ferite_class.c \
    --extra-file $root/src/ferite_compile.c \
    --extra-file $root/src/ferite_error.c \
    --extra-file $root/src/ferite_execute.c \
    --extra-file $root/src/ferite_function.c \
    --extra-file $root/src/ferite_gc.c \
    --extra-file $root/src/ferite_gc_generation.c \
    --extra-file $root/src/ferite_globals.c \
    --extra-file $root/src/ferite_hash.c \
    --extra-file $root/src/ferite_mem_classic.c \
    --extra-file $root/src/ferite_mem_jedi.c \
    --extra-file $root/src/ferite_module.c \
    --extra-file $root/src/ferite_namespace.c \
    --extra-file $root/src/ferite_obj.c \
    --extra-file $root/src/ferite_opcode.c \
    --extra-file $root/src/ferite_ops.c \
    --extra-file $root/src/ferite_regex.c \
    --extra-file $root/src/ferite_script.c \
    --extra-file $root/src/ferite_stack.c \
    --extra-file $root/src/ferite_string.c \
    --extra-file $root/src/ferite_thread.c \
    --extra-file $root/src/ferite_uarray.c \
    --extra-file $root/src/ferite_utils.c \
    --extra-file $root/src/ferite_variables.c \
    --extra-file $root/include/ferite.h \
    --extra-file $root/include/ferite/farray.h \
    --extra-file $root/include/ferite/fbuffer.h \
    --extra-file $root/include/ferite/fcompile.h \
    --extra-file $root/include/ferite/fdebug.h \
    --extra-file $root/include/ferite/ferror.h \
    --extra-file $root/include/ferite/ffunction.h \
    --extra-file $root/include/ferite/fgc.h \
    --extra-file $root/include/ferite/fglobals.h \
    --extra-file $root/include/ferite/fhash.h \
    --extra-file $root/include/ferite/fmem.h \
    --extra-file $root/include/ferite/fmem_jedi.h \
    --extra-file $root/include/ferite/fmodule.h \
    --extra-file $root/include/ferite/fns.h \
    --extra-file $root/include/ferite/fobj.h \
    --extra-file $root/include/ferite/foop.h \
    --extra-file $root/include/ferite/fopcode.h \
    --extra-file $root/include/ferite/fops.h \
    --extra-file $root/include/ferite/fparser.h \
    --extra-file $root/include/ferite/fregex.h \
    --extra-file $root/include/ferite/freq.h \
    --extra-file $root/include/ferite/fstack.h \
    --extra-file $root/include/ferite/fstring.h \
    --extra-file $root/include/ferite/fstructs.h \
    --extra-file $root/include/ferite/fthread.h \
    --extra-file $root/include/ferite/futils.h \
    --extra-file $root/include/ferite/fvariables.h \
    --extra-file $root/include/ferite/famt.h \
    --template html \
    --prefix capi/
