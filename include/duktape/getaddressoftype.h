/*! \file getaddressoftype.h
    
    Gets address of specified type
 */
#pragma once
#include "../db/dbvariant.h"
#include "../3rdparty/dukpp-03.h"

namespace sad
{

namespace duktape
{

/*! Gets adress fo value of specified type, that is stored in variant.
    Here we try to get plain value (not a pointer type) as and address, so we don't allow such behaviour
 */
template<typename _UnderlyingValue>
struct GetAddressOfType
{
public: 
    /*! Returns address of type, stored in variant - hence nothing for plain types
        \param[in] v value
        \return empty maybe
     */
    static dukpp03::Maybe<_UnderlyingValue> getAddress(sad::db::dbVariant* v)
    {
        return dukpp03::Maybe<_UnderlyingValue>();
    }
};

/*! Returns address of type stored in variant.
 */
template<typename _UnderlyingValue>
struct GetAddressOfType<_UnderlyingValue*>
{
public: 
    /*! Returns address of type, stored in variant.
        \param[in] v value
        \return empty maybe
     */
    static dukpp03::Maybe<_UnderlyingValue*> getAddress(sad::db::dbVariant* v)
    {
        sad::Maybe<_UnderlyingValue> result = v->get(true);
        if (result.exists())
        {
            return dukpp03::Maybe<_UnderlyingValue*>(&(result.mutableValue()));
        }
        return dukpp03::Maybe<_UnderlyingValue*>();
    }
};

}

}
