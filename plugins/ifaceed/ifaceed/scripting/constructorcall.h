/*! \file constructorcall.h
	\author HiddenSeeker

	Describes a constructor call, which returns new constructed object
 */
#pragma once
#include "callable.h"

namespace scripting
{
	
/*! A functor class, which represents constructable call with 2 arguments
 */
template<
	typename _Constructed,
	typename _Arg1,
	typename _Arg2
>
class ConstructorCall2: public Callable
{
public:
	/*! Represents a constructor call for a function with two arguments
	 */
	ConstructorCall2(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 2)
	{
		
	}
	virtual ~ConstructorCall2()
	{
	
	}
	/*! Whether constructor can be called
		\param[in] context a context element
		\return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
		sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_Arg1>(result, 0, context);
		checkArgument<_Arg1>(result, 1, context);
		return result;
	}
	/*! Calls  an object
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_Arg1> value1 = scripting::ToValue<_Arg1>::perform(ctx->argument(0));
		sad::Maybe<_Arg2> value2 = scripting::ToValue<_Arg2>::perform(ctx->argument(1));
		return scripting::FromValue<_Constructed>::perform(_Constructed(
			value1.value(), 
			value2.value()
		), engine);
	}
};


}