/*! \file functioncall.h
	\author HiddenSeeker

	Describes a call from function data
 */
#pragma once
#include "callable.h"

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
/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
class WithArgs0: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)();

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs0(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 0), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs0()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		return result;
	}

	/*! Calls  a function
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext*, QScriptEngine* engine)
	{
		return scripting::FromValue<_ReturnType>::perform(m_f(
		), engine);
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0
>
class WithArgs1: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)(_ArgType0);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs1(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 1), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs1()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		return scripting::FromValue<_ReturnType>::perform(m_f(
			value0.value()
		), engine);
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1
>
class WithArgs2: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)(_ArgType0, _ArgType1);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs2(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 2), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs2()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		return scripting::FromValue<_ReturnType>::perform(m_f(
			value0.value(),
			value1.value()
		), engine);
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2
>
class WithArgs3: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)(_ArgType0, _ArgType1, _ArgType2);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs3(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 3), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs3()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		return scripting::FromValue<_ReturnType>::perform(m_f(
			value0.value(),
			value1.value(),
			value2.value()
		), engine);
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3
>
class WithArgs4: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs4(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 4), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs4()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		return scripting::FromValue<_ReturnType>::perform(m_f(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value()
		), engine);
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4
>
class WithArgs5: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs5(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 5), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs5()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		return scripting::FromValue<_ReturnType>::perform(m_f(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value()
		), engine);
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5
>
class WithArgs6: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs6(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 6), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs6()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		return scripting::FromValue<_ReturnType>::perform(m_f(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value(),
			value5.value()
		), engine);
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6
>
class WithArgs7: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs7(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 7), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs7()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		return scripting::FromValue<_ReturnType>::perform(m_f(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value(),
			value5.value(),
			value6.value()
		), engine);
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7
>
class WithArgs8: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs8(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 8), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs8()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		return scripting::FromValue<_ReturnType>::perform(m_f(
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
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8
>
class WithArgs9: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs9(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 9), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs9()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		checkArgument<_ArgType8>(result, 8, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		sad::Maybe<_ArgType8> value8 = scripting::ToValue<_ArgType8>::perform(ctx->argument(8));
		return scripting::FromValue<_ReturnType>::perform(m_f(
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
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */

template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9
>
class WithArgs10: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs10(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 10), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs10()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		checkArgument<_ArgType8>(result, 8, context);
		checkArgument<_ArgType9>(result, 9, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		sad::Maybe<_ArgType8> value8 = scripting::ToValue<_ArgType8>::perform(ctx->argument(8));
		sad::Maybe<_ArgType9> value9 = scripting::ToValue<_ArgType9>::perform(ctx->argument(9));
		return scripting::FromValue<_ReturnType>::perform(m_f(
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
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10
>
class WithArgs11: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs11(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 11), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs11()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		checkArgument<_ArgType8>(result, 8, context);
		checkArgument<_ArgType9>(result, 9, context);
		checkArgument<_ArgType10>(result, 10, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		sad::Maybe<_ArgType8> value8 = scripting::ToValue<_ArgType8>::perform(ctx->argument(8));
		sad::Maybe<_ArgType9> value9 = scripting::ToValue<_ArgType9>::perform(ctx->argument(9));
		sad::Maybe<_ArgType10> value10 = scripting::ToValue<_ArgType10>::perform(ctx->argument(10));
		return scripting::FromValue<_ReturnType>::perform(m_f(
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
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10,
	typename _ArgType11
>
class WithArgs12: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs12(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 12), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs12()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		checkArgument<_ArgType8>(result, 8, context);
		checkArgument<_ArgType9>(result, 9, context);
		checkArgument<_ArgType10>(result, 10, context);
		checkArgument<_ArgType11>(result, 11, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		sad::Maybe<_ArgType8> value8 = scripting::ToValue<_ArgType8>::perform(ctx->argument(8));
		sad::Maybe<_ArgType9> value9 = scripting::ToValue<_ArgType9>::perform(ctx->argument(9));
		sad::Maybe<_ArgType10> value10 = scripting::ToValue<_ArgType10>::perform(ctx->argument(10));
		sad::Maybe<_ArgType11> value11 = scripting::ToValue<_ArgType11>::perform(ctx->argument(11));
		return scripting::FromValue<_ReturnType>::perform(m_f(
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
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10,
	typename _ArgType11,
	typename _ArgType12
>
class WithArgs13: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11, _ArgType12);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs13(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 13), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs13()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		checkArgument<_ArgType8>(result, 8, context);
		checkArgument<_ArgType9>(result, 9, context);
		checkArgument<_ArgType10>(result, 10, context);
		checkArgument<_ArgType11>(result, 11, context);
		checkArgument<_ArgType12>(result, 12, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		sad::Maybe<_ArgType8> value8 = scripting::ToValue<_ArgType8>::perform(ctx->argument(8));
		sad::Maybe<_ArgType9> value9 = scripting::ToValue<_ArgType9>::perform(ctx->argument(9));
		sad::Maybe<_ArgType10> value10 = scripting::ToValue<_ArgType10>::perform(ctx->argument(10));
		sad::Maybe<_ArgType11> value11 = scripting::ToValue<_ArgType11>::perform(ctx->argument(11));
		sad::Maybe<_ArgType12> value12 = scripting::ToValue<_ArgType12>::perform(ctx->argument(12));
		return scripting::FromValue<_ReturnType>::perform(m_f(
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
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10,
	typename _ArgType11,
	typename _ArgType12,
	typename _ArgType13
>
class WithArgs14: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11, _ArgType12, _ArgType13);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs14(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 14), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs14()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		checkArgument<_ArgType8>(result, 8, context);
		checkArgument<_ArgType9>(result, 9, context);
		checkArgument<_ArgType10>(result, 10, context);
		checkArgument<_ArgType11>(result, 11, context);
		checkArgument<_ArgType12>(result, 12, context);
		checkArgument<_ArgType13>(result, 13, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		sad::Maybe<_ArgType8> value8 = scripting::ToValue<_ArgType8>::perform(ctx->argument(8));
		sad::Maybe<_ArgType9> value9 = scripting::ToValue<_ArgType9>::perform(ctx->argument(9));
		sad::Maybe<_ArgType10> value10 = scripting::ToValue<_ArgType10>::perform(ctx->argument(10));
		sad::Maybe<_ArgType11> value11 = scripting::ToValue<_ArgType11>::perform(ctx->argument(11));
		sad::Maybe<_ArgType12> value12 = scripting::ToValue<_ArgType12>::perform(ctx->argument(12));
		sad::Maybe<_ArgType13> value13 = scripting::ToValue<_ArgType13>::perform(ctx->argument(13));
		return scripting::FromValue<_ReturnType>::perform(m_f(
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
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10,
	typename _ArgType11,
	typename _ArgType12,
	typename _ArgType13,
	typename _ArgType14
>
class WithArgs15: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11, _ArgType12, _ArgType13, _ArgType14);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs15(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 15), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs15()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		checkArgument<_ArgType8>(result, 8, context);
		checkArgument<_ArgType9>(result, 9, context);
		checkArgument<_ArgType10>(result, 10, context);
		checkArgument<_ArgType11>(result, 11, context);
		checkArgument<_ArgType12>(result, 12, context);
		checkArgument<_ArgType13>(result, 13, context);
		checkArgument<_ArgType14>(result, 14, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		sad::Maybe<_ArgType8> value8 = scripting::ToValue<_ArgType8>::perform(ctx->argument(8));
		sad::Maybe<_ArgType9> value9 = scripting::ToValue<_ArgType9>::perform(ctx->argument(9));
		sad::Maybe<_ArgType10> value10 = scripting::ToValue<_ArgType10>::perform(ctx->argument(10));
		sad::Maybe<_ArgType11> value11 = scripting::ToValue<_ArgType11>::perform(ctx->argument(11));
		sad::Maybe<_ArgType12> value12 = scripting::ToValue<_ArgType12>::perform(ctx->argument(12));
		sad::Maybe<_ArgType13> value13 = scripting::ToValue<_ArgType13>::perform(ctx->argument(13));
		sad::Maybe<_ArgType14> value14 = scripting::ToValue<_ArgType14>::perform(ctx->argument(14));
		return scripting::FromValue<_ReturnType>::perform(m_f(
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
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10,
	typename _ArgType11,
	typename _ArgType12,
	typename _ArgType13,
	typename _ArgType14,
	typename _ArgType15
>
class WithArgs16: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef _ReturnType (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11, _ArgType12, _ArgType13, _ArgType14, _ArgType15);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs16(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 16), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs16()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		checkArgument<_ArgType8>(result, 8, context);
		checkArgument<_ArgType9>(result, 9, context);
		checkArgument<_ArgType10>(result, 10, context);
		checkArgument<_ArgType11>(result, 11, context);
		checkArgument<_ArgType12>(result, 12, context);
		checkArgument<_ArgType13>(result, 13, context);
		checkArgument<_ArgType14>(result, 14, context);
		checkArgument<_ArgType15>(result, 15, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine* engine)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		sad::Maybe<_ArgType8> value8 = scripting::ToValue<_ArgType8>::perform(ctx->argument(8));
		sad::Maybe<_ArgType9> value9 = scripting::ToValue<_ArgType9>::perform(ctx->argument(9));
		sad::Maybe<_ArgType10> value10 = scripting::ToValue<_ArgType10>::perform(ctx->argument(10));
		sad::Maybe<_ArgType11> value11 = scripting::ToValue<_ArgType11>::perform(ctx->argument(11));
		sad::Maybe<_ArgType12> value12 = scripting::ToValue<_ArgType12>::perform(ctx->argument(12));
		sad::Maybe<_ArgType13> value13 = scripting::ToValue<_ArgType13>::perform(ctx->argument(13));
		sad::Maybe<_ArgType14> value14 = scripting::ToValue<_ArgType14>::perform(ctx->argument(14));
		sad::Maybe<_ArgType15> value15 = scripting::ToValue<_ArgType15>::perform(ctx->argument(15));
		return scripting::FromValue<_ReturnType>::perform(m_f(
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
protected:
	Signature m_f;
};

};

/*! A function call for calling specific simple function with void signature
 */
template<
>
class FunctionCall<void>
{
public:
/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
class WithArgs0: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)();

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs0(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 0), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs0()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		m_f(
		);
		return ctx->thisObject();
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0
>
class WithArgs1: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)(_ArgType0);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs1(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 1), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs1()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		m_f(
			value0.value()
		);
		return ctx->thisObject();
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1
>
class WithArgs2: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)(_ArgType0, _ArgType1);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs2(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 2), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs2()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		m_f(
			value0.value(),
			value1.value()
		);
		return ctx->thisObject();
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2
>
class WithArgs3: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)(_ArgType0, _ArgType1, _ArgType2);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs3(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 3), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs3()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		m_f(
			value0.value(),
			value1.value(),
			value2.value()
		);
		return ctx->thisObject();
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3
>
class WithArgs4: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs4(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 4), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs4()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		m_f(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value()
		);
		return ctx->thisObject();
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4
>
class WithArgs5: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs5(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 5), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs5()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		m_f(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value()
		);
		return ctx->thisObject();
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5
>
class WithArgs6: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs6(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 6), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs6()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		m_f(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value(),
			value5.value()
		);
		return ctx->thisObject();
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6
>
class WithArgs7: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs7(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 7), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs7()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		m_f(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value(),
			value5.value(),
			value6.value()
		);
		return ctx->thisObject();
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7
>
class WithArgs8: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs8(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 8), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs8()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		m_f(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value(),
			value5.value(),
			value6.value(),
			value7.value()
		);
		return ctx->thisObject();
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8
>
class WithArgs9: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs9(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 9), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs9()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		checkArgument<_ArgType8>(result, 8, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		sad::Maybe<_ArgType8> value8 = scripting::ToValue<_ArgType8>::perform(ctx->argument(8));
		m_f(
			value0.value(),
			value1.value(),
			value2.value(),
			value3.value(),
			value4.value(),
			value5.value(),
			value6.value(),
			value7.value(),
			value8.value()
		);
		return ctx->thisObject();
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9
>
class WithArgs10: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs10(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 10), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs10()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		checkArgument<_ArgType8>(result, 8, context);
		checkArgument<_ArgType9>(result, 9, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		sad::Maybe<_ArgType8> value8 = scripting::ToValue<_ArgType8>::perform(ctx->argument(8));
		sad::Maybe<_ArgType9> value9 = scripting::ToValue<_ArgType9>::perform(ctx->argument(9));
		m_f(
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
		);
		return ctx->thisObject();
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10
>
class WithArgs11: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs11(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 11), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs11()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		checkArgument<_ArgType8>(result, 8, context);
		checkArgument<_ArgType9>(result, 9, context);
		checkArgument<_ArgType10>(result, 10, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		sad::Maybe<_ArgType8> value8 = scripting::ToValue<_ArgType8>::perform(ctx->argument(8));
		sad::Maybe<_ArgType9> value9 = scripting::ToValue<_ArgType9>::perform(ctx->argument(9));
		sad::Maybe<_ArgType10> value10 = scripting::ToValue<_ArgType10>::perform(ctx->argument(10));
		m_f(
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
		);
		return ctx->thisObject();
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10,
	typename _ArgType11
>
class WithArgs12: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs12(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 12), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs12()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		checkArgument<_ArgType8>(result, 8, context);
		checkArgument<_ArgType9>(result, 9, context);
		checkArgument<_ArgType10>(result, 10, context);
		checkArgument<_ArgType11>(result, 11, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		sad::Maybe<_ArgType8> value8 = scripting::ToValue<_ArgType8>::perform(ctx->argument(8));
		sad::Maybe<_ArgType9> value9 = scripting::ToValue<_ArgType9>::perform(ctx->argument(9));
		sad::Maybe<_ArgType10> value10 = scripting::ToValue<_ArgType10>::perform(ctx->argument(10));
		sad::Maybe<_ArgType11> value11 = scripting::ToValue<_ArgType11>::perform(ctx->argument(11));
		m_f(
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
		);
		return ctx->thisObject();
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10,
	typename _ArgType11,
	typename _ArgType12
>
class WithArgs13: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11, _ArgType12);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs13(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 13), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs13()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		checkArgument<_ArgType8>(result, 8, context);
		checkArgument<_ArgType9>(result, 9, context);
		checkArgument<_ArgType10>(result, 10, context);
		checkArgument<_ArgType11>(result, 11, context);
		checkArgument<_ArgType12>(result, 12, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		sad::Maybe<_ArgType8> value8 = scripting::ToValue<_ArgType8>::perform(ctx->argument(8));
		sad::Maybe<_ArgType9> value9 = scripting::ToValue<_ArgType9>::perform(ctx->argument(9));
		sad::Maybe<_ArgType10> value10 = scripting::ToValue<_ArgType10>::perform(ctx->argument(10));
		sad::Maybe<_ArgType11> value11 = scripting::ToValue<_ArgType11>::perform(ctx->argument(11));
		sad::Maybe<_ArgType12> value12 = scripting::ToValue<_ArgType12>::perform(ctx->argument(12));
		m_f(
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
		);
		return ctx->thisObject();
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10,
	typename _ArgType11,
	typename _ArgType12,
	typename _ArgType13
>
class WithArgs14: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11, _ArgType12, _ArgType13);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs14(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 14), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs14()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		checkArgument<_ArgType8>(result, 8, context);
		checkArgument<_ArgType9>(result, 9, context);
		checkArgument<_ArgType10>(result, 10, context);
		checkArgument<_ArgType11>(result, 11, context);
		checkArgument<_ArgType12>(result, 12, context);
		checkArgument<_ArgType13>(result, 13, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		sad::Maybe<_ArgType8> value8 = scripting::ToValue<_ArgType8>::perform(ctx->argument(8));
		sad::Maybe<_ArgType9> value9 = scripting::ToValue<_ArgType9>::perform(ctx->argument(9));
		sad::Maybe<_ArgType10> value10 = scripting::ToValue<_ArgType10>::perform(ctx->argument(10));
		sad::Maybe<_ArgType11> value11 = scripting::ToValue<_ArgType11>::perform(ctx->argument(11));
		sad::Maybe<_ArgType12> value12 = scripting::ToValue<_ArgType12>::perform(ctx->argument(12));
		sad::Maybe<_ArgType13> value13 = scripting::ToValue<_ArgType13>::perform(ctx->argument(13));
		m_f(
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
		);
		return ctx->thisObject();
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10,
	typename _ArgType11,
	typename _ArgType12,
	typename _ArgType13,
	typename _ArgType14
>
class WithArgs15: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11, _ArgType12, _ArgType13, _ArgType14);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs15(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 15), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs15()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		checkArgument<_ArgType8>(result, 8, context);
		checkArgument<_ArgType9>(result, 9, context);
		checkArgument<_ArgType10>(result, 10, context);
		checkArgument<_ArgType11>(result, 11, context);
		checkArgument<_ArgType12>(result, 12, context);
		checkArgument<_ArgType13>(result, 13, context);
		checkArgument<_ArgType14>(result, 14, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		sad::Maybe<_ArgType8> value8 = scripting::ToValue<_ArgType8>::perform(ctx->argument(8));
		sad::Maybe<_ArgType9> value9 = scripting::ToValue<_ArgType9>::perform(ctx->argument(9));
		sad::Maybe<_ArgType10> value10 = scripting::ToValue<_ArgType10>::perform(ctx->argument(10));
		sad::Maybe<_ArgType11> value11 = scripting::ToValue<_ArgType11>::perform(ctx->argument(11));
		sad::Maybe<_ArgType12> value12 = scripting::ToValue<_ArgType12>::perform(ctx->argument(12));
		sad::Maybe<_ArgType13> value13 = scripting::ToValue<_ArgType13>::perform(ctx->argument(13));
		sad::Maybe<_ArgType14> value14 = scripting::ToValue<_ArgType14>::perform(ctx->argument(14));
		m_f(
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
		);
		return ctx->thisObject();
	}
protected:
	Signature m_f;
};

/*! Registers a functional call
	\param[in] name a name of object
	\param[in] s scripting object
 */
template<
	typename _ArgType0,
	typename _ArgType1,
	typename _ArgType2,
	typename _ArgType3,
	typename _ArgType4,
	typename _ArgType5,
	typename _ArgType6,
	typename _ArgType7,
	typename _ArgType8,
	typename _ArgType9,
	typename _ArgType10,
	typename _ArgType11,
	typename _ArgType12,
	typename _ArgType13,
	typename _ArgType14,
	typename _ArgType15
>
class WithArgs16: public scripting:: Callable
{
public:
	/*! A signature type for creating objects
	 */
	typedef void (*Signature)(_ArgType0, _ArgType1, _ArgType2, _ArgType3, _ArgType4, _ArgType5, _ArgType6, _ArgType7, _ArgType8, _ArgType9, _ArgType10, _ArgType11, _ArgType12, _ArgType13, _ArgType14, _ArgType15);

    /*! Represents a functional call for a function with 2 arguments
	    \param[in] e engine
	    \param[in] name a name
		\param[in] s function
	 */
	WithArgs16(QScriptEngine* e, const QString& name, Signature s) : scripting::Callable(e, name, 16), m_f(s)
	{
		
	}

	/*! Can be inherited
	 */
	virtual ~WithArgs16()
	{
	
	}

	/*! Whether function can be called
	    \param[in] context a context element
	    \return whether it has an error
	 */
	sad::Maybe<QString> canBeCalled(QScriptContext* context)
	{
	    sad::Maybe<QString> result;
		checkArgumentCount(result, context);
		checkArgument<_ArgType0>(result, 0, context);
		checkArgument<_ArgType1>(result, 1, context);
		checkArgument<_ArgType2>(result, 2, context);
		checkArgument<_ArgType3>(result, 3, context);
		checkArgument<_ArgType4>(result, 4, context);
		checkArgument<_ArgType5>(result, 5, context);
		checkArgument<_ArgType6>(result, 6, context);
		checkArgument<_ArgType7>(result, 7, context);
		checkArgument<_ArgType8>(result, 8, context);
		checkArgument<_ArgType9>(result, 9, context);
		checkArgument<_ArgType10>(result, 10, context);
		checkArgument<_ArgType11>(result, 11, context);
		checkArgument<_ArgType12>(result, 12, context);
		checkArgument<_ArgType13>(result, 13, context);
		checkArgument<_ArgType14>(result, 14, context);
		checkArgument<_ArgType15>(result, 15, context);
		return result;
	}

	/*! Calls  a function
		\param[in] ctx context
	 */
	QScriptValue call(QScriptContext* ctx, QScriptEngine*)
	{
		sad::Maybe<_ArgType0> value0 = scripting::ToValue<_ArgType0>::perform(ctx->argument(0));
		sad::Maybe<_ArgType1> value1 = scripting::ToValue<_ArgType1>::perform(ctx->argument(1));
		sad::Maybe<_ArgType2> value2 = scripting::ToValue<_ArgType2>::perform(ctx->argument(2));
		sad::Maybe<_ArgType3> value3 = scripting::ToValue<_ArgType3>::perform(ctx->argument(3));
		sad::Maybe<_ArgType4> value4 = scripting::ToValue<_ArgType4>::perform(ctx->argument(4));
		sad::Maybe<_ArgType5> value5 = scripting::ToValue<_ArgType5>::perform(ctx->argument(5));
		sad::Maybe<_ArgType6> value6 = scripting::ToValue<_ArgType6>::perform(ctx->argument(6));
		sad::Maybe<_ArgType7> value7 = scripting::ToValue<_ArgType7>::perform(ctx->argument(7));
		sad::Maybe<_ArgType8> value8 = scripting::ToValue<_ArgType8>::perform(ctx->argument(8));
		sad::Maybe<_ArgType9> value9 = scripting::ToValue<_ArgType9>::perform(ctx->argument(9));
		sad::Maybe<_ArgType10> value10 = scripting::ToValue<_ArgType10>::perform(ctx->argument(10));
		sad::Maybe<_ArgType11> value11 = scripting::ToValue<_ArgType11>::perform(ctx->argument(11));
		sad::Maybe<_ArgType12> value12 = scripting::ToValue<_ArgType12>::perform(ctx->argument(12));
		sad::Maybe<_ArgType13> value13 = scripting::ToValue<_ArgType13>::perform(ctx->argument(13));
		sad::Maybe<_ArgType14> value14 = scripting::ToValue<_ArgType14>::perform(ctx->argument(14));
		sad::Maybe<_ArgType15> value15 = scripting::ToValue<_ArgType15>::perform(ctx->argument(15));
		m_f(
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
		);
		return ctx->thisObject();
	}
protected:
	Signature m_f;
};

};


}
