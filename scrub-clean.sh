#!/usr/bin/env bash
(cd scripts; rm -f Makefile Makefile.in)
rm -rf src/.deps
rm -f src/Makefile.in src/Makefile src/mcacc src/*.o
rm -f aclocal.m4      configure  install-sh  Makefile.in configure.lineno
rm -rf autom4te.cache
rm -f depcomp           missing
rm -f config.log      config.status   Makefile  

