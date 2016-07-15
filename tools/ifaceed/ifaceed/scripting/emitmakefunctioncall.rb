print "/*! \\file makefunctioncall.h
	

	Contains a function for making a function call
 */
#pragma once
#include \"functioncall.h\"
#include \"scripting.h\"

namespace scripting
{
"

for count in 0..16
		print "/*! Makes new function call
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
	print "scripting::Callable* make_function_call(_ReturnType (*f)("
	for i in 0..count - 1
		print "_ArgType" + i.to_s
		if (i != count - 1)
			print ", "
		end
	end
	print "), scripting::Scripting* s)\n{\n";
	print "\treturn new typename scripting::FunctionCall<_ReturnType>::template WithArgs"  + count.to_s + "<"
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
