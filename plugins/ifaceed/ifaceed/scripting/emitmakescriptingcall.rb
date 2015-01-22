print "/*! \\file makescriptingcall.h
	\\author HiddenSeeker

	Contains a function for making a call for function with scripting
 */
#pragma once
#include \"scriptingcall.h\"
#include \"scripting.h\"

namespace scripting
{
"

for count in 0..16
		print "/*! Makes new scripting call
	\\param[in] f a function
	\\param[in] s engine
 */
template<
	typename _ReturnType"
	if count != 0
		print ","
	end
	print "\n"
	for i in 0..count - 1
		print "	typename _ArgType" + i.to_s
		if (i != count - 1)
			print ","
		end
		print "\n"
	end
	print ">\n"
	print "scripting::Callable* make_scripting_call(_ReturnType (*f)(scripting::Scripting*"
	if count != 0
		print ","
	end
	for i in 0..count - 1
		print "_ArgType" + i.to_s
		if (i != count - 1)
			print ", "
		end
	end
	print "), scripting::Scripting* s)\n{\n";
	print "\treturn new typename scripting::ScriptingFunctionCall<_ReturnType>::template WithArgs"  + count.to_s + "<"
	for i in 0..count - 1
		print "_ArgType" + i.to_s
		if (i != count - 1)
			print ", "
		end
	end
	print ">(s->engine(), \"\", f);\n"
	print "}\n\n"
end

print "}\n"
