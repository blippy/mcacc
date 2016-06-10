divert(-1)
dnl just define header name
define(`upcase', `translit(`$*', `a-z', `A-Z')')
include(nacc.m4)
define(H_FILE, upcase(defn(`NAME'))`_H')
define(`NAME_T', `defn(`NAME')_t')
define(`NAME_S', `defn(`NAME')_ts')
divert
`#'ifndef H_FILE
`#'define H_FILE
/* This header file was automatically created. Do not edit */

divert(-1)
dnl define(NAME_S, `$1_s')
dnl define(`defstruct', `define(NAME_S, `$1_s')typedef struct NAME_S {')
define(`defstruct', `typedef struct NAME_T {')
define(STRING, std::string)
define(`sfield', `$2 $1;') 
define(`endstruct', `} NAME_T;')
divert


include(nacc.m4)

NAME_S load();
save(const NAME_S &xs);

`#'endif
