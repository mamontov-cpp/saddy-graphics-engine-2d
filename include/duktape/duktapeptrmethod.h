/*! \file duktapeptrmethod.h
    \author HiddenSeeker
     
    Defines a simple defines for creating bindings for method, which will be used to call
	method on pointer to object
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
class VoidPtrMethod0 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)();
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidPtrMethod0(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidPtrMethod0<_ClassName>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
                
        try
        {
            ((_cls.mutableValue())->*m_callee)();
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
    virtual ~VoidPtrMethod0()
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
class VoidPtrMethod1 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidPtrMethod1(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidPtrMethod1<_ClassName, _Arg0>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue());
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
    virtual ~VoidPtrMethod1()
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
class VoidPtrMethod2 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidPtrMethod2(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidPtrMethod2<_ClassName, _Arg0, _Arg1>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue());
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
    virtual ~VoidPtrMethod2()
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
class VoidPtrMethod3 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidPtrMethod3(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidPtrMethod3<_ClassName, _Arg0, _Arg1, _Arg2>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue());
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
    virtual ~VoidPtrMethod3()
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
class VoidPtrMethod4 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidPtrMethod4(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidPtrMethod4<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue());
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
    virtual ~VoidPtrMethod4()
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
class VoidPtrMethod5 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidPtrMethod5(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidPtrMethod5<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue());
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
    virtual ~VoidPtrMethod5()
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
class VoidPtrMethod6 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidPtrMethod6(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidPtrMethod6<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue());
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
    virtual ~VoidPtrMethod6()
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
class VoidPtrMethod7 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidPtrMethod7(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidPtrMethod7<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue());
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
    virtual ~VoidPtrMethod7()
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
class VoidPtrMethod8 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidPtrMethod8(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidPtrMethod8<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue());
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
    virtual ~VoidPtrMethod8()
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
class VoidPtrMethod9 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidPtrMethod9(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidPtrMethod9<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue());
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
    virtual ~VoidPtrMethod9()
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
class VoidPtrMethod10 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidPtrMethod10(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidPtrMethod10<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue());
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
    virtual ~VoidPtrMethod10()
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
class VoidPtrMethod11 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidPtrMethod11(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidPtrMethod11<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue());
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
    virtual ~VoidPtrMethod11()
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
class VoidPtrMethod12 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidPtrMethod12(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidPtrMethod12<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue());
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
    virtual ~VoidPtrMethod12()
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
class VoidPtrMethod13 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidPtrMethod13(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidPtrMethod13<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue());
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
    virtual ~VoidPtrMethod13()
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
class VoidPtrMethod14 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidPtrMethod14(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidPtrMethod14<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue());
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
    virtual ~VoidPtrMethod14()
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
class VoidPtrMethod15 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidPtrMethod15(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidPtrMethod15<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue());
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
    virtual ~VoidPtrMethod15()
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
class VoidPtrMethod16 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidPtrMethod16(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::VoidPtrMethod16<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue(), _a15.mutableValue());
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
    virtual ~VoidPtrMethod16()
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
class RetPtrMethod0 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)();
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetPtrMethod0(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetPtrMethod0<_ClassName, _ReturnType>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
       try
        {
            _ReturnType t = ((_cls.mutableValue())->*m_callee)();
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
    virtual ~RetPtrMethod0()
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
class RetPtrMethod1 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetPtrMethod1(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetPtrMethod1<_ClassName, _ReturnType, _Arg0>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue());
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
    virtual ~RetPtrMethod1()
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
class RetPtrMethod2 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetPtrMethod2(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetPtrMethod2<_ClassName, _ReturnType, _Arg0, _Arg1>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue());
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
    virtual ~RetPtrMethod2()
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
class RetPtrMethod3 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetPtrMethod3(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetPtrMethod3<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue());
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
    virtual ~RetPtrMethod3()
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
class RetPtrMethod4 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetPtrMethod4(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetPtrMethod4<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue());
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
    virtual ~RetPtrMethod4()
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
class RetPtrMethod5 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetPtrMethod5(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetPtrMethod5<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue());
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
    virtual ~RetPtrMethod5()
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
class RetPtrMethod6 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetPtrMethod6(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetPtrMethod6<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue());
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
    virtual ~RetPtrMethod6()
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
class RetPtrMethod7 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetPtrMethod7(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetPtrMethod7<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue());
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
    virtual ~RetPtrMethod7()
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
class RetPtrMethod8 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetPtrMethod8(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetPtrMethod8<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue());
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
    virtual ~RetPtrMethod8()
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
class RetPtrMethod9 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetPtrMethod9(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetPtrMethod9<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue());
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
    virtual ~RetPtrMethod9()
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
class RetPtrMethod10 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetPtrMethod10(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetPtrMethod10<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue());
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
    virtual ~RetPtrMethod10()
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
class RetPtrMethod11 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetPtrMethod11(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetPtrMethod11<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue());
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
    virtual ~RetPtrMethod11()
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
class RetPtrMethod12 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetPtrMethod12(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetPtrMethod12<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue());
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
    virtual ~RetPtrMethod12()
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
class RetPtrMethod13 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetPtrMethod13(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetPtrMethod13<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue());
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
    virtual ~RetPtrMethod13()
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
class RetPtrMethod14 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetPtrMethod14(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetPtrMethod14<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue());
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
    virtual ~RetPtrMethod14()
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
class RetPtrMethod15 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetPtrMethod15(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetPtrMethod15<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue());
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
    virtual ~RetPtrMethod15()
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
class RetPtrMethod16 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetPtrMethod16(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetPtrMethod16<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue(), _a15.mutableValue());
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
    virtual ~RetPtrMethod16()
    {
    }
protected:
    /*! A method, which is being wrapped
     */
    Method m_callee;
};


/*! Defines a wrapper for method, which returns nothing and receives 0 arguments
 */
template<
    typename _ClassName
>
class ConstVoidPtrMethod0 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)() const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstVoidPtrMethod0(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::ConstVoidPtrMethod0<_ClassName>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
       
        if (_cls.exists() == false) 
        {
            sad::db::TypeName< _ClassName >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< _ClassName  >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
                
        try
        {
            ((_cls.mutableValue())->*m_callee)();
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
    virtual ~ConstVoidPtrMethod0()
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
class ConstVoidPtrMethod1 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstVoidPtrMethod1(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::ConstVoidPtrMethod1<_ClassName, _Arg0>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue());
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
    virtual ~ConstVoidPtrMethod1()
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
class ConstVoidPtrMethod2 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstVoidPtrMethod2(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::ConstVoidPtrMethod2<_ClassName, _Arg0, _Arg1>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue());
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
    virtual ~ConstVoidPtrMethod2()
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
class ConstVoidPtrMethod3 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstVoidPtrMethod3(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::ConstVoidPtrMethod3<_ClassName, _Arg0, _Arg1, _Arg2>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue());
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
    virtual ~ConstVoidPtrMethod3()
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
class ConstVoidPtrMethod4 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstVoidPtrMethod4(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::ConstVoidPtrMethod4<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue());
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
    virtual ~ConstVoidPtrMethod4()
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
class ConstVoidPtrMethod5 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstVoidPtrMethod5(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::ConstVoidPtrMethod5<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue());
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
    virtual ~ConstVoidPtrMethod5()
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
class ConstVoidPtrMethod6 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstVoidPtrMethod6(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::ConstVoidPtrMethod6<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue());
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
    virtual ~ConstVoidPtrMethod6()
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
class ConstVoidPtrMethod7 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstVoidPtrMethod7(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::ConstVoidPtrMethod7<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue());
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
    virtual ~ConstVoidPtrMethod7()
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
class ConstVoidPtrMethod8 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstVoidPtrMethod8(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::ConstVoidPtrMethod8<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue());
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
    virtual ~ConstVoidPtrMethod8()
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
class ConstVoidPtrMethod9 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstVoidPtrMethod9(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::ConstVoidPtrMethod9<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue());
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
    virtual ~ConstVoidPtrMethod9()
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
class ConstVoidPtrMethod10 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstVoidPtrMethod10(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::ConstVoidPtrMethod10<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue());
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
    virtual ~ConstVoidPtrMethod10()
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
class ConstVoidPtrMethod11 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstVoidPtrMethod11(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::ConstVoidPtrMethod11<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue());
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
    virtual ~ConstVoidPtrMethod11()
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
class ConstVoidPtrMethod12 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstVoidPtrMethod12(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::ConstVoidPtrMethod12<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue());
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
    virtual ~ConstVoidPtrMethod12()
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
class ConstVoidPtrMethod13 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstVoidPtrMethod13(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::ConstVoidPtrMethod13<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue());
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
    virtual ~ConstVoidPtrMethod13()
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
class ConstVoidPtrMethod14 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstVoidPtrMethod14(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::ConstVoidPtrMethod14<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue());
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
    virtual ~ConstVoidPtrMethod14()
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
class ConstVoidPtrMethod15 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstVoidPtrMethod15(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::ConstVoidPtrMethod15<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue());
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
    virtual ~ConstVoidPtrMethod15()
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
class ConstVoidPtrMethod16 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstVoidPtrMethod16(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::ConstVoidPtrMethod16<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);      
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
            ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue(), _a15.mutableValue());
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
    virtual ~ConstVoidPtrMethod16()
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
class ConstRetPtrMethod0 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)() const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstRetPtrMethod0(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::ConstRetPtrMethod0<_ClassName, _ReturnType>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
       try
        {
            _ReturnType t = ((_cls.mutableValue())->*m_callee)();
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
    virtual ~ConstRetPtrMethod0()
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
class ConstRetPtrMethod1 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstRetPtrMethod1(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::ConstRetPtrMethod1<_ClassName, _ReturnType, _Arg0>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue());
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
    virtual ~ConstRetPtrMethod1()
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
class ConstRetPtrMethod2 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstRetPtrMethod2(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::ConstRetPtrMethod2<_ClassName, _ReturnType, _Arg0, _Arg1>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue());
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
    virtual ~ConstRetPtrMethod2()
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
class ConstRetPtrMethod3 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstRetPtrMethod3(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::ConstRetPtrMethod3<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue());
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
    virtual ~ConstRetPtrMethod3()
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
class ConstRetPtrMethod4 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstRetPtrMethod4(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::ConstRetPtrMethod4<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue());
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
    virtual ~ConstRetPtrMethod4()
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
class ConstRetPtrMethod5 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstRetPtrMethod5(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::ConstRetPtrMethod5<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue());
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
    virtual ~ConstRetPtrMethod5()
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
class ConstRetPtrMethod6 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstRetPtrMethod6(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::ConstRetPtrMethod6<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue());
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
    virtual ~ConstRetPtrMethod6()
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
class ConstRetPtrMethod7 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstRetPtrMethod7(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::ConstRetPtrMethod7<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue());
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
    virtual ~ConstRetPtrMethod7()
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
class ConstRetPtrMethod8 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstRetPtrMethod8(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::ConstRetPtrMethod8<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue());
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
    virtual ~ConstRetPtrMethod8()
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
class ConstRetPtrMethod9 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstRetPtrMethod9(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::ConstRetPtrMethod9<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue());
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
    virtual ~ConstRetPtrMethod9()
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
class ConstRetPtrMethod10 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstRetPtrMethod10(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::ConstRetPtrMethod10<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue());
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
    virtual ~ConstRetPtrMethod10()
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
class ConstRetPtrMethod11 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstRetPtrMethod11(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::ConstRetPtrMethod11<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue());
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
    virtual ~ConstRetPtrMethod11()
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
class ConstRetPtrMethod12 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstRetPtrMethod12(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::ConstRetPtrMethod12<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue());
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
    virtual ~ConstRetPtrMethod12()
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
class ConstRetPtrMethod13 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstRetPtrMethod13(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::ConstRetPtrMethod13<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue());
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
    virtual ~ConstRetPtrMethod13()
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
class ConstRetPtrMethod14 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstRetPtrMethod14(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::ConstRetPtrMethod14<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue());
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
    virtual ~ConstRetPtrMethod14()
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
class ConstRetPtrMethod15 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstRetPtrMethod15(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::ConstRetPtrMethod15<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue());
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
    virtual ~ConstRetPtrMethod15()
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
class ConstRetPtrMethod16 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (_ClassName::*Method)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15) const;
    /*! Constructs new function wrapper
        \param[in] f function
     */
    ConstRetPtrMethod16(Method f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::ConstRetPtrMethod16<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>(m_callee);
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

        sad::Maybe<_ClassName*> _cls = sad::duktape::GetValue< _ClassName* >::perform(c, 0);  
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
            _ReturnType t = ((_cls.mutableValue())->*m_callee)(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue(), _a15.mutableValue());
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
    virtual ~ConstRetPtrMethod16()
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)())
{
    return new VoidPtrMethod0<_ClassName>(f);
}

/*! Makes callable from method which returns nothing and receives 1 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _Arg0
>
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0))
{
    return new VoidPtrMethod1<_ClassName, _Arg0>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1))
{
    return new VoidPtrMethod2<_ClassName, _Arg0, _Arg1>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2))
{
    return new VoidPtrMethod3<_ClassName, _Arg0, _Arg1, _Arg2>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3))
{
    return new VoidPtrMethod4<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4))
{
    return new VoidPtrMethod5<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5))
{
    return new VoidPtrMethod6<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6))
{
    return new VoidPtrMethod7<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7))
{
    return new VoidPtrMethod8<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8))
{
    return new VoidPtrMethod9<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9))
{
    return new VoidPtrMethod10<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10))
{
    return new VoidPtrMethod11<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11))
{
    return new VoidPtrMethod12<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12))
{
    return new VoidPtrMethod13<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13))
{
    return new VoidPtrMethod14<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14))
{
    return new VoidPtrMethod15<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15))
{
    return new VoidPtrMethod16<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>(f);
}


/*! Makes callable from method which returns value and receives 0 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType
>
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)())
{
    return new RetPtrMethod0<_ClassName, _ReturnType>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0))
{
    return new RetPtrMethod1<_ClassName, _ReturnType, _Arg0>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1))
{
    return new RetPtrMethod2<_ClassName, _ReturnType, _Arg0, _Arg1>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2))
{
    return new RetPtrMethod3<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3))
{
    return new RetPtrMethod4<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4))
{
    return new RetPtrMethod5<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5))
{
    return new RetPtrMethod6<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6))
{
    return new RetPtrMethod7<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7))
{
    return new RetPtrMethod8<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8))
{
    return new RetPtrMethod9<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9))
{
    return new RetPtrMethod10<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10))
{
    return new RetPtrMethod11<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11))
{
    return new RetPtrMethod12<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12))
{
    return new RetPtrMethod13<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13))
{
    return new RetPtrMethod14<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14))
{
    return new RetPtrMethod15<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>(f);
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15))
{
    return new RetPtrMethod16<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>(f);
}

/*! Makes callable from const method which returns nothing and receives 0 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName
>
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)() const)
{
    return new ConstVoidPtrMethod0<_ClassName>(f);
}

/*! Makes callable from const method which returns nothing and receives 1 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _Arg0
>
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0) const)
{
    return new ConstVoidPtrMethod1<_ClassName, _Arg0>(f);
}

/*! Makes callable from const method which returns nothing and receives 2 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1
>
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1) const)
{
    return new ConstVoidPtrMethod2<_ClassName, _Arg0, _Arg1>(f);
}

/*! Makes callable from const method which returns nothing and receives 3 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2
>
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2) const)
{
    return new ConstVoidPtrMethod3<_ClassName, _Arg0, _Arg1, _Arg2>(f);
}

/*! Makes callable from const method which returns nothing and receives 4 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3) const)
{
    return new ConstVoidPtrMethod4<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3>(f);
}

/*! Makes callable from const method which returns nothing and receives 5 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4) const)
{
    return new ConstVoidPtrMethod5<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>(f);
}

/*! Makes callable from const method which returns nothing and receives 6 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5) const)
{
    return new ConstVoidPtrMethod6<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>(f);
}

/*! Makes callable from const method which returns nothing and receives 7 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6) const)
{
    return new ConstVoidPtrMethod7<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>(f);
}

/*! Makes callable from const method which returns nothing and receives 8 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7) const)
{
    return new ConstVoidPtrMethod8<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>(f);
}

/*! Makes callable from const method which returns nothing and receives 9 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8) const)
{
    return new ConstVoidPtrMethod9<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>(f);
}

/*! Makes callable from const method which returns nothing and receives 10 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9) const)
{
    return new ConstVoidPtrMethod10<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>(f);
}

/*! Makes callable from const method which returns nothing and receives 11 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10) const)
{
    return new ConstVoidPtrMethod11<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>(f);
}

/*! Makes callable from const method which returns nothing and receives 12 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11) const)
{
    return new ConstVoidPtrMethod12<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>(f);
}

/*! Makes callable from const method which returns nothing and receives 13 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12) const)
{
    return new ConstVoidPtrMethod13<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>(f);
}

/*! Makes callable from const method which returns nothing and receives 14 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13) const)
{
    return new ConstVoidPtrMethod14<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>(f);
}

/*! Makes callable from const method which returns nothing and receives 15 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14) const)
{
    return new ConstVoidPtrMethod15<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>(f);
}

/*! Makes callable from const method which returns nothing and receives 16 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(void (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15) const)
{
    return new ConstVoidPtrMethod16<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>(f);
}


/*! Makes callable from const method which returns value and receives 0 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType
>
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)() const)
{
    return new ConstRetPtrMethod0<_ClassName, _ReturnType>(f);
}

/*! Makes callable from const method which returns value and receives 1 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0
>
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0) const)
{
    return new ConstRetPtrMethod1<_ClassName, _ReturnType, _Arg0>(f);
}

/*! Makes callable from const method which returns value and receives 2 arguments
    \param[in] f method
    \return callable version
 */
template<
    typename _ClassName,
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1
>
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1) const)
{
    return new ConstRetPtrMethod2<_ClassName, _ReturnType, _Arg0, _Arg1>(f);
}

/*! Makes callable from const method which returns value and receives 3 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2) const)
{
    return new ConstRetPtrMethod3<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2>(f);
}

/*! Makes callable from const method which returns value and receives 4 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3) const)
{
    return new ConstRetPtrMethod4<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3>(f);
}

/*! Makes callable from const method which returns value and receives 5 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4) const)
{
    return new ConstRetPtrMethod5<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>(f);
}

/*! Makes callable from const method which returns value and receives 6 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5) const)
{
    return new ConstRetPtrMethod6<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>(f);
}

/*! Makes callable from const method which returns value and receives 7 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6) const)
{
    return new ConstRetPtrMethod7<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>(f);
}

/*! Makes callable from const method which returns value and receives 8 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7) const)
{
    return new ConstRetPtrMethod8<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>(f);
}

/*! Makes callable from const method which returns value and receives 9 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8) const)
{
    return new ConstRetPtrMethod9<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>(f);
}

/*! Makes callable from const method which returns value and receives 10 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9) const)
{
    return new ConstRetPtrMethod10<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>(f);
}

/*! Makes callable from const method which returns value and receives 11 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10) const)
{
    return new ConstRetPtrMethod11<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>(f);
}

/*! Makes callable from const method which returns value and receives 12 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11) const)
{
    return new ConstRetPtrMethod12<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>(f);
}

/*! Makes callable from const method which returns value and receives 13 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12) const)
{
    return new ConstRetPtrMethod13<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>(f);
}

/*! Makes callable from const method which returns value and receives 14 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13) const)
{
    return new ConstRetPtrMethod14<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>(f);
}

/*! Makes callable from const method which returns value and receives 15 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14) const)
{
    return new ConstRetPtrMethod15<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>(f);
}

/*! Makes callable from const method which returns value and receives 16 arguments
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
inline sad::duktape::DuktapeCallable* make_method_ptr_callable(_ReturnType (_ClassName::*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15) const)
{
    return new ConstRetPtrMethod16<_ClassName, _ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>(f);
}


}

}