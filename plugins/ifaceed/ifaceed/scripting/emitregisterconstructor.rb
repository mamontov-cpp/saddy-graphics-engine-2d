print "/*! \\file registerconstructor.h
	\\author HiddenSeeker

	Defines register constructor function
 */
#pragma once
#include \"callable.h\"
#include \"scripting.h\"

namespace scripting
{
"

for count in 0..16
		print "/*! Registers a constructor call
	\\param[in] name a name of object
	\\param[in] s scripting object
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
	print "void register_constructor(const QString& name, scripting::Scripting* s)\n{\n"
	print "\tQScriptClass* c = new scripting::ConstructorCall"  + count.to_s + "<_Constructed"
	if count != 0
		print ", "
	end
	for i in 0..count - 1
		print "_Arg" + i.to_s
		if (i != count - 1)
			print ", "
		end
	end
	print ">(s->engine(), name);\n"
	print "\ts->registerScriptClass(name, c);\n"
	print "}\n\n"
end

print "}\n"
