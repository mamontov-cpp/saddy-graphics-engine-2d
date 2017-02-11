/*! \file   isrefcountable.h
    
    Declares an is compile-time check, whether type is ref-countable
*/
#pragma once
#include "refcountable.h"

namespace sad
{


/*! A compile-time check, whether type is refcountable
 */    
template<
    typename _Type
>
struct IsRefCountable
{
public:

    /*! Yes value for enum
     */
    typedef char yes;
    /*! No value for enum
     */
    typedef long long no;

    static yes check(sad::RefCountable*);
    static no  check(...);

    enum { value = sizeof(check(static_cast<_Type*>(0))) == sizeof(yes) };

};

}

