/*! \file variantinterface.h
    
    Contains an interface for interacting with variants in code
*/
#pragma once
#include "../db/dbvariant.h"
#include "../db/dbtypename.h"
#include "../db/dbconversiontable.h"


#include "getaddressoftype.h"

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

/*! A main interface for interacting with variants
 */
class VariantInterface
{
public:
    typedef sad::db::Variant Variant;
    
    /*! Makes variant from value
        \param[in] val value
        \param[in] v variant
     */
    template<
        typename _UnderlyingValue
    >
    static Variant* makeFrom(_UnderlyingValue val)
    {
        return new sad::db::Variant(val);
    }
    /*! Fetches underlying value from variant type
        \param[in] v a variant, containing data
        \return an underlying value
     */
    template<
        typename _UnderlyingValue
    >   
    static ::dukpp03::Maybe<_UnderlyingValue> get(sad::db::Variant* v)
    {
        sad::Maybe<_UnderlyingValue> maybe_value = v->get<_UnderlyingValue>(true, &sad::dukpp03::internal::conversion_table);
        if (maybe_value.exists())
        {
            if (maybe_value.referenceable())
            { 
                return ::dukpp03::Maybe<_UnderlyingValue>(&(maybe_value.mutableValue()));
            }
            return ::dukpp03::Maybe<_UnderlyingValue>(maybe_value.mutableValue());
        }
        return ::dukpp03::Maybe<_UnderlyingValue>();
    }
    
    /*! Fetches underlying value adress from variant type
        \param[in] v a variant, containing data
        \return an underlying value
     */
    template<
        typename _UnderlyingValue
    >   
    static ::dukpp03::Maybe<_UnderlyingValue> getAddress(sad::db::Variant* v)
    {
        return sad::dukpp03::GetAddressOfType<_UnderlyingValue>::getAddress(v);
    }
    
    /*! A typename interface for variant
     */
    template<
        typename _UnderlyingValue
    >
    class TypeName 
    {
    public:
        /*! Returns name of type 
            \return name of type
         */
        static std::string type()
        {
            sad::db::TypeName<_UnderlyingValue>::init();
            return sad::db::TypeName<_UnderlyingValue>::name();
        }
    };
};

}


}
