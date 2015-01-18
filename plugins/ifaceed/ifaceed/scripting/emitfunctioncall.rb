print "/*! \\file functioncall.h
	\\author HiddenSeeker

	Describes a call from function data
 */
#pragma once
#include \"callable.h\"

namespace scripting
{
	
/*! A function call for calling specific simple function
 */
template<
	typename _ReturnType
>
class FunctionCall
{
public:
"

for count in 0..16
		print "/*! Registers a constructor call
	\\param[in] name a name of object
	\\param[in] s scripting object
 */\n"
	if count != 0
		print "
template<
"
		for i in 0..count - 1
			print "	typename _ArgType" + i.to_s
			if (i != count - 1)
				print ","
			end
			print "\n"
		end
		print ">\n"
	end
	print "class WithArgs" + count.to_s + ": public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)("
	for i in 0..count - 1
		print "_ArgType" + i.to_s
		if (i != count - 1)
			print ", "
		end
	end
	print ");\n\n"
	print "\t/*! Represents a constructor call for a function with 2 arguments
	    \\param[in] e engine
	    \\param[in] name a name
		\\param[in] s function
	 */
	WithArgs" + count.to_s + "(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, " + count.to_s + "), m_f(s)
	{
		
	}\n\n"
	print "\t/*! Can be inherited
	 */
	virtual ~WithArgs" + count.to_s + "()
	{
	
	}\n"
	print "\n\t/*! Whether function can be called
	    \\param[in] context a context element
	    \\return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
	"
	print "\tcheckArgumentCount(result, context);\n"
	for i in 0..count - 1
		print "\t\tcheckArgument<_ArgType" + i.to_s + ">(result, " + i.to_s + ", context);\n"
	end
	print "\t\treturn result;\n"
	print "\t}\n"	
	print "\n\t/*! Calls  a function
		\\param[in] ctx context
		\\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)\n\t{\n"
	for i in 0..count - 1
		print "\t\tsad::Maybe<_ArgType" + i.to_s + "> value" + i.to_s + " = scripting::ToValue<_ArgType" + i.to_s + ">::perform(ctx->argument(" + i.to_s + "));\n"
	end
	print "\t\treturn scripting::FromValue<_ReturnType>::perform(m_f(\n"
	for i in 0..count - 1
		print "\t\t\tvalue" + i.to_s + ".value()"
		if i != count -1
			print ","
		end
		print "\n"
	end
	print "\t\t), engine);\n"
	print "\t}\n"
	print "protected:\n"
	print "\tSignature m_f;\n"
	print "};\n\n"
end

print "};\n\n"

print "/*! A function call for calling specific simple function with void signature
 */
template<
>
class FunctionCall<void>
{
public:
"

for count in 0..16
		print "/*! Registers a constructor call
	\\param[in] name a name of object
	\\param[in] s scripting object
 */\n"
	if count != 0
		print "
template<
"
		for i in 0..count - 1
			print "	typename _ArgType" + i.to_s
			if (i != count - 1)
				print ","
			end
			print "\n"
		end
		print ">\n"
	end
	print "class WithArgs" + count.to_s + ": public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)("
	for i in 0..count - 1
		print "_ArgType" + i.to_s
		if (i != count - 1)
			print ", "
		end
	end
	print ");\n\n"
	print "\t/*! Represents a constructor call for a function with 2 arguments
	    \\param[in] e engine
	    \\param[in] name a name
		\\param[in] s function
	 */
	WithArgs" + count.to_s + "(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, " + count.to_s +"), m_f(s)
	{
		
	}\n\n"
	print "\t/*! Can be inherited
	 */
	virtual ~WithArgs" + count.to_s + "()
	{
	
	}\n"
	print "\n\t/*! Whether function can be called
	    \\param[in] context a context element
	    \\return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
	"
	print "\tcheckArgumentCount(result, context);\n"
	for i in 0..count - 1
		print "\t\tcheckArgument<_ArgType" + i.to_s + ">(result, " + i.to_s + ", context);\n"
	end
	print "\t\treturn result;\n"
	print "\t}\n"	
	print "\n\t/*! Calls  a function
		\\param[in] ctx context
		\\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)\n\t{\n"
	for i in 0..count - 1
		print "\t\tsad::Maybe<_ArgType" + i.to_s + "> value" + i.to_s + " = scripting::ToValue<_ArgType" + i.to_s + ">::perform(ctx->argument(" + i.to_s + "));\n"
	end
	print "\t\tm_f(\n"
	for i in 0..count - 1
		print "\t\t\tvalue" + i.to_s + ".value()"
		if i != count -1
			print ","
		end
		print "\n"
	end
	print "\t\t);\n"
	print "\t\treturn ctx->thisObject();\n"
	print "\t}\n"
	print "protected:\n"
	print "\tSignature m_f;\n"
	print "};\n\n"
end

print "};\n\n"

print "\n}\n"

