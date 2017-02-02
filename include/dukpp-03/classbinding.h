/*! \file classbinding.h
    
    Defines a class binding with support for sad::db::Object-style data
 */
#pragma once
#include "basiccontext.h"
#include "../db/schema/schema.h"
#include "ptrconstructor.h"

namespace sad
{
    
namespace dukpp03
{

/*! A new class binding with objects
 */    
class ClassBinding: public ::dukpp03::ClassBinding<sad::dukpp03::BasicContext>
{
public:
    /*! Makes a default binding
     */
    ClassBinding();
    /*! Could be inherited
     */
    virtual ~ClassBinding();
    /*! Registers this class binding as QObject descendant, filling it
        it with metamethods
     */
    template<typename T>
    void registerAsObjectWithSchema()
    {
        registerSchema(T::basicSchema());
    }
    /*! Registers this class binding as sad::db::Object descendant with schema. Schema properties
        are registered as accessors
        \param[in] schema a schema
     */
    void registerSchema(const sad::db::schema::Schema* schema);
    /*! Adds new constructor for specified class
        \param[in] name a name for it
        \return reference to constructor
     */
    template<
        typename _ClassName
    >
    ::dukpp03::Callable<sad::dukpp03::BasicContext>* addObjectConstructor(const std::string& name)
    {
        return addConstructor(name, new ::dukpp03::PtrConstructor0<sad::dukpp03::BasicContext, _ClassName>());
    }
    /*! Adds new constructor for specified class
        \param[in] name a name for it
        \return reference to constructor
     */
    template<
        typename _ClassName,
        typename _Arg0
    >
    ::dukpp03::Callable<sad::dukpp03::BasicContext>* addObjectConstructor(const std::string& name)
    {
        return addConstructor(name, new ::dukpp03::PtrConstructor1<sad::dukpp03::BasicContext, _ClassName, _Arg0>());
    }
    /*! Adds new constructor for specified class
        \param[in] name a name for it
        \return reference to constructor
     */
    template<
        typename _ClassName,
        typename _Arg0,
        typename _Arg1
    >
    ::dukpp03::Callable<sad::dukpp03::BasicContext>* addObjectConstructor(const std::string& name)
    {
        return addConstructor(name, new ::dukpp03::PtrConstructor2<sad::dukpp03::BasicContext, _ClassName, _Arg0, _Arg1>());
    }
    /*! Adds new constructor for specified class
        \param[in] name a name for it
        \return reference to constructor
     */
    template<
        typename _ClassName,
        typename _Arg0,
        typename _Arg1,
        typename _Arg2
    >
    ::dukpp03::Callable<sad::dukpp03::BasicContext>* addObjectConstructor(const std::string& name)
    {
        return addConstructor(name, new ::dukpp03::PtrConstructor3<sad::dukpp03::BasicContext, _ClassName, _Arg0, _Arg1, _Arg2>());
    }
    /*! Adds new constructor for specified class
        \param[in] name a name for it
        \return reference to constructor
     */
    template<
        typename _ClassName,
        typename _Arg0,
        typename _Arg1,
        typename _Arg2,
        typename _Arg3
    >
    ::dukpp03::Callable<sad::dukpp03::BasicContext>* addObjectConstructor(const std::string& name)
    {
        return addConstructor(name, new ::dukpp03::PtrConstructor4<sad::dukpp03::BasicContext, _ClassName, _Arg0, _Arg1, _Arg2, _Arg3>());
    }
    /*! Adds new constructor for specified class
        \param[in] name a name for it
        \return reference to constructor
     */
    template<
        typename _ClassName,
        typename _Arg0,
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4
    >
    ::dukpp03::Callable<sad::dukpp03::BasicContext>* addObjectConstructor(const std::string& name)
    {
        return addConstructor(name, new ::dukpp03::PtrConstructor5<sad::dukpp03::BasicContext, _ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4>());
    }
    /*! Adds new constructor for specified class
        \param[in] name a name for it
        \return reference to constructor
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
    ::dukpp03::Callable<sad::dukpp03::BasicContext>* addObjectConstructor(const std::string& name)
    {
        return addConstructor(name, new ::dukpp03::PtrConstructor6<sad::dukpp03::BasicContext, _ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5>());
    }
    /*! Adds new constructor for specified class
        \param[in] name a name for it
        \return reference to constructor
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
    ::dukpp03::Callable<sad::dukpp03::BasicContext>* addObjectConstructor(const std::string& name)
    {
        return addConstructor(name, new ::dukpp03::PtrConstructor7<sad::dukpp03::BasicContext, _ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6>());
    }
    /*! Adds new constructor for specified class
        \param[in] name a name for it
        \return reference to constructor
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
    ::dukpp03::Callable<sad::dukpp03::BasicContext>* addObjectConstructor(const std::string& name)
    {
        return addConstructor(name, new ::dukpp03::PtrConstructor8<sad::dukpp03::BasicContext, _ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7>());
    }
    /*! Adds new constructor for specified class
        \param[in] name a name for it
        \return reference to constructor
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
    ::dukpp03::Callable<sad::dukpp03::BasicContext>* addObjectConstructor(const std::string& name)
    {
        return addConstructor(name, new ::dukpp03::PtrConstructor9<sad::dukpp03::BasicContext, _ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8>());
    }
    /*! Adds new constructor for specified class
        \param[in] name a name for it
        \return reference to constructor
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
    ::dukpp03::Callable<sad::dukpp03::BasicContext>* addObjectConstructor(const std::string& name)
    {
        return addConstructor(name, new ::dukpp03::PtrConstructor10<sad::dukpp03::BasicContext, _ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9>());
    }
    /*! Adds new constructor for specified class
        \param[in] name a name for it
        \return reference to constructor
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
    ::dukpp03::Callable<sad::dukpp03::BasicContext>* addObjectConstructor(const std::string& name)
    {
        return addConstructor(name, new ::dukpp03::PtrConstructor11<sad::dukpp03::BasicContext, _ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10>());
    }
    /*! Adds new constructor for specified class
        \param[in] name a name for it
        \return reference to constructor
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
    ::dukpp03::Callable<sad::dukpp03::BasicContext>* addObjectConstructor(const std::string& name)
    {
        return addConstructor(name, new ::dukpp03::PtrConstructor12<sad::dukpp03::BasicContext, _ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11>());
    }
    /*! Adds new constructor for specified class
        \param[in] name a name for it
        \return reference to constructor
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
    ::dukpp03::Callable<sad::dukpp03::BasicContext>* addObjectConstructor(const std::string& name)
    {
        return addConstructor(name, new ::dukpp03::PtrConstructor13<sad::dukpp03::BasicContext, _ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12>());
    }
    /*! Adds new constructor for specified class
        \param[in] name a name for it
        \return reference to constructor
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
    ::dukpp03::Callable<sad::dukpp03::BasicContext>* addObjectConstructor(const std::string& name)
    {
        return addConstructor(name, new ::dukpp03::PtrConstructor14<sad::dukpp03::BasicContext, _ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13>());
    }
    /*! Adds new constructor for specified class
        \param[in] name a name for it
        \return reference to constructor
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
    ::dukpp03::Callable<sad::dukpp03::BasicContext>* addObjectConstructor(const std::string& name)
    {
        return addConstructor(name, new ::dukpp03::PtrConstructor15<sad::dukpp03::BasicContext, _ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14>());
    }
    /*! Adds new constructor for specified class
        \param[in] name a name for it
        \return reference to constructor
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
    ::dukpp03::Callable<sad::dukpp03::BasicContext>* addObjectConstructor(const std::string& name)
    {
        return addConstructor(name, new ::dukpp03::PtrConstructor16<sad::dukpp03::BasicContext, _ClassName, _Arg0, _Arg1, _Arg2, _Arg3, _Arg4, _Arg5, _Arg6, _Arg7, _Arg8, _Arg9, _Arg10, _Arg11, _Arg12, _Arg13, _Arg14, _Arg15>());
    }
};

}

}
