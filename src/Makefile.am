bin_PROGRAMS = mcacc 
BUILT_SOURCES = assets_mcacc_htm.xxd
mcacc_LDADD = -lpthread -lboost_system -lboost_filesystem
#-lasprintf
mcacc_SOURCES = assets.cc \ 
		comm.cc common.cc \
		epics.cc etb.cc etran.cc etrans-aug.cc \
		gaap.cc \
		mcacc.cc \
		nacc.cc \
		parse.cc posts.cc \
		reusable.cc stend.cc types.cc yahoo.cc \
		$(BUILT_SOURCES)

CLEANFILES = $(BUILT_SOURCES)


assets_mcacc_htm.xxd : ../assets/mcacc.htm
	xxd -i $^ $@

assets.cc : assets_mcacc_htm.xxd

#tags :
#	ctags -R *
