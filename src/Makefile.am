bin_PROGRAMS = mcacc 
BUILT_SOURCES = autotypes.cc autotypes.h nacc.hpp nacc.cc
mcacc_SOURCES = common.cc epics.cc etb.cc \
		etrans-aug.cc \
		mcacc.cc \
		parse.cc posts.cc \
		reusable.cc stend.cc types.cc \
		$(BUILT_SOURCES)

CLEANFILES = $(BUILT_SOURCES)

autotypes.cc autotypes.h : autotypes.tcl
	tclsh autotypes.tcl

nacc.hpp : hpp.m4 nacc.m4
	m4 hpp.m4  > nacc.hpp

nacc.cc : cpp.m4 nacc.m4
	m4 cpp.m4  > nacc.cc

#tags :
#	ctags -R *
