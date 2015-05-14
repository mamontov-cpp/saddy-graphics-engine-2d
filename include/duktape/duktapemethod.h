/*! \file duktapemethod.h
    \author HiddenSeeker
     
    Defines a simple defines for creating bindings for any kind of method
 */
#pragma once
#include "duktapecallable.h"
#include "decay.h"
#include "getvalue.h"
#include "pushvalue.h"
#include "errorcodes.h"
#include "context.h"
#include "../db/dbtypename.h"

namespace sad
{

namespace duktape
{

/*! Defines a wrapper for method, which returns nothing and receives 0 arguments
 */
template<
    typename _ClassName
>
class VoidMethod0 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)();
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod0(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod0<_ClassName>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 1;
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        if (c->getTop() != 1)
        {
            c->throwError(sad::String("Method receives 1 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
                
        try
        {
            ((_cls.mutableValue()).*m_callee)();
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod0()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns nothing and receives 1 arguments
 */
template<
    typename _ClassName,
    typename _Arg0
>
class VoidMethod1 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod1(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod1<_ClassName, _Arg0>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 2;
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        if (c->getTop() != 2)
        {
            c->throwError(sad::String("Method receives 2 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 1);
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
               
        if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        
        try
        {
            ((_cls.mutableValue()).*m_callee)(_a0.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod1()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns nothing and receives 2 arguments
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1
>
class VoidMethod2 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod2(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod2<_ClassName, _Arg0, _Arg1>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 3;
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        if (c->getTop() != 3)
        {
            c->throwError(sad::String("Method receives 3 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 2);
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
               
        if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        
        try
        {
            ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod2()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns nothing and receives 3 arguments
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2
>
class VoidMethod3 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod3(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod3<_ClassName, _Arg0, _Arg1, _Arg2>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 4;
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        if (c->getTop() != 4)
        {
            c->throwError(sad::String("Method receives 4 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 3);
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
               
        if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        
        try
        {
            ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod3()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns nothing and receives 4 arguments
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3
>
class VoidMethod4 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod4(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod4<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 5;
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        if (c->getTop() != 5)
        {
            c->throwError(sad::String("Method receives 5 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 4);
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
               
        if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        
        try
        {
            ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod4()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns nothing and receives 5 arguments
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4
>
class VoidMethod5 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod5(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod5<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 6;
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        if (c->getTop() != 6)
        {
            c->throwError(sad::String("Method receives 6 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 5);
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
               
        if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        
        try
        {
            ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod5()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns nothing and receives 6 arguments
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5
>
class VoidMethod6 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod6(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod6<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 7;
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        if (c->getTop() != 7)
        {
            c->throwError(sad::String("Method receives 7 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 6);
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
               
        if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        
        try
        {
            ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod6()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns nothing and receives 7 arguments
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5,
    typename _Arg6
>
class VoidMethod7 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod7(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod7<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 8;
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        if (c->getTop() != 8)
        {
            c->throwError(sad::String("Method receives 8 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 7);
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
               
        if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        
        try
        {
            ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod7()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns nothing and receives 8 arguments
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5,
    typename _Arg6,
    typename _Arg7
>
class VoidMethod8 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod8(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod8<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 9;
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        if (c->getTop() != 9)
        {
            c->throwError(sad::String("Method receives 9 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 8);
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
               
        if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 9. Argument 9 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        
        try
        {
            ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod8()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns nothing and receives 9 arguments
 */
template<
    typename _ClassName,
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
class VoidMethod9 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod9(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod9<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 10;
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        if (c->getTop() != 10)
        {
            c->throwError(sad::String("Method receives 10 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 8);
        sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 9);
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
               
        if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 9. Argument 9 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a8.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::init();
            c->throwError("Invalid type passed for argument 10. Argument 10 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        
        try
        {
            ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod9()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns nothing and receives 10 arguments
 */
template<
    typename _ClassName,
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
class VoidMethod10 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod10(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod10<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 11;
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        if (c->getTop() != 11)
        {
            c->throwError(sad::String("Method receives 11 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 8);
        sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 9);
        sad::Maybe< typename sad::duktape::Decay<_Arg9>::Type > _a9 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg9>::Type >::perform(c, 10);
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
               
        if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 9. Argument 9 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a8.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::init();
            c->throwError("Invalid type passed for argument 10. Argument 10 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a9.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::init();
            c->throwError("Invalid type passed for argument 11. Argument 11 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        
        try
        {
            ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod10()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns nothing and receives 11 arguments
 */
template<
    typename _ClassName,
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
class VoidMethod11 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod11(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod11<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 12;
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        if (c->getTop() != 12)
        {
            c->throwError(sad::String("Method receives 12 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 8);
        sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 9);
        sad::Maybe< typename sad::duktape::Decay<_Arg9>::Type > _a9 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg9>::Type >::perform(c, 10);
        sad::Maybe< typename sad::duktape::Decay<_Arg10>::Type > _a10 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg10>::Type >::perform(c, 11);
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
               
        if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 9. Argument 9 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a8.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::init();
            c->throwError("Invalid type passed for argument 10. Argument 10 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a9.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::init();
            c->throwError("Invalid type passed for argument 11. Argument 11 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a10.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::init();
            c->throwError("Invalid type passed for argument 12. Argument 12 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        
        try
        {
            ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod11()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns nothing and receives 12 arguments
 */
template<
    typename _ClassName,
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
class VoidMethod12 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod12(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod12<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 13;
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        if (c->getTop() != 13)
        {
            c->throwError(sad::String("Method receives 13 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 8);
        sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 9);
        sad::Maybe< typename sad::duktape::Decay<_Arg9>::Type > _a9 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg9>::Type >::perform(c, 10);
        sad::Maybe< typename sad::duktape::Decay<_Arg10>::Type > _a10 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg10>::Type >::perform(c, 11);
        sad::Maybe< typename sad::duktape::Decay<_Arg11>::Type > _a11 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg11>::Type >::perform(c, 12);
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
               
        if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 9. Argument 9 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a8.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::init();
            c->throwError("Invalid type passed for argument 10. Argument 10 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a9.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::init();
            c->throwError("Invalid type passed for argument 11. Argument 11 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a10.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::init();
            c->throwError("Invalid type passed for argument 12. Argument 12 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a11.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::init();
            c->throwError("Invalid type passed for argument 13. Argument 13 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        
        try
        {
            ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod12()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns nothing and receives 13 arguments
 */
template<
    typename _ClassName,
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
class VoidMethod13 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod13(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod13<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 14;
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        if (c->getTop() != 14)
        {
            c->throwError(sad::String("Method receives 14 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 8);
        sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 9);
        sad::Maybe< typename sad::duktape::Decay<_Arg9>::Type > _a9 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg9>::Type >::perform(c, 10);
        sad::Maybe< typename sad::duktape::Decay<_Arg10>::Type > _a10 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg10>::Type >::perform(c, 11);
        sad::Maybe< typename sad::duktape::Decay<_Arg11>::Type > _a11 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg11>::Type >::perform(c, 12);
        sad::Maybe< typename sad::duktape::Decay<_Arg12>::Type > _a12 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg12>::Type >::perform(c, 13);
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
               
        if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 9. Argument 9 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a8.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::init();
            c->throwError("Invalid type passed for argument 10. Argument 10 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a9.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::init();
            c->throwError("Invalid type passed for argument 11. Argument 11 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a10.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::init();
            c->throwError("Invalid type passed for argument 12. Argument 12 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a11.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::init();
            c->throwError("Invalid type passed for argument 13. Argument 13 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a12.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg12>::Type >::init();
            c->throwError("Invalid type passed for argument 14. Argument 14 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg12>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        
        try
        {
            ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod13()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns nothing and receives 14 arguments
 */
template<
    typename _ClassName,
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
class VoidMethod14 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod14(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod14<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 15;
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        if (c->getTop() != 15)
        {
            c->throwError(sad::String("Method receives 15 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 8);
        sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 9);
        sad::Maybe< typename sad::duktape::Decay<_Arg9>::Type > _a9 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg9>::Type >::perform(c, 10);
        sad::Maybe< typename sad::duktape::Decay<_Arg10>::Type > _a10 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg10>::Type >::perform(c, 11);
        sad::Maybe< typename sad::duktape::Decay<_Arg11>::Type > _a11 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg11>::Type >::perform(c, 12);
        sad::Maybe< typename sad::duktape::Decay<_Arg12>::Type > _a12 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg12>::Type >::perform(c, 13);
        sad::Maybe< typename sad::duktape::Decay<_Arg13>::Type > _a13 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg13>::Type >::perform(c, 14);
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
               
        if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 9. Argument 9 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a8.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::init();
            c->throwError("Invalid type passed for argument 10. Argument 10 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a9.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::init();
            c->throwError("Invalid type passed for argument 11. Argument 11 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a10.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::init();
            c->throwError("Invalid type passed for argument 12. Argument 12 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a11.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::init();
            c->throwError("Invalid type passed for argument 13. Argument 13 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a12.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg12>::Type >::init();
            c->throwError("Invalid type passed for argument 14. Argument 14 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg12>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a13.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg13>::Type >::init();
            c->throwError("Invalid type passed for argument 15. Argument 15 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg13>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        
        try
        {
            ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod14()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns nothing and receives 15 arguments
 */
template<
    typename _ClassName,
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
class VoidMethod15 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod15(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod15<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 16;
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        if (c->getTop() != 16)
        {
            c->throwError(sad::String("Method receives 16 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 8);
        sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 9);
        sad::Maybe< typename sad::duktape::Decay<_Arg9>::Type > _a9 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg9>::Type >::perform(c, 10);
        sad::Maybe< typename sad::duktape::Decay<_Arg10>::Type > _a10 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg10>::Type >::perform(c, 11);
        sad::Maybe< typename sad::duktape::Decay<_Arg11>::Type > _a11 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg11>::Type >::perform(c, 12);
        sad::Maybe< typename sad::duktape::Decay<_Arg12>::Type > _a12 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg12>::Type >::perform(c, 13);
        sad::Maybe< typename sad::duktape::Decay<_Arg13>::Type > _a13 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg13>::Type >::perform(c, 14);
        sad::Maybe< typename sad::duktape::Decay<_Arg14>::Type > _a14 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg14>::Type >::perform(c, 15);
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
               
        if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 9. Argument 9 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a8.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::init();
            c->throwError("Invalid type passed for argument 10. Argument 10 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a9.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::init();
            c->throwError("Invalid type passed for argument 11. Argument 11 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a10.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::init();
            c->throwError("Invalid type passed for argument 12. Argument 12 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a11.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::init();
            c->throwError("Invalid type passed for argument 13. Argument 13 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a12.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg12>::Type >::init();
            c->throwError("Invalid type passed for argument 14. Argument 14 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg12>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a13.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg13>::Type >::init();
            c->throwError("Invalid type passed for argument 15. Argument 15 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg13>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a14.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg14>::Type >::init();
            c->throwError("Invalid type passed for argument 16. Argument 16 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg14>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        
        try
        {
            ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod15()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns nothing and receives 16 arguments
 */
template<
    typename _ClassName,
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
class VoidMethod16 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidMethod16(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidMethod16<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()
    {
        return 17;
    }
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
        if (c->getTop() != 17)
        {
            c->throwError(sad::String("Method receives 17 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);      
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 8);
        sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 9);
        sad::Maybe< typename sad::duktape::Decay<_Arg9>::Type > _a9 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg9>::Type >::perform(c, 10);
        sad::Maybe< typename sad::duktape::Decay<_Arg10>::Type > _a10 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg10>::Type >::perform(c, 11);
        sad::Maybe< typename sad::duktape::Decay<_Arg11>::Type > _a11 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg11>::Type >::perform(c, 12);
        sad::Maybe< typename sad::duktape::Decay<_Arg12>::Type > _a12 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg12>::Type >::perform(c, 13);
        sad::Maybe< typename sad::duktape::Decay<_Arg13>::Type > _a13 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg13>::Type >::perform(c, 14);
        sad::Maybe< typename sad::duktape::Decay<_Arg14>::Type > _a14 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg14>::Type >::perform(c, 15);
        sad::Maybe< typename sad::duktape::Decay<_Arg15>::Type > _a15 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg15>::Type >::perform(c, 16);
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
               
        if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 9. Argument 9 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a8.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::init();
            c->throwError("Invalid type passed for argument 10. Argument 10 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a9.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::init();
            c->throwError("Invalid type passed for argument 11. Argument 11 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a10.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::init();
            c->throwError("Invalid type passed for argument 12. Argument 12 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a11.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::init();
            c->throwError("Invalid type passed for argument 13. Argument 13 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a12.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg12>::Type >::init();
            c->throwError("Invalid type passed for argument 14. Argument 14 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg12>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a13.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg13>::Type >::init();
            c->throwError("Invalid type passed for argument 15. Argument 15 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg13>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a14.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg14>::Type >::init();
            c->throwError("Invalid type passed for argument 16. Argument 16 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg14>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       
        if (_a15.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg15>::Type >::init();
            c->throwError("Invalid type passed for argument 17. Argument 17 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg15>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        
        try
        {
            ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue(), _a15.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidMethod16()
    {
    }   
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns some value and receives 0 arguments
 */
template<
    typename _ClassName,
    typename _ReturnType
>
class RetMethod0 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)();
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod0(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod0<_ClassName, _ReturnType>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 1;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
		if (c->getTop() != 1)
        {
            c->throwError(sad::String("Method receives 1 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)();
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod0()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns some value and receives 1 arguments
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0
>
class RetMethod1 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod1(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod1<_ClassName, _ReturnType, _Arg0>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 2;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
		if (c->getTop() != 2)
        {
            c->throwError(sad::String("Method receives 2 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }

       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)(_a0.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod1()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns some value and receives 2 arguments
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1
>
class RetMethod2 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod2(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod2<_ClassName, _ReturnType, _Arg0, _Arg1>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 3;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
		if (c->getTop() != 3)
        {
            c->throwError(sad::String("Method receives 3 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }

       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod2()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns some value and receives 3 arguments
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2
>
class RetMethod3 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod3(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod3<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 4;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
		if (c->getTop() != 4)
        {
            c->throwError(sad::String("Method receives 4 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }

       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod3()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns some value and receives 4 arguments
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3
>
class RetMethod4 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod4(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod4<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 5;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
		if (c->getTop() != 5)
        {
            c->throwError(sad::String("Method receives 5 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }

       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod4()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns some value and receives 5 arguments
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4
>
class RetMethod5 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod5(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod5<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 6;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
		if (c->getTop() != 6)
        {
            c->throwError(sad::String("Method receives 6 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
        
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }

       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod5()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns some value and receives 6 arguments
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5
>
class RetMethod6 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod6(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod6<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 7;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
		if (c->getTop() != 7)
        {
            c->throwError(sad::String("Method receives 7 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
        
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }

       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod6()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns some value and receives 7 arguments
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5,
    typename _Arg6
>
class RetMethod7 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod7(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod7<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 8;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
		if (c->getTop() != 8)
        {
            c->throwError(sad::String("Method receives 8 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 6);
        
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }

       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod7()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns some value and receives 8 arguments
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5,
    typename _Arg6,
    typename _Arg7
>
class RetMethod8 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod8(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod8<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 9;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
		if (c->getTop() != 9)
        {
            c->throwError(sad::String("Method receives 9 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 7);
        
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }

       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod8()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns some value and receives 9 arguments
 */
template<
    typename _ClassName,
    typename _ReturnType,
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
class RetMethod9 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod9(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod9<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 10;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
		if (c->getTop() != 10)
        {
            c->throwError(sad::String("Method receives 10 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 8);
        
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a8.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::init();
            c->throwError("Invalid type passed for argument 9. Argument 9 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }

       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod9()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns some value and receives 10 arguments
 */
template<
    typename _ClassName,
    typename _ReturnType,
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
class RetMethod10 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod10(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod10<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 11;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
		if (c->getTop() != 11)
        {
            c->throwError(sad::String("Method receives 11 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 8);
        sad::Maybe< typename sad::duktape::Decay<_Arg9>::Type > _a9 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg9>::Type >::perform(c, 9);
        
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a8.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::init();
            c->throwError("Invalid type passed for argument 9. Argument 9 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a9.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::init();
            c->throwError("Invalid type passed for argument 10. Argument 10 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }

       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod10()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns some value and receives 11 arguments
 */
template<
    typename _ClassName,
    typename _ReturnType,
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
class RetMethod11 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod11(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod11<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 12;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
		if (c->getTop() != 12)
        {
            c->throwError(sad::String("Method receives 12 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 8);
        sad::Maybe< typename sad::duktape::Decay<_Arg9>::Type > _a9 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg9>::Type >::perform(c, 9);
        sad::Maybe< typename sad::duktape::Decay<_Arg10>::Type > _a10 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg10>::Type >::perform(c, 10);
        
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a8.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::init();
            c->throwError("Invalid type passed for argument 9. Argument 9 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a9.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::init();
            c->throwError("Invalid type passed for argument 10. Argument 10 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a10.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::init();
            c->throwError("Invalid type passed for argument 11. Argument 11 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }

       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod11()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns some value and receives 12 arguments
 */
template<
    typename _ClassName,
    typename _ReturnType,
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
class RetMethod12 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod12(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod12<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 13;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
		if (c->getTop() != 13)
        {
            c->throwError(sad::String("Method receives 13 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 8);
        sad::Maybe< typename sad::duktape::Decay<_Arg9>::Type > _a9 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg9>::Type >::perform(c, 9);
        sad::Maybe< typename sad::duktape::Decay<_Arg10>::Type > _a10 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg10>::Type >::perform(c, 10);
        sad::Maybe< typename sad::duktape::Decay<_Arg11>::Type > _a11 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg11>::Type >::perform(c, 11);
        
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a8.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::init();
            c->throwError("Invalid type passed for argument 9. Argument 9 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a9.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::init();
            c->throwError("Invalid type passed for argument 10. Argument 10 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a10.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::init();
            c->throwError("Invalid type passed for argument 11. Argument 11 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a11.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::init();
            c->throwError("Invalid type passed for argument 12. Argument 12 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }

       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod12()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns some value and receives 13 arguments
 */
template<
    typename _ClassName,
    typename _ReturnType,
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
class RetMethod13 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod13(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod13<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 14;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
		if (c->getTop() != 14)
        {
            c->throwError(sad::String("Method receives 14 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 8);
        sad::Maybe< typename sad::duktape::Decay<_Arg9>::Type > _a9 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg9>::Type >::perform(c, 9);
        sad::Maybe< typename sad::duktape::Decay<_Arg10>::Type > _a10 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg10>::Type >::perform(c, 10);
        sad::Maybe< typename sad::duktape::Decay<_Arg11>::Type > _a11 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg11>::Type >::perform(c, 11);
        sad::Maybe< typename sad::duktape::Decay<_Arg12>::Type > _a12 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg12>::Type >::perform(c, 12);
        
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a8.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::init();
            c->throwError("Invalid type passed for argument 9. Argument 9 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a9.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::init();
            c->throwError("Invalid type passed for argument 10. Argument 10 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a10.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::init();
            c->throwError("Invalid type passed for argument 11. Argument 11 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a11.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::init();
            c->throwError("Invalid type passed for argument 12. Argument 12 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a12.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg12>::Type >::init();
            c->throwError("Invalid type passed for argument 13. Argument 13 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg12>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }

       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod13()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns some value and receives 14 arguments
 */
template<
    typename _ClassName,
    typename _ReturnType,
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
class RetMethod14 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod14(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod14<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 15;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
		if (c->getTop() != 15)
        {
            c->throwError(sad::String("Method receives 15 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 8);
        sad::Maybe< typename sad::duktape::Decay<_Arg9>::Type > _a9 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg9>::Type >::perform(c, 9);
        sad::Maybe< typename sad::duktape::Decay<_Arg10>::Type > _a10 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg10>::Type >::perform(c, 10);
        sad::Maybe< typename sad::duktape::Decay<_Arg11>::Type > _a11 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg11>::Type >::perform(c, 11);
        sad::Maybe< typename sad::duktape::Decay<_Arg12>::Type > _a12 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg12>::Type >::perform(c, 12);
        sad::Maybe< typename sad::duktape::Decay<_Arg13>::Type > _a13 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg13>::Type >::perform(c, 13);
        
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a8.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::init();
            c->throwError("Invalid type passed for argument 9. Argument 9 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a9.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::init();
            c->throwError("Invalid type passed for argument 10. Argument 10 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a10.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::init();
            c->throwError("Invalid type passed for argument 11. Argument 11 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a11.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::init();
            c->throwError("Invalid type passed for argument 12. Argument 12 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a12.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg12>::Type >::init();
            c->throwError("Invalid type passed for argument 13. Argument 13 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg12>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a13.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg13>::Type >::init();
            c->throwError("Invalid type passed for argument 14. Argument 14 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg13>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }

       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod14()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns some value and receives 15 arguments
 */
template<
    typename _ClassName,
    typename _ReturnType,
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
class RetMethod15 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod15(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod15<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 16;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
		if (c->getTop() != 16)
        {
            c->throwError(sad::String("Method receives 16 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 8);
        sad::Maybe< typename sad::duktape::Decay<_Arg9>::Type > _a9 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg9>::Type >::perform(c, 9);
        sad::Maybe< typename sad::duktape::Decay<_Arg10>::Type > _a10 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg10>::Type >::perform(c, 10);
        sad::Maybe< typename sad::duktape::Decay<_Arg11>::Type > _a11 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg11>::Type >::perform(c, 11);
        sad::Maybe< typename sad::duktape::Decay<_Arg12>::Type > _a12 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg12>::Type >::perform(c, 12);
        sad::Maybe< typename sad::duktape::Decay<_Arg13>::Type > _a13 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg13>::Type >::perform(c, 13);
        sad::Maybe< typename sad::duktape::Decay<_Arg14>::Type > _a14 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg14>::Type >::perform(c, 14);
        
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a8.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::init();
            c->throwError("Invalid type passed for argument 9. Argument 9 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a9.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::init();
            c->throwError("Invalid type passed for argument 10. Argument 10 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a10.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::init();
            c->throwError("Invalid type passed for argument 11. Argument 11 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a11.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::init();
            c->throwError("Invalid type passed for argument 12. Argument 12 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a12.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg12>::Type >::init();
            c->throwError("Invalid type passed for argument 13. Argument 13 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg12>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a13.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg13>::Type >::init();
            c->throwError("Invalid type passed for argument 14. Argument 14 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg13>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a14.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg14>::Type >::init();
            c->throwError("Invalid type passed for argument 15. Argument 15 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg14>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }

       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod15()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Defines a wrapper for method, which returns some value and receives 16 arguments
 */
template<
    typename _ClassName,
    typename _ReturnType,
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
class RetMethod16 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetMethod16(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetMethod16<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 17;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
		if (c->getTop() != 17)
        {
            c->throwError(sad::String("Method receives 17 arguments, but ") + sad::String::number(c->getTop()) + " given");
            return 0;
        }

        sad::Maybe<_ClassName> _cls = sad::duktape::GetValue< _ClassName >::perform(c, 0);  
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 8);
        sad::Maybe< typename sad::duktape::Decay<_Arg9>::Type > _a9 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg9>::Type >::perform(c, 9);
        sad::Maybe< typename sad::duktape::Decay<_Arg10>::Type > _a10 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg10>::Type >::perform(c, 10);
        sad::Maybe< typename sad::duktape::Decay<_Arg11>::Type > _a11 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg11>::Type >::perform(c, 11);
        sad::Maybe< typename sad::duktape::Decay<_Arg12>::Type > _a12 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg12>::Type >::perform(c, 12);
        sad::Maybe< typename sad::duktape::Decay<_Arg13>::Type > _a13 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg13>::Type >::perform(c, 13);
        sad::Maybe< typename sad::duktape::Decay<_Arg14>::Type > _a14 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg14>::Type >::perform(c, 14);
        sad::Maybe< typename sad::duktape::Decay<_Arg15>::Type > _a15 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg15>::Type >::perform(c, 15);
        
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a1.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::init();
            c->throwError("Invalid type passed for argument 2. Argument 2 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg1>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a2.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::init();
            c->throwError("Invalid type passed for argument 3. Argument 3 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg2>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a3.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::init();
            c->throwError("Invalid type passed for argument 4. Argument 4 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg3>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a4.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::init();
            c->throwError("Invalid type passed for argument 5. Argument 5 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg4>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a5.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::init();
            c->throwError("Invalid type passed for argument 6. Argument 6 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg5>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a6.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::init();
            c->throwError("Invalid type passed for argument 7. Argument 7 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg6>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a7.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::init();
            c->throwError("Invalid type passed for argument 8. Argument 8 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg7>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a8.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::init();
            c->throwError("Invalid type passed for argument 9. Argument 9 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg8>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a9.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::init();
            c->throwError("Invalid type passed for argument 10. Argument 10 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg9>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a10.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::init();
            c->throwError("Invalid type passed for argument 11. Argument 11 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg10>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a11.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::init();
            c->throwError("Invalid type passed for argument 12. Argument 12 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg11>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a12.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg12>::Type >::init();
            c->throwError("Invalid type passed for argument 13. Argument 13 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg12>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a13.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg13>::Type >::init();
            c->throwError("Invalid type passed for argument 14. Argument 14 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg13>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a14.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg14>::Type >::init();
            c->throwError("Invalid type passed for argument 15. Argument 15 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg14>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
       if (_a15.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg15>::Type >::init();
            c->throwError("Invalid type passed for argument 16. Argument 16 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg15>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }

       try
        {
            _ReturnType t = ((_cls.mutableValue()).*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue(), _a15.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling method");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~RetMethod16()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};

/*! Makes callable from method which returns nothing and receives 0 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName
>
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)())
{
    return new VoidMethod0<_ClassName>(f);
}

/*! Makes callable from method which returns nothing and receives 1 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _Arg0
>
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)(_Arg0))
{
    return new VoidMethod1<_ClassName, _Arg0>(f);
}

/*! Makes callable from method which returns nothing and receives 2 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1
>
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)(_Arg0, _Arg1))
{
    return new VoidMethod2<_ClassName, _Arg0, _Arg1>(f);
}

/*! Makes callable from method which returns nothing and receives 3 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2
>
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2))
{
    return new VoidMethod3<_ClassName, _Arg0, _Arg1, _Arg2>(f);
}

/*! Makes callable from method which returns nothing and receives 4 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3
>
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3))
{
    return new VoidMethod4<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3>(f);
}

/*! Makes callable from method which returns nothing and receives 5 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4
>
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4))
{
    return new VoidMethod5<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>(f);
}

/*! Makes callable from method which returns nothing and receives 6 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5
>
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5))
{
    return new VoidMethod6<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>(f);
}

/*! Makes callable from method which returns nothing and receives 7 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5,
    typename _Arg6
>
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6))
{
    return new VoidMethod7<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>(f);
}

/*! Makes callable from method which returns nothing and receives 8 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5,
    typename _Arg6,
    typename _Arg7
>
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7))
{
    return new VoidMethod8<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>(f);
}

/*! Makes callable from method which returns nothing and receives 9 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
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
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8))
{
    return new VoidMethod9<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>(f);
}

/*! Makes callable from method which returns nothing and receives 10 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
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
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9))
{
    return new VoidMethod10<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>(f);
}

/*! Makes callable from method which returns nothing and receives 11 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
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
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10))
{
    return new VoidMethod11<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>(f);
}

/*! Makes callable from method which returns nothing and receives 12 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
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
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11))
{
    return new VoidMethod12<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>(f);
}

/*! Makes callable from method which returns nothing and receives 13 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
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
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12))
{
    return new VoidMethod13<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>(f);
}

/*! Makes callable from method which returns nothing and receives 14 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
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
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13))
{
    return new VoidMethod14<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>(f);
}

/*! Makes callable from method which returns nothing and receives 15 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
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
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14))
{
    return new VoidMethod15<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>(f);
}

/*! Makes callable from method which returns nothing and receives 16 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
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
inline sad::duktape::DuktapeCallable* make_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15))
{
    return new VoidMethod16<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>(f);
}


/*! Makes callable from method which returns value and receives 0 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)())
{
    return new RetMethod0<_ClassName, _ReturnType>(f);
}

/*! Makes callable from method which returns value and receives 1 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)(_Arg0))
{
    return new RetMethod1<_ClassName, _ReturnType, _Arg0>(f);
}

/*! Makes callable from method which returns value and receives 2 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1))
{
    return new RetMethod2<_ClassName, _ReturnType, _Arg0, _Arg1>(f);
}

/*! Makes callable from method which returns value and receives 3 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2))
{
    return new RetMethod3<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2>(f);
}

/*! Makes callable from method which returns value and receives 4 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3))
{
    return new RetMethod4<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3>(f);
}

/*! Makes callable from method which returns value and receives 5 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4))
{
    return new RetMethod5<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>(f);
}

/*! Makes callable from method which returns value and receives 6 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5))
{
    return new RetMethod6<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>(f);
}

/*! Makes callable from method which returns value and receives 7 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5,
    typename _Arg6
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6))
{
    return new RetMethod7<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>(f);
}

/*! Makes callable from method which returns value and receives 8 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5,
    typename _Arg6,
    typename _Arg7
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7))
{
    return new RetMethod8<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>(f);
}

/*! Makes callable from method which returns value and receives 9 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
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
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8))
{
    return new RetMethod9<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>(f);
}

/*! Makes callable from method which returns value and receives 10 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
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
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9))
{
    return new RetMethod10<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>(f);
}

/*! Makes callable from method which returns value and receives 11 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
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
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10))
{
    return new RetMethod11<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>(f);
}

/*! Makes callable from method which returns value and receives 12 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
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
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11))
{
    return new RetMethod12<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>(f);
}

/*! Makes callable from method which returns value and receives 13 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
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
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12))
{
    return new RetMethod13<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>(f);
}

/*! Makes callable from method which returns value and receives 14 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
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
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13))
{
    return new RetMethod14<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>(f);
}

/*! Makes callable from method which returns value and receives 15 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
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
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14))
{
    return new RetMethod15<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>(f);
}

/*! Makes callable from method which returns value and receives 16 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
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
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15))
{
    return new RetMethod16<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>(f);
}

}

}