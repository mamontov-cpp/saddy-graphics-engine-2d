/*! \file duktapefunction.h
    \author HiddenSeeker
     
    Defines a simple defines for creating bindings for any kind of function
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

/*! Defines a wrapper for function, which returns nothing and receives 0 arguments
 */
class VoidFunction0 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)();
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction0(Function f);    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone();  
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments();  
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c);
    /*! Can be inherited
     */
    virtual ~VoidFunction0();
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns nothing and receives 1 arguments
 */
template<
    typename _Arg0
>
class VoidFunction1 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)(_Arg0);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction1(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::VoidFunction1<_Arg0>(m_callee);
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
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
      
        if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }
        
        try
        {
            m_callee(_a0.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidFunction1()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns nothing and receives 2 arguments
 */
template<
    typename _Arg0,
    typename _Arg1
>
class VoidFunction2 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)(_Arg0,_Arg1);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction2(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::VoidFunction2<_Arg0,_Arg1>(m_callee);
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
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
      
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
            m_callee(_a0.mutableValue(), _a1.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidFunction2()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns nothing and receives 3 arguments
 */
template<
    typename _Arg0,
    typename _Arg1,
    typename _Arg2
>
class VoidFunction3 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)(_Arg0,_Arg1,_Arg2);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction3(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::VoidFunction3<_Arg0,_Arg1,_Arg2>(m_callee);
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
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
      
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
            m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidFunction3()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns nothing and receives 4 arguments
 */
template<
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3
>
class VoidFunction4 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)(_Arg0,_Arg1,_Arg2,_Arg3);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction4(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::VoidFunction4<_Arg0,_Arg1,_Arg2,_Arg3>(m_callee);
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
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
      
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
            m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidFunction4()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns nothing and receives 5 arguments
 */
template<
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4
>
class VoidFunction5 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction5(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::VoidFunction5<_Arg0,_Arg1,_Arg2,_Arg3,_Arg4>(m_callee);
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
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
      
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
            m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidFunction5()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns nothing and receives 6 arguments
 */
template<
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5
>
class VoidFunction6 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction6(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::VoidFunction6<_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5>(m_callee);
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
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
      
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
            m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidFunction6()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns nothing and receives 7 arguments
 */
template<
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5,
    typename _Arg6
>
class VoidFunction7 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction7(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::VoidFunction7<_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6>(m_callee);
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
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 6);
      
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
            m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidFunction7()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns nothing and receives 8 arguments
 */
template<
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5,
    typename _Arg6,
    typename _Arg7
>
class VoidFunction8 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction8(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::VoidFunction8<_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7>(m_callee);
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
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 7);
      
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
            m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidFunction8()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns nothing and receives 9 arguments
 */
template<
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
class VoidFunction9 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction9(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::VoidFunction9<_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8>(m_callee);
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
        sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
        sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
        sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 6);
        sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 7);
        sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 8);
      
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
            m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidFunction9()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns nothing and receives 10 arguments
 */
template<
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
class VoidFunction10 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction10(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::VoidFunction10<_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9>(m_callee);
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
            m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidFunction10()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns nothing and receives 11 arguments
 */
template<
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
class VoidFunction11 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction11(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::VoidFunction11<_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10>(m_callee);
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
            m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidFunction11()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns nothing and receives 12 arguments
 */
template<
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
class VoidFunction12 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction12(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::VoidFunction12<_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11>(m_callee);
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
            m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidFunction12()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns nothing and receives 13 arguments
 */
template<
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
class VoidFunction13 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction13(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::VoidFunction13<_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12>(m_callee);
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
            m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidFunction13()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns nothing and receives 14 arguments
 */
template<
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
class VoidFunction14 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction14(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::VoidFunction14<_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13>(m_callee);
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
            m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidFunction14()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns nothing and receives 15 arguments
 */
template<
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
class VoidFunction15 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13,_Arg14);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction15(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::VoidFunction15<_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13,_Arg14>(m_callee);
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
            m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidFunction15()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns nothing and receives 16 arguments
 */
template<
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
class VoidFunction16 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef void (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13,_Arg14,_Arg15);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    VoidFunction16(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::VoidFunction16<_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13,_Arg14,_Arg15>(m_callee);
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
            m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue(), _a15.mutableValue());
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~VoidFunction16()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns some value and receives 0 arguments
 */
template<
    typename _ReturnType
>
class RetFunction0 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)();
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction0(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction0<_ReturnType>(m_callee);
    }
    /*! Returns count of required arguments
        \return count of required arguments
     */
    virtual int requiredArguments()  
    {
        return 0;
    }   
    /*! Performs call of object, using specified context
        \param[in] c context
        \return count of values on stack, placed by functions
     */
    virtual int call(sad::duktape::Context* c)
    {
       try
        {
            _ReturnType t = m_callee();
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction0()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns some value and receives 1 arguments
 */
template<
    typename _ReturnType,
    typename _Arg0
>
class RetFunction1 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)(_Arg0);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction1(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction1<_ReturnType,_Arg0>(m_callee);
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
      sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
        
       if (_a0.exists() == false) 
        {
            sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::init();
            c->throwError("Invalid type passed for argument 1. Argument 1 must have type " + sad::db::TypeName< typename sad::duktape::Decay<_Arg0>::Type >::name(), sad::duktape::SAD_DUK_API_ERROR);
            return 0;
        }

       try
        {
            _ReturnType t = m_callee(_a0.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction1()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns some value and receives 2 arguments
 */
template<
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1
>
class RetFunction2 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)(_Arg0,_Arg1);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction2(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction2<_ReturnType,_Arg0,_Arg1>(m_callee);
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
      sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
      sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
        
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
            _ReturnType t = m_callee(_a0.mutableValue(), _a1.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction2()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns some value and receives 3 arguments
 */
template<
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2
>
class RetFunction3 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)(_Arg0,_Arg1,_Arg2);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction3(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction3<_ReturnType,_Arg0,_Arg1,_Arg2>(m_callee);
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
      sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
      sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
      sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
        
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
            _ReturnType t = m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction3()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns some value and receives 4 arguments
 */
template<
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3
>
class RetFunction4 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)(_Arg0,_Arg1,_Arg2,_Arg3);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction4(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction4<_ReturnType,_Arg0,_Arg1,_Arg2,_Arg3>(m_callee);
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
      sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
      sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
      sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
      sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
        
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
            _ReturnType t = m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction4()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns some value and receives 5 arguments
 */
template<
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4
>
class RetFunction5 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction5(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction5<_ReturnType,_Arg0,_Arg1,_Arg2,_Arg3,_Arg4>(m_callee);
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
      sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
      sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
      sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
      sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
      sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
        
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
            _ReturnType t = m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction5()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns some value and receives 6 arguments
 */
template<
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5
>
class RetFunction6 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction6(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction6<_ReturnType,_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5>(m_callee);
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
      sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
      sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
      sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
      sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
      sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
      sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
        
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
            _ReturnType t = m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction6()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns some value and receives 7 arguments
 */
template<
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5,
    typename _Arg6
>
class RetFunction7 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction7(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction7<_ReturnType,_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6>(m_callee);
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
      sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
      sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
      sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
      sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
      sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
      sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
      sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 6);
        
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
            _ReturnType t = m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction7()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns some value and receives 8 arguments
 */
template<
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
class RetFunction8 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction8(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction8<_ReturnType,_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7>(m_callee);
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
      sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
      sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
      sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
      sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
      sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
      sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
      sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 6);
      sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 7);
        
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
            _ReturnType t = m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction8()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns some value and receives 9 arguments
 */
template<
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
class RetFunction9 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction9(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction9<_ReturnType,_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8>(m_callee);
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
      sad::Maybe< typename sad::duktape::Decay<_Arg0>::Type > _a0 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg0>::Type >::perform(c, 0);
      sad::Maybe< typename sad::duktape::Decay<_Arg1>::Type > _a1 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg1>::Type >::perform(c, 1);
      sad::Maybe< typename sad::duktape::Decay<_Arg2>::Type > _a2 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg2>::Type >::perform(c, 2);
      sad::Maybe< typename sad::duktape::Decay<_Arg3>::Type > _a3 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg3>::Type >::perform(c, 3);
      sad::Maybe< typename sad::duktape::Decay<_Arg4>::Type > _a4 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg4>::Type >::perform(c, 4);
      sad::Maybe< typename sad::duktape::Decay<_Arg5>::Type > _a5 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg5>::Type >::perform(c, 5);
      sad::Maybe< typename sad::duktape::Decay<_Arg6>::Type > _a6 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg6>::Type >::perform(c, 6);
      sad::Maybe< typename sad::duktape::Decay<_Arg7>::Type > _a7 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg7>::Type >::perform(c, 7);
      sad::Maybe< typename sad::duktape::Decay<_Arg8>::Type > _a8 = sad::duktape::GetValue< typename sad::duktape::Decay<_Arg8>::Type >::perform(c, 8);
        
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
            _ReturnType t = m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction9()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns some value and receives 10 arguments
 */
template<
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
class RetFunction10 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction10(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction10<_ReturnType,_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9>(m_callee);
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
            _ReturnType t = m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction10()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns some value and receives 11 arguments
 */
template<
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
class RetFunction11 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction11(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction11<_ReturnType,_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10>(m_callee);
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
            _ReturnType t = m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction11()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns some value and receives 12 arguments
 */
template<
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
class RetFunction12 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction12(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction12<_ReturnType,_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11>(m_callee);
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
            _ReturnType t = m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction12()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns some value and receives 13 arguments
 */
template<
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
class RetFunction13 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction13(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction13<_ReturnType,_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12>(m_callee);
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
            _ReturnType t = m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction13()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns some value and receives 14 arguments
 */
template<
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
class RetFunction14 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction14(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction14<_ReturnType,_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13>(m_callee);
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
            _ReturnType t = m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction14()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns some value and receives 15 arguments
 */
template<
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
class RetFunction15 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13,_Arg14);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction15(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction15<_ReturnType,_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13,_Arg14>(m_callee);
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
            _ReturnType t = m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction15()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Defines a wrapper for function, which returns some value and receives 16 arguments
 */
template<
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
class RetFunction16 : public sad::duktape::DuktapeCallable
{
public:
    /*! A function type, which is being wrapped
     */
    typedef _ReturnType (*Function)(_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13,_Arg14,_Arg15);
    /*! Constructs new function wrapper
        \param[in] f function
     */
    RetFunction16(Function f): m_callee(f)   
    {
    }
    /*! Returns copy of callable object
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()  
    {
        return new sad::duktape::RetFunction16<_ReturnType,_Arg0,_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13,_Arg14,_Arg15>(m_callee);
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
            _ReturnType t = m_callee(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue(), _a15.mutableValue());
            sad::duktape::PushValue<_ReturnType>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 0;
    }
    /*! Can be inherited
     */
    virtual ~RetFunction16()
    {
    }
protected:
    /*! A function, which is being wrapped
     */
    Function m_callee;
};

/*! Makes callable from function which returns nothing and receives 0 arguments
    \param[in] f function
    \return callable version
 */
inline sad::duktape::DuktapeCallable* make_callable(void (*f)())
{
    return new VoidFunction0(f);
}

/*! Makes callable from function which returns nothing and receives 1 arguments
    \param[in] f function
    \return callable version
 */
template<
    typename _Arg0
>
inline sad::duktape::DuktapeCallable* make_callable(void (*f)(_Arg0))
{
    return new VoidFunction1<_Arg0>(f);
}

/*! Makes callable from function which returns nothing and receives 2 arguments
    \param[in] f function
    \return callable version
 */
template<
    typename _Arg0,
    typename _Arg1
>
inline sad::duktape::DuktapeCallable* make_callable(void (*f)(_Arg0, _Arg1))
{
    return new VoidFunction2<_Arg0, _Arg1>(f);
}

/*! Makes callable from function which returns nothing and receives 3 arguments
    \param[in] f function
    \return callable version
 */
template<
    typename _Arg0,
    typename _Arg1,
    typename _Arg2
>
inline sad::duktape::DuktapeCallable* make_callable(void (*f)(_Arg0, _Arg1, _Arg2))
{
    return new VoidFunction3<_Arg0, _Arg1, _Arg2>(f);
}

/*! Makes callable from function which returns nothing and receives 4 arguments
    \param[in] f function
    \return callable version
 */
template<
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3
>
inline sad::duktape::DuktapeCallable* make_callable(void (*f)(_Arg0, _Arg1, _Arg2, _Arg3))
{
    return new VoidFunction4<_Arg0, _Arg1, _Arg2, _Arg3>(f);
}

/*! Makes callable from function which returns nothing and receives 5 arguments
    \param[in] f function
    \return callable version
 */
template<
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4
>
inline sad::duktape::DuktapeCallable* make_callable(void (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4))
{
    return new VoidFunction5<_Arg0, _Arg1, _Arg2, _Arg3, _Arg4>(f);
}

/*! Makes callable from function which returns nothing and receives 6 arguments
    \param[in] f function
    \return callable version
 */
template<
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5
>
inline sad::duktape::DuktapeCallable* make_callable(void (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5))
{
    return new VoidFunction6<_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>(f);
}

/*! Makes callable from function which returns nothing and receives 7 arguments
    \param[in] f function
    \return callable version
 */
template<
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5,
    typename _Arg6
>
inline sad::duktape::DuktapeCallable* make_callable(void (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6))
{
    return new VoidFunction7<_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>(f);
}

/*! Makes callable from function which returns nothing and receives 8 arguments
    \param[in] f function
    \return callable version
 */
template<
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5,
    typename _Arg6,
    typename _Arg7
>
inline sad::duktape::DuktapeCallable* make_callable(void (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7))
{
    return new VoidFunction8<_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>(f);
}

/*! Makes callable from function which returns nothing and receives 9 arguments
    \param[in] f function
    \return callable version
 */
template<
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
inline sad::duktape::DuktapeCallable* make_callable(void (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8))
{
    return new VoidFunction9<_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>(f);
}

/*! Makes callable from function which returns nothing and receives 10 arguments
    \param[in] f function
    \return callable version
 */
template<
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
inline sad::duktape::DuktapeCallable* make_callable(void (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9))
{
    return new VoidFunction10<_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>(f);
}

/*! Makes callable from function which returns nothing and receives 11 arguments
    \param[in] f function
    \return callable version
 */
template<
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
inline sad::duktape::DuktapeCallable* make_callable(void (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10))
{
    return new VoidFunction11<_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>(f);
}

/*! Makes callable from function which returns nothing and receives 12 arguments
    \param[in] f function
    \return callable version
 */
template<
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
inline sad::duktape::DuktapeCallable* make_callable(void (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11))
{
    return new VoidFunction12<_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>(f);
}

/*! Makes callable from function which returns nothing and receives 13 arguments
    \param[in] f function
    \return callable version
 */
template<
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
inline sad::duktape::DuktapeCallable* make_callable(void (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12))
{
    return new VoidFunction13<_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>(f);
}

/*! Makes callable from function which returns nothing and receives 14 arguments
    \param[in] f function
    \return callable version
 */
template<
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
inline sad::duktape::DuktapeCallable* make_callable(void (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13))
{
    return new VoidFunction14<_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>(f);
}

/*! Makes callable from function which returns nothing and receives 15 arguments
    \param[in] f function
    \return callable version
 */
template<
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
inline sad::duktape::DuktapeCallable* make_callable(void (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14))
{
    return new VoidFunction15<_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>(f);
}

/*! Makes callable from function which returns nothing and receives 16 arguments
    \param[in] f function
    \return callable version
 */
template<
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
inline sad::duktape::DuktapeCallable* make_callable(void (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15))
{
    return new VoidFunction16<_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>(f);
}


/*! Makes callable from function which returns value and receives 0 arguments
    \param[in] f function
    \return callable version
 */
template<
    typename _ReturnType
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)())
{
    return new RetFunction0<_ReturnType>(f);
}

/*! Makes callable from function which returns value and receives 1 arguments
    \param[in] f function
    \return callable version
 */
template<
    typename _ReturnType,
    typename _Arg0
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)(_Arg0))
{
    return new RetFunction1<_ReturnType, _Arg0>(f);
}

/*! Makes callable from function which returns value and receives 2 arguments
    \param[in] f function
    \return callable version
 */
template<
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)(_Arg0, _Arg1))
{
    return new RetFunction2<_ReturnType, _Arg0, _Arg1>(f);
}

/*! Makes callable from function which returns value and receives 3 arguments
    \param[in] f function
    \return callable version
 */
template<
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)(_Arg0, _Arg1, _Arg2))
{
    return new RetFunction3<_ReturnType, _Arg0, _Arg1, _Arg2>(f);
}

/*! Makes callable from function which returns value and receives 4 arguments
    \param[in] f function
    \return callable version
 */
template<
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)(_Arg0, _Arg1, _Arg2, _Arg3))
{
    return new RetFunction4<_ReturnType, _Arg0, _Arg1, _Arg2, _Arg3>(f);
}

/*! Makes callable from function which returns value and receives 5 arguments
    \param[in] f function
    \return callable version
 */
template<
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4))
{
    return new RetFunction5<_ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>(f);
}

/*! Makes callable from function which returns value and receives 6 arguments
    \param[in] f function
    \return callable version
 */
template<
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5))
{
    return new RetFunction6<_ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>(f);
}

/*! Makes callable from function which returns value and receives 7 arguments
    \param[in] f function
    \return callable version
 */
template<
    typename _ReturnType,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4,
    typename _Arg5,
    typename _Arg6
>
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6))
{
    return new RetFunction7<_ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>(f);
}

/*! Makes callable from function which returns value and receives 8 arguments
    \param[in] f function
    \return callable version
 */
template<
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
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7))
{
    return new RetFunction8<_ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>(f);
}

/*! Makes callable from function which returns value and receives 9 arguments
    \param[in] f function
    \return callable version
 */
template<
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
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8))
{
    return new RetFunction9<_ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>(f);
}

/*! Makes callable from function which returns value and receives 10 arguments
    \param[in] f function
    \return callable version
 */
template<
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
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9))
{
    return new RetFunction10<_ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>(f);
}

/*! Makes callable from function which returns value and receives 11 arguments
    \param[in] f function
    \return callable version
 */
template<
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
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10))
{
    return new RetFunction11<_ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>(f);
}

/*! Makes callable from function which returns value and receives 12 arguments
    \param[in] f function
    \return callable version
 */
template<
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
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11))
{
    return new RetFunction12<_ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>(f);
}

/*! Makes callable from function which returns value and receives 13 arguments
    \param[in] f function
    \return callable version
 */
template<
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
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12))
{
    return new RetFunction13<_ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>(f);
}

/*! Makes callable from function which returns value and receives 14 arguments
    \param[in] f function
    \return callable version
 */
template<
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
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13))
{
    return new RetFunction14<_ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>(f);
}

/*! Makes callable from function which returns value and receives 15 arguments
    \param[in] f function
    \return callable version
 */
template<
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
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14))
{
    return new RetFunction15<_ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>(f);
}

/*! Makes callable from function which returns value and receives 16 arguments
    \param[in] f function
    \return callable version
 */
template<
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
inline sad::duktape::DuktapeCallable* make_callable(_ReturnType (*f)(_Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15))
{
    return new RetFunction16<_ReturnType, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>(f);
}

}

}