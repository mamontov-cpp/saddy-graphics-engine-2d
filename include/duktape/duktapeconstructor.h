/*! \file duktapeconstructor.h
    \author HiddenSeeker
     
    Defines a simple defines for creating bindings for creating new objects
 */
#pragma once
#include "duktapecallable.h"
#include "decay.h"
#include "getvalue.h"
#include "pushvalue.h"
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
class Constructor0 : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor0()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor0<_ClassName>();
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
            _ClassName  t;
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor0()
    {
    }   
};

/*! Defines a wrapper for method, which returns nothing and receives 1 arguments
 */
template<
    typename _ClassName,
    typename _Arg0
>
class Constructor1 : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor1()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor1<_ClassName, _Arg0>();
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
            _ClassName  t(_a0.mutableValue());
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor1()
    {
    }   
};

/*! Defines a wrapper for method, which returns nothing and receives 2 arguments
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1
>
class Constructor2 : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor2()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor2<_ClassName, _Arg0, _Arg1>();
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
            _ClassName  t(_a0.mutableValue(), _a1.mutableValue());
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor2()
    {
    }   
};

/*! Defines a wrapper for method, which returns nothing and receives 3 arguments
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2
>
class Constructor3 : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor3()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor3<_ClassName, _Arg0, _Arg1, _Arg2>();
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
            _ClassName  t(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue());
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor3()
    {
    }   
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
class Constructor4 : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor4()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor4<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3>();
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
            _ClassName  t(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue());
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor4()
    {
    }   
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
class Constructor5 : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor5()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor5<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>();
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
            _ClassName  t(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue());
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor5()
    {
    }   
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
class Constructor6 : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor6()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor6<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>();
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
            _ClassName  t(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue());
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor6()
    {
    }   
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
class Constructor7 : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor7()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor7<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>();
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
            _ClassName  t(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue());
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor7()
    {
    }   
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
class Constructor8 : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor8()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor8<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>();
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
            _ClassName  t(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue());
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor8()
    {
    }   
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
class Constructor9 : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor9()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor9<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>();
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
            _ClassName  t(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue());
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor9()
    {
    }   
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
class Constructor10 : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor10()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor10<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>();
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
            _ClassName  t(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue());
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor10()
    {
    }   
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
class Constructor11 : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor11()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor11<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>();
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
            _ClassName  t(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue());
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor11()
    {
    }   
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
class Constructor12 : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor12()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor12<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>();
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
            _ClassName  t(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue());
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor12()
    {
    }   
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
class Constructor13 : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor13()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor13<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>();
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
            _ClassName  t(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue());
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor13()
    {
    }   
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
class Constructor14 : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor14()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor14<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>();
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
            _ClassName  t(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue());
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor14()
    {
    }   
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
class Constructor15 : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor15()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor15<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>();
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
            _ClassName  t(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue());
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor15()
    {
    }   
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
class Constructor16 : public sad::duktape::DuktapeCallable
{
public:
    /*! Constructs new object
     */
    Constructor16()
    {
    }
    /*! Returns copy of callable object 
        \return copy of callable object
     */
    virtual sad::duktape::DuktapeCallable* clone()
    {
        return new sad::duktape::Constructor16<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>();
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
            _ClassName  t(_a0.mutableValue(), _a1.mutableValue(), _a2.mutableValue(), _a3.mutableValue(), _a4.mutableValue(), _a5.mutableValue(), _a6.mutableValue(), _a7.mutableValue(), _a8.mutableValue(), _a9.mutableValue(), _a10.mutableValue(), _a11.mutableValue(), _a12.mutableValue(), _a13.mutableValue(), _a14.mutableValue(), _a15.mutableValue());
            sad::duktape::PushValue<_ClassName>::perform(c, t, false);
        }
        catch(...)
        {
            c->throwError("Caught exception while calling function");
            return 0;
        }
        return 1;
    }
    /*! Can be inherited
     */
    virtual ~Constructor16()
    {
    }   
};

/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
 */
template<
    typename _ClassName
>
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor0<_ClassName>();
    ctx->registerCallable(prop, c);
}

/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
 */
template<
    typename _ClassName,
    typename _Arg0
>
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor1<_ClassName, _Arg0>();
    ctx->registerCallable(prop, c);
}

/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1
>
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor2<_ClassName, _Arg0, _Arg1>();
    ctx->registerCallable(prop, c);
}

/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2
>
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor3<_ClassName, _Arg0, _Arg1, _Arg2>();
    ctx->registerCallable(prop, c);
}

/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3
>
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor4<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3>();
    ctx->registerCallable(prop, c);
}

/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
 */
template<
    typename _ClassName,
    typename _Arg0,
    typename _Arg1,
    typename _Arg2,
    typename _Arg3,
    typename _Arg4
>
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor5<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>();
    ctx->registerCallable(prop, c);
}

/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
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
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor6<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>();
    ctx->registerCallable(prop, c);
}

/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
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
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor7<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>();
    ctx->registerCallable(prop, c);
}

/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
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
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor8<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>();
    ctx->registerCallable(prop, c);
}

/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
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
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor9<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>();
    ctx->registerCallable(prop, c);
}

/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
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
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor10<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>();
    ctx->registerCallable(prop, c);
}

/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
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
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor11<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>();
    ctx->registerCallable(prop, c);
}

/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
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
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor12<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>();
    ctx->registerCallable(prop, c);
}

/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
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
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor13<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>();
    ctx->registerCallable(prop, c);
}

/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
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
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor14<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>();
    ctx->registerCallable(prop, c);
}

/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
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
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor15<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>();
    ctx->registerCallable(prop, c);
}

/*! Register constructor as global function of context
    \param[in] ctx context
    \param[in] prop property name
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
inline void register_constructor(sad::duktape::Context* ctx, const sad::String& prop)
{
    sad::duktape::DuktapeCallable* c = new Constructor16<_ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>();
    ctx->registerCallable(prop, c);
}


}

}
