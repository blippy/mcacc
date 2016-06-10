divert(-1)
dnl just define header name
define(`upcase', `translit(`$*', `a-z', `A-Z')')

define(`SRC_M4', nacc.m4)
include(SRC_M4)
dnl include(nacc.m4)

define(H_FILE, upcase(defn(`NAME'))`_H')
define(`NAME_T', `defn(`NAME')_t')
define(`NAME_S', `defn(`NAME')_ts')
divert
