print "/*! \\file makeconstructor.h
	\\author HiddenSeeker

	Defines a functions for making constructor
 */
#pragma once
#include \"callable.h\"
#include \"scripting.h\"

namespace scripting
{
"

for count in 0..16
		print "/*! Makes a constructor call
	\\param[in] s scripting object
	\\return constructor call
 */
template<
	typename _Constructed"
	if count != 0
		print ","
	end
	print "\n"
	for i in 0..count - 1
		print "	typename _Arg" + i.to_s
		if (i != count - 1)
			print ","
		end
		print "\n"
	end
	print ">\n"
	print "scripting::Callable* make_constructor(scripting::Scripting* s)\n{\n"
	print "\treturn new scripting::ConstructorCall"  + count.to_s + "<_Constructed"
	if count != 0
		print ", "
	end
	for i in 0..count - 1
		print "_Arg" + i.to_s
		if (i != count - 1)
			print ", "
		end
	end
	print ">(s->engine(), \"\");\n"
	print "}\n\n"
end

print "}\n"
