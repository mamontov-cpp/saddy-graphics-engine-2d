/*! \file getaddressoftype.h
    
    Gets address of specified type
 */
#pragma once
#include "../db/dbvariant.h"
#include "../3rdparty/dukpp-03.h"

namespace sad
{

namespace dukpp03
{

namespace internal
{
/*! A local conversion table for all conversions
 */
extern sad::db::ConversionTable conversion_table;

}


/*! Gets adress fo value of specified type, that is stored in variant.
    Here we try to get plain value (not a pointer type) as and address, so we don't allow such behaviour
 */
template<typename _UnderlyingValue, bool isPointerToAbstractClass>
struct GetAddressOfType
{
public: 
    /*! Returns address of type, stored in variant - hence nothing for plain types
        \param[in] v value
        \return empty maybe
     */
    static ::dukpp03::Maybe<_UnderlyingValue> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<_UnderlyingValue>();
    }
};

/*! Gets adress fo value of specified type, that is stored in variant.
    Here we try to get plain value (not a pointer type) as and address, so we don't allow such behaviour
 */
template<typename _UnderlyingValue>
struct GetAddressOfType<_UnderlyingValue, true>
{
public: 
    /*! Returns address of type, stored in variant - hence nothing for plain types
        \param[in] v value
        \return empty maybe
     */
    static ::dukpp03::Maybe<_UnderlyingValue> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<_UnderlyingValue>();
    }
};

/*! Returns address of type stored in variant.
 */
template<typename _UnderlyingValue>
struct GetAddressOfType<_UnderlyingValue*, false>
{
public: 
    /*! Returns address of type, stored in variant.
        \param[in] v value
        \return empty maybe
     */
    static ::dukpp03::Maybe<_UnderlyingValue*> getAddress(sad::db::Variant* v)
    {
        sad::Maybe<_UnderlyingValue> result = v->get<_UnderlyingValue>(true, &sad::dukpp03::internal::conversion_table);
        if (result.exists())
        {
            return ::dukpp03::Maybe<_UnderlyingValue*>(&(result.mutableValue()));
        }
        return ::dukpp03::Maybe<_UnderlyingValue*>();
    }
};


/*! Returns address of type stored in variant.
 */
template<>
struct GetAddressOfType<const char*, false>
{
public: 
    /*! Returns address of type, stored in variant.
        \param[in] v value
        \return empty maybe
     */
    inline static ::dukpp03::Maybe<const char*> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<const char*>();
    }
};

}

}
