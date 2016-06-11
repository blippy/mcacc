bin_PROGRAMS = mcacc 
BUILT_SOURCES = autotypes.cc autotypes.h comm.hpp comm.cc nacc.hpp nacc.cc
mcacc_LDADD = -lpthread
mcacc_SOURCES = common.cc epics.cc etb.cc \
		etrans-aug.cc \
		mcacc.cc \
		parse.cc posts.cc \
		reusable.cc stend.cc types.cc yahoo.cc \
		$(BUILT_SOURCES)

CLEANFILES = $(BUILT_SOURCES)

autotypes.cc autotypes.h : autotypes.tcl
	tclsh autotypes.tcl

nacc.hpp nacc.cc: nacc.m4 hpp.m4 cc.m4 common.m4
	m5 nacc

comm.hpp comm.cc : comm.m4 hpp.m4 cc.m4 common.m4
	m5 comm


#tags :
#	ctags -R *
