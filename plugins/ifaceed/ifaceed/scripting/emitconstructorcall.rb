print "/*! \\file constructorcall.h
	\\author HiddenSeeker

	Describes a constructor call, which returns new constructed object
 */
#pragma once
#include \"callable.h\"

namespace scripting
{
"

for count in 0..16
	print "/*! A functor class, which represents constructable call with " + count.to_s + " arguments
 */
template<
	typename _Constructed,
"
	for i in 0..count - 1
		print "	typename _Arg" + i.to_s
		if (i != count - 1)
			print ","
		end
		print "\n"
	end
	print ">
class ConstructorCall" + count.to_s + ": public Callable
{
public:
"
	print "\t/*! Represents a constructor call for a function with " + count.to_s + " arguments
	    \\param[in] e engine
	    \\param[in] name a name
	 */
	ConstructorCall" + count.to_s + "(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, " + count.to_s + ")
	{
		
	}
"
	print "\t/*! Can be inherited
	 */
	virtual ~ConstructorCall" + count.to_s + "()
	{
	
	}
	"
	print "/*! Whether constructor can be called
	    \\param[in] context a context element
	    \\return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
	"
	print "\tcheckArgumentCount(result, context);\n"
	for i in 0..count - 1
		print "\t\tcheckArgument<_Arg" + i.to_s + ">(result, " + i.to_s + ", context);\n"
	end
	print "\t\treturn result;\n"
	print "\t}\n"	
	print "\t/*! Calls  an object
		\\param[in] ctx context
		\\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)\n\t{\n"
	for i in 0..count - 1
		print "\t\tsad::Maybe<_Arg" + i.to_s + "> value" + i.to_s + " = scripting::ToValue<_Arg" + i.to_s + ">::perform(ctx->argument(" + i.to_s + "));\n"
	end
	print "\t\treturn scripting::FromValue<_Constructed>::perform(_Constructed(\n"
	for i in 0..count - 1
		print "\t\t\tvalue" + i.to_s + ".value()"
		if i != count -1
			print ","
		end
		print "\n"
	end
	print "\t\t), engine);\n"
	print "\t}\n"
	print "};\n\n"
end

print "}\n"