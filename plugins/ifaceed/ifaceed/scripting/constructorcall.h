/*! \file constructorcall.h
	\author HiddenSeeker

	Describes a constructor call, which returns new constructed object
 */
#pragma once
#include "callable.h"

namespace scripting
{
/*! A functor class, which represents constructable call with 0 arguments
 */
template<
	typename _Constructed
>
class ConstructorCall0: public Callable
{
public:
	/*! Represents a constructor call for a function with 0 arguments
	    \param[in] e engine
	    \param[in] name a name
	 */
	ConstructorCall0(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 0)
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ConstructorCall0()
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
		return result;
	}
	/*! Calls  an object
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext*, QScriptEngine* engine)
	{
		return scripting::FromValue<_Constructed>::perform(_Constructed(
		), engine);
	}
};

/*! A functor class, which represents constructable call with 1 arguments
 */
template<
	typename _Constructed,
	typename _Arg0
>
class ConstructorCall1: public Callable
{
public:
	/*! Represents a constructor call for a function with 1 arguments
	    \param[in] e engine
	    \param[in] name a name
	 */
	ConstructorCall1(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 1)
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ConstructorCall1()
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
		checkArgument<_Arg0>(result, 0, context);
		return result;
	}
	/*! Calls  an object
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_Arg0> value0 = scripting::ToValue<_Arg0>::perform(ctx->argument(0));
		return scripting::FromValue<_Constructed>::perform(_Constructed(
			value0.value()
		), engine);
	}
};

/*! A functor class, which represents constructable call with 2 arguments
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1
>
class ConstructorCall2: public Callable
{
public:
	/*! Represents a constructor call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
	 */
	ConstructorCall2(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 2)
	{
		
	}
	/*! Can be inherited
	 */
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
		checkArgument<_Arg0>(result, 0, context);
		checkArgument<_Arg1>(result, 1, context);
		return result;
	}
	/*! Calls  an object
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_Arg0> value0 = scripting::ToValue<_Arg0>::perform(ctx->argument(0));
		sad::Maybe<_Arg1> value1 = scripting::ToValue<_Arg1>::perform(ctx->argument(1));
		return scripting::FromValue<_Constructed>::perform(_Constructed(
			value0.value(),
			value1.value()
		), engine);
	}
};

/*! A functor class, which represents constructable call with 3 arguments
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2
>
class ConstructorCall3: public Callable
{
public:
	/*! Represents a constructor call for a function with 3 arguments
	    \param[in] e engine
	    \param[in] name a name
	 */
	ConstructorCall3(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 3)
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ConstructorCall3()
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
		checkArgument<_Arg0>(result, 0, context);
		checkArgument<_Arg1>(result, 1, context);
		checkArgument<_Arg2>(result, 2, context);
		return result;
	}
	/*! Calls  an object
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_Arg0> value0 = scripting::ToValue<_Arg0>::perform(ctx->argument(0));
		sad::Maybe<_Arg1> value1 = scripting::ToValue<_Arg1>::perform(ctx->argument(1));
		sad::Maybe<_Arg2> value2 = scripting::ToValue<_Arg2>::perform(ctx->argument(2));
		return scripting::FromValue<_Constructed>::perform(_Constructed(
			value0.value(),
			value1.value(),
			value2.value()
		), engine);
	}
};

/*! A functor class, which represents constructable call with 4 arguments
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3
>
class ConstructorCall4: public Callable
{
public:
	/*! Represents a constructor call for a function with 4 arguments
	    \param[in] e engine
	    \param[in] name a name
	 */
	ConstructorCall4(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 4)
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ConstructorCall4()
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
		checkArgument<_Arg0>(result, 0, context);
		checkArgument<_Arg1>(result, 1, context);
		checkArgument<_Arg2>(result, 2, context);
		checkArgument<_Arg3>(result, 3, context);
		return result;
	}
	/*! Calls  an object
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_Arg0> value0 = scripting::ToValue<_Arg0>::perform(ctx->argument(0));
		sad::Maybe<_Arg1> value1 = scripting::ToValue<_Arg1>::perform(ctx->argument(1));
		sad::Maybe<_Arg2> value2 = scripting::ToValue<_Arg2>::perform(ctx->argument(2));
		sad::Maybe<_Arg3> value3 = scripting::ToValue<_Arg3>::perform(ctx->argument(3));
		return scripting::FromValue<_Constructed>::perform(_Constructed(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value()
		), engine);
	}
};

/*! A functor class, which represents constructable call with 5 arguments
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4
>
class ConstructorCall5: public Callable
{
public:
	/*! Represents a constructor call for a function with 5 arguments
	    \param[in] e engine
	    \param[in] name a name
	 */
	ConstructorCall5(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 5)
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ConstructorCall5()
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
		checkArgument<_Arg0>(result, 0, context);
		checkArgument<_Arg1>(result, 1, context);
		checkArgument<_Arg2>(result, 2, context);
		checkArgument<_Arg3>(result, 3, context);
		checkArgument<_Arg4>(result, 4, context);
		return result;
	}
	/*! Calls  an object
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_Arg0> value0 = scripting::ToValue<_Arg0>::perform(ctx->argument(0));
		sad::Maybe<_Arg1> value1 = scripting::ToValue<_Arg1>::perform(ctx->argument(1));
		sad::Maybe<_Arg2> value2 = scripting::ToValue<_Arg2>::perform(ctx->argument(2));
		sad::Maybe<_Arg3> value3 = scripting::ToValue<_Arg3>::perform(ctx->argument(3));
		sad::Maybe<_Arg4> value4 = scripting::ToValue<_Arg4>::perform(ctx->argument(4));
		return scripting::FromValue<_Constructed>::perform(_Constructed(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value()
		), engine);
	}
};

/*! A functor class, which represents constructable call with 6 arguments
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5
>
class ConstructorCall6: public Callable
{
public:
	/*! Represents a constructor call for a function with 6 arguments
	    \param[in] e engine
	    \param[in] name a name
	 */
	ConstructorCall6(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 6)
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ConstructorCall6()
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
		checkArgument<_Arg0>(result, 0, context);
		checkArgument<_Arg1>(result, 1, context);
		checkArgument<_Arg2>(result, 2, context);
		checkArgument<_Arg3>(result, 3, context);
		checkArgument<_Arg4>(result, 4, context);
		checkArgument<_Arg5>(result, 5, context);
		return result;
	}
	/*! Calls  an object
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_Arg0> value0 = scripting::ToValue<_Arg0>::perform(ctx->argument(0));
		sad::Maybe<_Arg1> value1 = scripting::ToValue<_Arg1>::perform(ctx->argument(1));
		sad::Maybe<_Arg2> value2 = scripting::ToValue<_Arg2>::perform(ctx->argument(2));
		sad::Maybe<_Arg3> value3 = scripting::ToValue<_Arg3>::perform(ctx->argument(3));
		sad::Maybe<_Arg4> value4 = scripting::ToValue<_Arg4>::perform(ctx->argument(4));
		sad::Maybe<_Arg5> value5 = scripting::ToValue<_Arg5>::perform(ctx->argument(5));
		return scripting::FromValue<_Constructed>::perform(_Constructed(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value(),
			value5.value()
		), engine);
	}
};

/*! A functor class, which represents constructable call with 7 arguments
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6
>
class ConstructorCall7: public Callable
{
public:
	/*! Represents a constructor call for a function with 7 arguments
	    \param[in] e engine
	    \param[in] name a name
	 */
	ConstructorCall7(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 7)
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ConstructorCall7()
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
		checkArgument<_Arg0>(result, 0, context);
		checkArgument<_Arg1>(result, 1, context);
		checkArgument<_Arg2>(result, 2, context);
		checkArgument<_Arg3>(result, 3, context);
		checkArgument<_Arg4>(result, 4, context);
		checkArgument<_Arg5>(result, 5, context);
		checkArgument<_Arg6>(result, 6, context);
		return result;
	}
	/*! Calls  an object
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_Arg0> value0 = scripting::ToValue<_Arg0>::perform(ctx->argument(0));
		sad::Maybe<_Arg1> value1 = scripting::ToValue<_Arg1>::perform(ctx->argument(1));
		sad::Maybe<_Arg2> value2 = scripting::ToValue<_Arg2>::perform(ctx->argument(2));
		sad::Maybe<_Arg3> value3 = scripting::ToValue<_Arg3>::perform(ctx->argument(3));
		sad::Maybe<_Arg4> value4 = scripting::ToValue<_Arg4>::perform(ctx->argument(4));
		sad::Maybe<_Arg5> value5 = scripting::ToValue<_Arg5>::perform(ctx->argument(5));
		sad::Maybe<_Arg6> value6 = scripting::ToValue<_Arg6>::perform(ctx->argument(6));
		return scripting::FromValue<_Constructed>::perform(_Constructed(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value(),
			value5.value(),
			value6.value()
		), engine);
	}
};

/*! A functor class, which represents constructable call with 8 arguments
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7
>
class ConstructorCall8: public Callable
{
public:
	/*! Represents a constructor call for a function with 8 arguments
	    \param[in] e engine
	    \param[in] name a name
	 */
	ConstructorCall8(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 8)
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ConstructorCall8()
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
		checkArgument<_Arg0>(result, 0, context);
		checkArgument<_Arg1>(result, 1, context);
		checkArgument<_Arg2>(result, 2, context);
		checkArgument<_Arg3>(result, 3, context);
		checkArgument<_Arg4>(result, 4, context);
		checkArgument<_Arg5>(result, 5, context);
		checkArgument<_Arg6>(result, 6, context);
		checkArgument<_Arg7>(result, 7, context);
		return result;
	}
	/*! Calls  an object
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_Arg0> value0 = scripting::ToValue<_Arg0>::perform(ctx->argument(0));
		sad::Maybe<_Arg1> value1 = scripting::ToValue<_Arg1>::perform(ctx->argument(1));
		sad::Maybe<_Arg2> value2 = scripting::ToValue<_Arg2>::perform(ctx->argument(2));
		sad::Maybe<_Arg3> value3 = scripting::ToValue<_Arg3>::perform(ctx->argument(3));
		sad::Maybe<_Arg4> value4 = scripting::ToValue<_Arg4>::perform(ctx->argument(4));
		sad::Maybe<_Arg5> value5 = scripting::ToValue<_Arg5>::perform(ctx->argument(5));
		sad::Maybe<_Arg6> value6 = scripting::ToValue<_Arg6>::perform(ctx->argument(6));
		sad::Maybe<_Arg7> value7 = scripting::ToValue<_Arg7>::perform(ctx->argument(7));
		return scripting::FromValue<_Constructed>::perform(_Constructed(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value(),
			value5.value(),
			value6.value(),
			value7.value()
		), engine);
	}
};

/*! A functor class, which represents constructable call with 9 arguments
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7,
	typename _Arg8
>
class ConstructorCall9: public Callable
{
public:
	/*! Represents a constructor call for a function with 9 arguments
	    \param[in] e engine
	    \param[in] name a name
	 */
	ConstructorCall9(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 9)
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ConstructorCall9()
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
		checkArgument<_Arg0>(result, 0, context);
		checkArgument<_Arg1>(result, 1, context);
		checkArgument<_Arg2>(result, 2, context);
		checkArgument<_Arg3>(result, 3, context);
		checkArgument<_Arg4>(result, 4, context);
		checkArgument<_Arg5>(result, 5, context);
		checkArgument<_Arg6>(result, 6, context);
		checkArgument<_Arg7>(result, 7, context);
		checkArgument<_Arg8>(result, 8, context);
		return result;
	}
	/*! Calls  an object
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_Arg0> value0 = scripting::ToValue<_Arg0>::perform(ctx->argument(0));
		sad::Maybe<_Arg1> value1 = scripting::ToValue<_Arg1>::perform(ctx->argument(1));
		sad::Maybe<_Arg2> value2 = scripting::ToValue<_Arg2>::perform(ctx->argument(2));
		sad::Maybe<_Arg3> value3 = scripting::ToValue<_Arg3>::perform(ctx->argument(3));
		sad::Maybe<_Arg4> value4 = scripting::ToValue<_Arg4>::perform(ctx->argument(4));
		sad::Maybe<_Arg5> value5 = scripting::ToValue<_Arg5>::perform(ctx->argument(5));
		sad::Maybe<_Arg6> value6 = scripting::ToValue<_Arg6>::perform(ctx->argument(6));
		sad::Maybe<_Arg7> value7 = scripting::ToValue<_Arg7>::perform(ctx->argument(7));
		sad::Maybe<_Arg8> value8 = scripting::ToValue<_Arg8>::perform(ctx->argument(8));
		return scripting::FromValue<_Constructed>::perform(_Constructed(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value(),
			value5.value(),
			value6.value(),
			value7.value(),
			value8.value()
		), engine);
	}
};

/*! A functor class, which represents constructable call with 10 arguments
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7,
	typename _Arg8,
	typename _Arg9
>
class ConstructorCall10: public Callable
{
public:
	/*! Represents a constructor call for a function with 10 arguments
	    \param[in] e engine
	    \param[in] name a name
	 */
	ConstructorCall10(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 10)
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ConstructorCall10()
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
		checkArgument<_Arg0>(result, 0, context);
		checkArgument<_Arg1>(result, 1, context);
		checkArgument<_Arg2>(result, 2, context);
		checkArgument<_Arg3>(result, 3, context);
		checkArgument<_Arg4>(result, 4, context);
		checkArgument<_Arg5>(result, 5, context);
		checkArgument<_Arg6>(result, 6, context);
		checkArgument<_Arg7>(result, 7, context);
		checkArgument<_Arg8>(result, 8, context);
		checkArgument<_Arg9>(result, 9, context);
		return result;
	}
	/*! Calls  an object
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_Arg0> value0 = scripting::ToValue<_Arg0>::perform(ctx->argument(0));
		sad::Maybe<_Arg1> value1 = scripting::ToValue<_Arg1>::perform(ctx->argument(1));
		sad::Maybe<_Arg2> value2 = scripting::ToValue<_Arg2>::perform(ctx->argument(2));
		sad::Maybe<_Arg3> value3 = scripting::ToValue<_Arg3>::perform(ctx->argument(3));
		sad::Maybe<_Arg4> value4 = scripting::ToValue<_Arg4>::perform(ctx->argument(4));
		sad::Maybe<_Arg5> value5 = scripting::ToValue<_Arg5>::perform(ctx->argument(5));
		sad::Maybe<_Arg6> value6 = scripting::ToValue<_Arg6>::perform(ctx->argument(6));
		sad::Maybe<_Arg7> value7 = scripting::ToValue<_Arg7>::perform(ctx->argument(7));
		sad::Maybe<_Arg8> value8 = scripting::ToValue<_Arg8>::perform(ctx->argument(8));
		sad::Maybe<_Arg9> value9 = scripting::ToValue<_Arg9>::perform(ctx->argument(9));
		return scripting::FromValue<_Constructed>::perform(_Constructed(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value(),
			value5.value(),
			value6.value(),
			value7.value(),
			value8.value(),
			value9.value()
		), engine);
	}
};

/*! A functor class, which represents constructable call with 11 arguments
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7,
	typename _Arg8,
	typename _Arg9,
	typename _Arg10
>
class ConstructorCall11: public Callable
{
public:
	/*! Represents a constructor call for a function with 11 arguments
	    \param[in] e engine
	    \param[in] name a name
	 */
	ConstructorCall11(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 11)
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ConstructorCall11()
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
		checkArgument<_Arg0>(result, 0, context);
		checkArgument<_Arg1>(result, 1, context);
		checkArgument<_Arg2>(result, 2, context);
		checkArgument<_Arg3>(result, 3, context);
		checkArgument<_Arg4>(result, 4, context);
		checkArgument<_Arg5>(result, 5, context);
		checkArgument<_Arg6>(result, 6, context);
		checkArgument<_Arg7>(result, 7, context);
		checkArgument<_Arg8>(result, 8, context);
		checkArgument<_Arg9>(result, 9, context);
		checkArgument<_Arg10>(result, 10, context);
		return result;
	}
	/*! Calls  an object
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_Arg0> value0 = scripting::ToValue<_Arg0>::perform(ctx->argument(0));
		sad::Maybe<_Arg1> value1 = scripting::ToValue<_Arg1>::perform(ctx->argument(1));
		sad::Maybe<_Arg2> value2 = scripting::ToValue<_Arg2>::perform(ctx->argument(2));
		sad::Maybe<_Arg3> value3 = scripting::ToValue<_Arg3>::perform(ctx->argument(3));
		sad::Maybe<_Arg4> value4 = scripting::ToValue<_Arg4>::perform(ctx->argument(4));
		sad::Maybe<_Arg5> value5 = scripting::ToValue<_Arg5>::perform(ctx->argument(5));
		sad::Maybe<_Arg6> value6 = scripting::ToValue<_Arg6>::perform(ctx->argument(6));
		sad::Maybe<_Arg7> value7 = scripting::ToValue<_Arg7>::perform(ctx->argument(7));
		sad::Maybe<_Arg8> value8 = scripting::ToValue<_Arg8>::perform(ctx->argument(8));
		sad::Maybe<_Arg9> value9 = scripting::ToValue<_Arg9>::perform(ctx->argument(9));
		sad::Maybe<_Arg10> value10 = scripting::ToValue<_Arg10>::perform(ctx->argument(10));
		return scripting::FromValue<_Constructed>::perform(_Constructed(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value(),
			value5.value(),
			value6.value(),
			value7.value(),
			value8.value(),
			value9.value(),
			value10.value()
		), engine);
	}
};

/*! A functor class, which represents constructable call with 12 arguments
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7,
	typename _Arg8,
	typename _Arg9,
	typename _Arg10,
	typename _Arg11
>
class ConstructorCall12: public Callable
{
public:
	/*! Represents a constructor call for a function with 12 arguments
	    \param[in] e engine
	    \param[in] name a name
	 */
	ConstructorCall12(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 12)
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ConstructorCall12()
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
		checkArgument<_Arg0>(result, 0, context);
		checkArgument<_Arg1>(result, 1, context);
		checkArgument<_Arg2>(result, 2, context);
		checkArgument<_Arg3>(result, 3, context);
		checkArgument<_Arg4>(result, 4, context);
		checkArgument<_Arg5>(result, 5, context);
		checkArgument<_Arg6>(result, 6, context);
		checkArgument<_Arg7>(result, 7, context);
		checkArgument<_Arg8>(result, 8, context);
		checkArgument<_Arg9>(result, 9, context);
		checkArgument<_Arg10>(result, 10, context);
		checkArgument<_Arg11>(result, 11, context);
		return result;
	}
	/*! Calls  an object
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_Arg0> value0 = scripting::ToValue<_Arg0>::perform(ctx->argument(0));
		sad::Maybe<_Arg1> value1 = scripting::ToValue<_Arg1>::perform(ctx->argument(1));
		sad::Maybe<_Arg2> value2 = scripting::ToValue<_Arg2>::perform(ctx->argument(2));
		sad::Maybe<_Arg3> value3 = scripting::ToValue<_Arg3>::perform(ctx->argument(3));
		sad::Maybe<_Arg4> value4 = scripting::ToValue<_Arg4>::perform(ctx->argument(4));
		sad::Maybe<_Arg5> value5 = scripting::ToValue<_Arg5>::perform(ctx->argument(5));
		sad::Maybe<_Arg6> value6 = scripting::ToValue<_Arg6>::perform(ctx->argument(6));
		sad::Maybe<_Arg7> value7 = scripting::ToValue<_Arg7>::perform(ctx->argument(7));
		sad::Maybe<_Arg8> value8 = scripting::ToValue<_Arg8>::perform(ctx->argument(8));
		sad::Maybe<_Arg9> value9 = scripting::ToValue<_Arg9>::perform(ctx->argument(9));
		sad::Maybe<_Arg10> value10 = scripting::ToValue<_Arg10>::perform(ctx->argument(10));
		sad::Maybe<_Arg11> value11 = scripting::ToValue<_Arg11>::perform(ctx->argument(11));
		return scripting::FromValue<_Constructed>::perform(_Constructed(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value(),
			value5.value(),
			value6.value(),
			value7.value(),
			value8.value(),
			value9.value(),
			value10.value(),
			value11.value()
		), engine);
	}
};

/*! A functor class, which represents constructable call with 13 arguments
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7,
	typename _Arg8,
	typename _Arg9,
	typename _Arg10,
	typename _Arg11,
	typename _Arg12
>
class ConstructorCall13: public Callable
{
public:
	/*! Represents a constructor call for a function with 13 arguments
	    \param[in] e engine
	    \param[in] name a name
	 */
	ConstructorCall13(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 13)
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ConstructorCall13()
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
		checkArgument<_Arg0>(result, 0, context);
		checkArgument<_Arg1>(result, 1, context);
		checkArgument<_Arg2>(result, 2, context);
		checkArgument<_Arg3>(result, 3, context);
		checkArgument<_Arg4>(result, 4, context);
		checkArgument<_Arg5>(result, 5, context);
		checkArgument<_Arg6>(result, 6, context);
		checkArgument<_Arg7>(result, 7, context);
		checkArgument<_Arg8>(result, 8, context);
		checkArgument<_Arg9>(result, 9, context);
		checkArgument<_Arg10>(result, 10, context);
		checkArgument<_Arg11>(result, 11, context);
		checkArgument<_Arg12>(result, 12, context);
		return result;
	}
	/*! Calls  an object
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_Arg0> value0 = scripting::ToValue<_Arg0>::perform(ctx->argument(0));
		sad::Maybe<_Arg1> value1 = scripting::ToValue<_Arg1>::perform(ctx->argument(1));
		sad::Maybe<_Arg2> value2 = scripting::ToValue<_Arg2>::perform(ctx->argument(2));
		sad::Maybe<_Arg3> value3 = scripting::ToValue<_Arg3>::perform(ctx->argument(3));
		sad::Maybe<_Arg4> value4 = scripting::ToValue<_Arg4>::perform(ctx->argument(4));
		sad::Maybe<_Arg5> value5 = scripting::ToValue<_Arg5>::perform(ctx->argument(5));
		sad::Maybe<_Arg6> value6 = scripting::ToValue<_Arg6>::perform(ctx->argument(6));
		sad::Maybe<_Arg7> value7 = scripting::ToValue<_Arg7>::perform(ctx->argument(7));
		sad::Maybe<_Arg8> value8 = scripting::ToValue<_Arg8>::perform(ctx->argument(8));
		sad::Maybe<_Arg9> value9 = scripting::ToValue<_Arg9>::perform(ctx->argument(9));
		sad::Maybe<_Arg10> value10 = scripting::ToValue<_Arg10>::perform(ctx->argument(10));
		sad::Maybe<_Arg11> value11 = scripting::ToValue<_Arg11>::perform(ctx->argument(11));
		sad::Maybe<_Arg12> value12 = scripting::ToValue<_Arg12>::perform(ctx->argument(12));
		return scripting::FromValue<_Constructed>::perform(_Constructed(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value(),
			value5.value(),
			value6.value(),
			value7.value(),
			value8.value(),
			value9.value(),
			value10.value(),
			value11.value(),
			value12.value()
		), engine);
	}
};

/*! A functor class, which represents constructable call with 14 arguments
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7,
	typename _Arg8,
	typename _Arg9,
	typename _Arg10,
	typename _Arg11,
	typename _Arg12,
	typename _Arg13
>
class ConstructorCall14: public Callable
{
public:
	/*! Represents a constructor call for a function with 14 arguments
	    \param[in] e engine
	    \param[in] name a name
	 */
	ConstructorCall14(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 14)
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ConstructorCall14()
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
		checkArgument<_Arg0>(result, 0, context);
		checkArgument<_Arg1>(result, 1, context);
		checkArgument<_Arg2>(result, 2, context);
		checkArgument<_Arg3>(result, 3, context);
		checkArgument<_Arg4>(result, 4, context);
		checkArgument<_Arg5>(result, 5, context);
		checkArgument<_Arg6>(result, 6, context);
		checkArgument<_Arg7>(result, 7, context);
		checkArgument<_Arg8>(result, 8, context);
		checkArgument<_Arg9>(result, 9, context);
		checkArgument<_Arg10>(result, 10, context);
		checkArgument<_Arg11>(result, 11, context);
		checkArgument<_Arg12>(result, 12, context);
		checkArgument<_Arg13>(result, 13, context);
		return result;
	}
	/*! Calls  an object
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_Arg0> value0 = scripting::ToValue<_Arg0>::perform(ctx->argument(0));
		sad::Maybe<_Arg1> value1 = scripting::ToValue<_Arg1>::perform(ctx->argument(1));
		sad::Maybe<_Arg2> value2 = scripting::ToValue<_Arg2>::perform(ctx->argument(2));
		sad::Maybe<_Arg3> value3 = scripting::ToValue<_Arg3>::perform(ctx->argument(3));
		sad::Maybe<_Arg4> value4 = scripting::ToValue<_Arg4>::perform(ctx->argument(4));
		sad::Maybe<_Arg5> value5 = scripting::ToValue<_Arg5>::perform(ctx->argument(5));
		sad::Maybe<_Arg6> value6 = scripting::ToValue<_Arg6>::perform(ctx->argument(6));
		sad::Maybe<_Arg7> value7 = scripting::ToValue<_Arg7>::perform(ctx->argument(7));
		sad::Maybe<_Arg8> value8 = scripting::ToValue<_Arg8>::perform(ctx->argument(8));
		sad::Maybe<_Arg9> value9 = scripting::ToValue<_Arg9>::perform(ctx->argument(9));
		sad::Maybe<_Arg10> value10 = scripting::ToValue<_Arg10>::perform(ctx->argument(10));
		sad::Maybe<_Arg11> value11 = scripting::ToValue<_Arg11>::perform(ctx->argument(11));
		sad::Maybe<_Arg12> value12 = scripting::ToValue<_Arg12>::perform(ctx->argument(12));
		sad::Maybe<_Arg13> value13 = scripting::ToValue<_Arg13>::perform(ctx->argument(13));
		return scripting::FromValue<_Constructed>::perform(_Constructed(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value(),
			value5.value(),
			value6.value(),
			value7.value(),
			value8.value(),
			value9.value(),
			value10.value(),
			value11.value(),
			value12.value(),
			value13.value()
		), engine);
	}
};

/*! A functor class, which represents constructable call with 15 arguments
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7,
	typename _Arg8,
	typename _Arg9,
	typename _Arg10,
	typename _Arg11,
	typename _Arg12,
	typename _Arg13,
	typename _Arg14
>
class ConstructorCall15: public Callable
{
public:
	/*! Represents a constructor call for a function with 15 arguments
	    \param[in] e engine
	    \param[in] name a name
	 */
	ConstructorCall15(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 15)
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ConstructorCall15()
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
		checkArgument<_Arg0>(result, 0, context);
		checkArgument<_Arg1>(result, 1, context);
		checkArgument<_Arg2>(result, 2, context);
		checkArgument<_Arg3>(result, 3, context);
		checkArgument<_Arg4>(result, 4, context);
		checkArgument<_Arg5>(result, 5, context);
		checkArgument<_Arg6>(result, 6, context);
		checkArgument<_Arg7>(result, 7, context);
		checkArgument<_Arg8>(result, 8, context);
		checkArgument<_Arg9>(result, 9, context);
		checkArgument<_Arg10>(result, 10, context);
		checkArgument<_Arg11>(result, 11, context);
		checkArgument<_Arg12>(result, 12, context);
		checkArgument<_Arg13>(result, 13, context);
		checkArgument<_Arg14>(result, 14, context);
		return result;
	}
	/*! Calls  an object
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_Arg0> value0 = scripting::ToValue<_Arg0>::perform(ctx->argument(0));
		sad::Maybe<_Arg1> value1 = scripting::ToValue<_Arg1>::perform(ctx->argument(1));
		sad::Maybe<_Arg2> value2 = scripting::ToValue<_Arg2>::perform(ctx->argument(2));
		sad::Maybe<_Arg3> value3 = scripting::ToValue<_Arg3>::perform(ctx->argument(3));
		sad::Maybe<_Arg4> value4 = scripting::ToValue<_Arg4>::perform(ctx->argument(4));
		sad::Maybe<_Arg5> value5 = scripting::ToValue<_Arg5>::perform(ctx->argument(5));
		sad::Maybe<_Arg6> value6 = scripting::ToValue<_Arg6>::perform(ctx->argument(6));
		sad::Maybe<_Arg7> value7 = scripting::ToValue<_Arg7>::perform(ctx->argument(7));
		sad::Maybe<_Arg8> value8 = scripting::ToValue<_Arg8>::perform(ctx->argument(8));
		sad::Maybe<_Arg9> value9 = scripting::ToValue<_Arg9>::perform(ctx->argument(9));
		sad::Maybe<_Arg10> value10 = scripting::ToValue<_Arg10>::perform(ctx->argument(10));
		sad::Maybe<_Arg11> value11 = scripting::ToValue<_Arg11>::perform(ctx->argument(11));
		sad::Maybe<_Arg12> value12 = scripting::ToValue<_Arg12>::perform(ctx->argument(12));
		sad::Maybe<_Arg13> value13 = scripting::ToValue<_Arg13>::perform(ctx->argument(13));
		sad::Maybe<_Arg14> value14 = scripting::ToValue<_Arg14>::perform(ctx->argument(14));
		return scripting::FromValue<_Constructed>::perform(_Constructed(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value(),
			value5.value(),
			value6.value(),
			value7.value(),
			value8.value(),
			value9.value(),
			value10.value(),
			value11.value(),
			value12.value(),
			value13.value(),
			value14.value()
		), engine);
	}
};

/*! A functor class, which represents constructable call with 16 arguments
 */
template<
	typename _Constructed,
	typename _Arg0,
	typename _Arg1,
	typename _Arg2,
	typename _Arg3,
	typename _Arg4,
	typename _Arg5,
	typename _Arg6,
	typename _Arg7,
	typename _Arg8,
	typename _Arg9,
	typename _Arg10,
	typename _Arg11,
	typename _Arg12,
	typename _Arg13,
	typename _Arg14,
	typename _Arg15
>
class ConstructorCall16: public Callable
{
public:
	/*! Represents a constructor call for a function with 16 arguments
	    \param[in] e engine
	    \param[in] name a name
	 */
	ConstructorCall16(QScriptEngine* e, const QString& name) : scripting::Callable(e, name, 16)
	{
		
	}
	/*! Can be inherited
	 */
	virtual ~ConstructorCall16()
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
		checkArgument<_Arg0>(result, 0, context);
		checkArgument<_Arg1>(result, 1, context);
		checkArgument<_Arg2>(result, 2, context);
		checkArgument<_Arg3>(result, 3, context);
		checkArgument<_Arg4>(result, 4, context);
		checkArgument<_Arg5>(result, 5, context);
		checkArgument<_Arg6>(result, 6, context);
		checkArgument<_Arg7>(result, 7, context);
		checkArgument<_Arg8>(result, 8, context);
		checkArgument<_Arg9>(result, 9, context);
		checkArgument<_Arg10>(result, 10, context);
		checkArgument<_Arg11>(result, 11, context);
		checkArgument<_Arg12>(result, 12, context);
		checkArgument<_Arg13>(result, 13, context);
		checkArgument<_Arg14>(result, 14, context);
		checkArgument<_Arg15>(result, 15, context);
		return result;
	}
	/*! Calls  an object
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_Arg0> value0 = scripting::ToValue<_Arg0>::perform(ctx->argument(0));
		sad::Maybe<_Arg1> value1 = scripting::ToValue<_Arg1>::perform(ctx->argument(1));
		sad::Maybe<_Arg2> value2 = scripting::ToValue<_Arg2>::perform(ctx->argument(2));
		sad::Maybe<_Arg3> value3 = scripting::ToValue<_Arg3>::perform(ctx->argument(3));
		sad::Maybe<_Arg4> value4 = scripting::ToValue<_Arg4>::perform(ctx->argument(4));
		sad::Maybe<_Arg5> value5 = scripting::ToValue<_Arg5>::perform(ctx->argument(5));
		sad::Maybe<_Arg6> value6 = scripting::ToValue<_Arg6>::perform(ctx->argument(6));
		sad::Maybe<_Arg7> value7 = scripting::ToValue<_Arg7>::perform(ctx->argument(7));
		sad::Maybe<_Arg8> value8 = scripting::ToValue<_Arg8>::perform(ctx->argument(8));
		sad::Maybe<_Arg9> value9 = scripting::ToValue<_Arg9>::perform(ctx->argument(9));
		sad::Maybe<_Arg10> value10 = scripting::ToValue<_Arg10>::perform(ctx->argument(10));
		sad::Maybe<_Arg11> value11 = scripting::ToValue<_Arg11>::perform(ctx->argument(11));
		sad::Maybe<_Arg12> value12 = scripting::ToValue<_Arg12>::perform(ctx->argument(12));
		sad::Maybe<_Arg13> value13 = scripting::ToValue<_Arg13>::perform(ctx->argument(13));
		sad::Maybe<_Arg14> value14 = scripting::ToValue<_Arg14>::perform(ctx->argument(14));
		sad::Maybe<_Arg15> value15 = scripting::ToValue<_Arg15>::perform(ctx->argument(15));
		return scripting::FromValue<_Constructed>::perform(_Constructed(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value(),
			value5.value(),
			value6.value(),
			value7.value(),
			value8.value(),
			value9.value(),
			value10.value(),
			value11.value(),
			value12.value(),
			value13.value(),
			value14.value(),
			value15.value()
		), engine);
	}
};

}
