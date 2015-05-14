/*! \file decay.h
    \author HiddenSeeker
    
    Simple decay replacement for old compilers
 */
#pragma once

namespace sad
{

namespace duktape
{

template<
    typename T
>
struct Decay
{
    typedef T Type;
};


template<
    typename T
>
struct Decay<T&>
{
    typedef T Type;
};

template<
    typename T
>
struct Decay<const T&>
{
    typedef T Type;
};

}
    
}
