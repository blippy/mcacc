define(`NAME', `nacc')
dnl define(`NAME_S', `nacc_s')

defstruct
	sfield(acc, STRING, 3)
	sfield(alt, STRING, 3)
	sfield(typ, STRING, 1)
	sfield(scale, double, 3)
	sfield(desc, STRING, 20)
endstruct()

define(`CONTAINER', `typedef std::map<std::string, NAME_T> NAME_S;')
define(`MAPKEY', `acc')

