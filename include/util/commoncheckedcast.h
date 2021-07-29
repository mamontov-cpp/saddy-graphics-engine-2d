/*! \file commoncheckedcast.h
    

    Defines a common checked cast for solving compile-time
    problems with checked_cast if we trying to cast to POD.
 */
#pragma once
#include "../maybe.h"
#include "../sadstring.h"
#include "../classmetadatacontainer.h"
#include "../db/dbtypename.h"

namespace sad
{

namespace db
{

class Object;    
    
}

class Object;
template<typename _Dest, typename _Src> 
_Dest * checked_cast(_Src * arg);

namespace util
{

/*! Defines a common checked cast for solving compile-time
    problems with checked_cast if we trying to cast to POD.
 */
template<typename T, bool isSadObject>
class CommonCheckedCast
{
public:
    /*! Does nothing, since we don't have a conversion in common cast
        \param[out] v converted value
        \param[in] o object
        \param[in] _typename a typename
     */
    static void perform(Maybe<T> & v, void * o, const sad::String & _typename)
    {
        
    }
};

/*! Defines a common checked cast case for solving problems with casting to a type.
 */
template<typename T>
class CommonCheckedCast<T, true>
{
public:
    /*! Does nothing, since we don't have a conversion in common cast
        \param[out] result converted value
        \param[in] object a converted object
        \param[in] _typename a typename
     */
    static void perform(Maybe<T> & result, void * object, const sad::String & _typename)
    {

    }
};

/*! Defines a common checked cast case for solving problems with casting to a type.
 */
template<typename T>
class CommonCheckedCast<T*, true>
{
public:
    /*! Does nothing, since we don't have a conversion in common cast
        \param[out] result converted value
        \param[in] object object to be converted
        \param[in] _typename a typename
     */
    static void perform(Maybe<T*> & result, void * object, const sad::String & _typename)
    {
        bool created = false;
        sad::db::TypeName<T>::init();
        if (sad::ClassMetaDataContainer::ref()->get(_typename, created)->canBeCastedTo(sad::db::TypeName<T>::name()) == true)
        {
            sad::Object ** o = static_cast<sad::Object**>(object);
            try 
            {
                result.setValue(sad::checked_cast<T, sad::Object>(*o));
            } 
            catch (...)
            {

            }
        }
    }
};


template<typename T>
class SadDBObjectCast
{
public:
    /*! Does nothing, since we don't have a conversion in common cast
        \param[out] result converted value
        \param[in] object object to be converted
     */
    inline static void perform(Maybe<T> & result, void * object)
    {
        
    }
};

template<>
class SadDBObjectCast<sad::db::Object*>
{
public:
    /*! Does nothing, since we don't have a conversion in common cast
        \param[out] result converted value
        \param[in] object object to be converted
     */
    inline static void perform(Maybe<sad::db::Object*> & result, void * object)
    {
        sad::Object ** o = static_cast<sad::Object**>(object);
        result.setValue(reinterpret_cast<sad::db::Object*>(*o));
    }
};

}

}