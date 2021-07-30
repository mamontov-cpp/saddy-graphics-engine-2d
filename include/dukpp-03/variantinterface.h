/*! \file variantinterface.h
    
    Contains an interface for interacting with variants in code
*/
#pragma once
#include "../db/dbvariant.h"
#include "../db/dbtypename.h"
#include "../db/dbconversiontable.h"


#include "getaddressoftype.h"

#ifdef _MSC_VER
#include <type_traits>
#endif


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
    /*! A checker for abstract classes
     */
    template<typename T>
    struct isAbstractClass
    {
#ifndef _MSC_VER
        /*! A checker for non-abstract class
            \return not-implemented
         */
        template<class U>
        static char check_sig(U (*)[1]);
        /*! A checker for abstract class
            \return not-implemented
         */        
        template<class U>
        static short check_sig(...);
        /*! A real value
         */
        enum { VALUE = sizeof(isAbstractClass<T>::template check_sig<T>(0)) - 1 };
#else
        /*! A real value
         */
        enum { VALUE = std::is_abstract<T>::value };
#endif
    };
    /*! A checker for pointer to abstract class
     */
    template<typename T>
    struct isPointerToAbstractClass
    {
        enum { VALUE = false };
    };
    /*! A checker for pointer to abstract class
     */ 
    template<typename T>
    struct isPointerToAbstractClass<T*>
    {
        enum { VALUE = isAbstractClass<T>::VALUE };
    };
    
    /*! Makes variant from value
        \param[in] val value
        \return resulting variant
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
    
    /*! Fetches underlying value address from variant type
        \param[in] v a variant, containing data
        \return an underlying value
     */
    template<
        typename _UnderlyingValue
    >   
    static ::dukpp03::Maybe<_UnderlyingValue> getAddress(sad::db::Variant* v)
    {
        return sad::dukpp03::GetAddressOfType<_UnderlyingValue, isPointerToAbstractClass<_UnderlyingValue>::VALUE, sad::db::TypeName<_UnderlyingValue>::CAN_BE_CASTED_TO_OBJECT>::getAddress(v);
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
