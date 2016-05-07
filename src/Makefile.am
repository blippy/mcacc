bin_PROGRAMS = mcacc 
mcacc_SOURCES = common.cc epics.cc etb.cc \
		etrans-aug.cc \
		mcacc.cc \
		parse.cc posts.cc stend.cc types.cc \
		autotypes.cc
BUILT_SOURCES = autotypes.cc autotypes.h
CLEANFILES = $(BUILT_SOURCES)

autotypes.cc autotypes.h : autotypes.tcl
	tclsh autotypes.tcl

#tags :
#	ctags -R *
