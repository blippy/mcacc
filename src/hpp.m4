include(common.m4)
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

#include <map>
#include <string>
#include <vector>

dnl include(nacc.m4)

include(SRC_M4)

CONTAINER
NAME_S load();
void save(const NAME_S &xs);

`#'endif
