bin_PROGRAMS = mcacc 
BUILT_SOURCES = assets_mcacc_htm.xxd
man1_MANS = mcacc.1
mcacc_LDADD = -lpthread -lboost_system -lboost_filesystem -lboost_program_options
#-lasprintf
mcacc_SOURCES = assets.cc \ 
		centis.cc cgt.cc comm.cc common.cc \
		epics.cc etb.cc etran.cc etrans-aug.cc \
		gaap.cc \
		inputs.cc \
		mcacc.cc \
		nacc.cc \
		parse.cc posts.cc \
		reusable.cc stend.cc \ 
		tests.cc types.cc \
		wiegley.cc \
		yahoo.cc yproc.cc

mcacc_SOURCES += $(BUILT_SOURCES)

CLEANFILES = $(BUILT_SOURCES)


assets_mcacc_htm.xxd : ../assets/mcacc.htm
	xxd -i $^ $@

assets.cc : assets_mcacc_htm.xxd

#tags :
#	ctags -R *
