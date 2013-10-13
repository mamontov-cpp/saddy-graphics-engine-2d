/*! \file sadthreadexecutablefunction.h
	\author HiddenSeeker

	Defines a simple function, which can be executed in thread
 */
#pragma once

namespace sad
{

/*! A basic interface, which can be executed in thread
 */
class AbsractThreadExecutableFunction
{
public:
	/*! Executes a function in thread
		\return code, which will be returned in thread
	 */
	virtual int execute() = 0;
	/*! Cretes a clone of executable function
		\returns exact copy of current thread executable function
	 */
	virtual AbsractThreadExecutableFunction * clone() const = 0;
	/*! Kept for purpose of inheritance
	 */
	virtual ~AbsractThreadExecutableFunction();
};

/*! Defines an empty code, which only returns zero on execution
 */
class EmptyThreadExecutableFunction
:public sad::AbsractThreadExecutableFunction 
{
public:
	/*! Returns zero
		\return 0
	 */
	virtual int execute();
	/*! Cretes a clone of executable function
		\returns exact copy of current thread executable function
	 */
	virtual AbsractThreadExecutableFunction * clone() const;
};

/*! Executes a function with zero arguments and returns zero
 */
class FreeZeroArgVoidExecutableFunction
:public sad::AbsractThreadExecutableFunction
{
public:
	/*! Creates a function
		\param[in] f function, which will be executed
	 */
	inline FreeZeroArgVoidExecutableFunction(void (*f)()) : m_f(f)
	{
	}
	/*! Executes code  and returns zero
		\return 0
	 */
	virtual int execute();
	/*! Cretes a clone of executable function
		\returns exact copy of current thread executable function
	 */
	virtual AbsractThreadExecutableFunction * clone() const;
protected:
	void (*m_f)();
};

/*! Executes a function with zero arguments and returns code
 */
class FreeZeroArgIntExecutableFunction
:public sad::AbsractThreadExecutableFunction
{
public:
	/*! Creates a function
		\param[in] f function, which will be executed
	 */
	inline FreeZeroArgIntExecutableFunction(int (*f)()) : m_f(f)
	{
	}
	/*! Executes code  and returns returned function
		\return 0
	 */
	virtual int execute();
	/*! Cretes a clone of executable function
		\returns exact copy of current thread executable function
	 */
	virtual AbsractThreadExecutableFunction * clone() const;
protected:
	int (*m_f)();
};

/*! Executes a function with one argument and returns zero
 */
template<
	typename _FunctionArg,
	typename _CalledArg
>
class FreeOneArgVoidExecutableFunction
:public sad::AbsractThreadExecutableFunction
{
public:
	/*! Creates a function
		\param[in] f function, which will be executed
	 */
	inline FreeOneArgVoidExecutableFunction(
		void (*f)(_FunctionArg),
		const _CalledArg & a
		) : m_f(f), m_arg(a)
	{
	}
	/*! Executes code  and returns zero
		\return 0
	 */
	virtual int execute()
	{
		m_f(m_arg);
		return 0;
	}
	/*! Cretes a clone of executable function
		\returns exact copy of current thread executable function
	 */
	virtual AbsractThreadExecutableFunction * clone() const
	{
		return new sad::FreeOneArgVoidExecutableFunction<_FunctionArg, _CalledArg>(*this);
	}
protected:
	void (*m_f)(_FunctionArg);
	_CalledArg m_arg;
};

/*! Executes a function with one argument and returns her returned code
 */
template<
	typename _FunctionArg,
	typename _CalledArg
>
class FreeOneArgIntExecutableFunction
: public sad::AbsractThreadExecutableFunction
{
public:
	/*! Creates a function
		\param[in] f function, which will be executed
	 */
	inline FreeOneArgIntExecutableFunction(
		int (*f)(_FunctionArg),
		const _CalledArg & a
		) : m_f(f), m_arg(a)
	{
	}
	/*! Executes code  and returns zero
		\return 0
	 */
	virtual int execute()
	{
		return m_f(m_arg);
	}
	/*! Cretes a clone of executable function
		\returns exact copy of current thread executable function
	 */
	virtual AbsractThreadExecutableFunction * clone() const
	{
		return new sad::FreeOneArgIntExecutableFunction<_FunctionArg, _CalledArg>(*this);
	}
protected:
	int (*m_f)(_FunctionArg);
	_CalledArg m_arg;
};


/*! Executes a method with zero argument and returns zero
 */
template<
	typename _ClassName,
	typename _InvokedClassName
>
class FreeZeroArgVoidMethodExecutableFunction
:public sad::AbsractThreadExecutableFunction
{
public:
	/*! Creates a function
		\param[in] f function, which will be executed
	 */
	inline FreeZeroArgVoidMethodExecutableFunction(
		_ClassName * o,
		void (_InvokedClassName::*m)()
	) : m_o(o), m_m(m)
	{
	}
	/*! Executes code  and returns zero
		\return 0
	 */
	virtual int execute()
	{
		(m_o->*m_m)();
		return 0;
	}
	/*! Cretes a clone of executable function
		\returns exact copy of current thread executable function
	 */
	virtual AbsractThreadExecutableFunction * clone() const
	{
		return new sad::FreeZeroArgVoidMethodExecutableFunction<
			_ClassName, 
			_InvokedClassName>
		(*this);
	}
protected:
	_ClassName * m_o;
	void (_InvokedClassName::*m_m)();
};

/*! Executes a method with zero argument and returns it's returned code
 */
template<
	typename _ClassName,
	typename _InvokedClassName
>
class FreeZeroArgIntMethodExecutableFunction
:public sad::AbsractThreadExecutableFunction
{
public:
	/*! Creates a function
		\param[in] f function, which will be executed
	 */
	inline FreeZeroArgIntMethodExecutableFunction(
		_ClassName * o,
		int (_InvokedClassName::*m)()
	) : m_o(o), m_m(m)
	{
	}
	/*! Executes code  and returns zero
		\return 0
	 */
	virtual int execute()
	{
		return (m_o->*m_m)();
	}
	/*! Cretes a clone of executable function
		\returns exact copy of current thread executable function
	 */
	virtual AbsractThreadExecutableFunction * clone() const
	{
		return new sad::FreeZeroArgIntMethodExecutableFunction<
			_ClassName, 
			_InvokedClassName>
		(*this);
	}
protected:
	_ClassName * m_o;
	int (_InvokedClassName::*m_m)();
};

/*! Executes a method with one argument and returns zero
 */
template<
	typename _ClassName,
	typename _InvokedClassName,
	typename _MethodArg,
	typename _InvokedArg
>
class FreeOneArgVoidMethodExecutableFunction
:public sad::AbsractThreadExecutableFunction
{
public:
	/*! Creates a function
		\param[in] f function, which will be executed
	 */
	inline FreeOneArgVoidMethodExecutableFunction(
		_ClassName * o,
		void (_InvokedClassName::*m)(_MethodArg),
		const _InvokedArg & a
	) : m_o(o), m_m(m), m_a(a)
	{
	}
	/*! Executes code  and returns zero
		\return 0
	 */
	virtual int execute()
	{
		(m_o->*m_m)(m_a);
		return 0;
	}
	/*! Cretes a clone of executable function
		\returns exact copy of current thread executable function
	 */
	virtual AbsractThreadExecutableFunction * clone() const
	{
		return new sad::FreeOneArgVoidMethodExecutableFunction<
			_ClassName,
			_InvokedClassName,
			_MethodArg,
			_InvokedArg
		> 
		(*this);
	}
protected:
	_ClassName * m_o;
	void (_InvokedClassName::*m_m)(_MethodArg);
	_InvokedArg m_a;
};

/*! Executes a method with one argument and returns the code, which is returned by it's
	argument
 */
template<
	typename _ClassName,
	typename _InvokedClassName,
	typename _MethodArg,
	typename _InvokedArg
>
class FreeOneArgIntMethodExecutableFunction
:public sad::AbsractThreadExecutableFunction
{
public:
	/*! Creates a function
		\param[in] f function, which will be executed
	 */
	inline FreeOneArgIntMethodExecutableFunction(
		_ClassName * o,
		int (_InvokedClassName::*m)(_MethodArg),
		const _InvokedArg & a
	) : m_o(o), m_m(m), m_a(a)
	{
	}
	/*! Executes code  and returns zero
		\return 0
	 */
	virtual int execute()
	{
		return (m_o->*m_m)(m_a);
	}
	/*! Cretes a clone of executable function
		\returns exact copy of current thread executable function
	 */
	virtual AbsractThreadExecutableFunction * clone() const
	{
		return new sad::FreeOneArgIntMethodExecutableFunction<
			_ClassName,
			_InvokedClassName,
			_MethodArg,
			_InvokedArg
		> 
		(*this);
	}
protected:
	_ClassName * m_o;
	int (_InvokedClassName::*m_m)(_MethodArg);
	_InvokedArg m_a;
};

}

