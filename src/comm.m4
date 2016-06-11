define(`NAME', `comm')
defstruct
	sfield(ticker, STRING, 6)
	sfield(down, STRING, 1)
	sfield(typ, STRING, 4)
	sfield(unit, STRING, 3)
	sfield(desc, STRING, 20)
endstruct()

define(`CONTAINER', `typedef std::map<std::string, NAME_T> NAME_S;')
define(`MAPKEY', `ticker')

