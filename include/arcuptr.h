/*! \file arcuptr.h
 *
 *  Declares automatically initializes RCUPtr which initializes itself automatically
 */
#pragma once
#include "refcountable.h"

namespace sad
{

template<
    typename T
>
struct ARCUPtr: public RCUPtr<T>
{
public:
    /*! Initializes self with default parameters
     */
    ARCUPtr() : sad::RCUPtr<T>(new T(), sad::RefCountable::delRefInstance)
    {
        this->get()->addRef();
    }
    /*! Initializes self with arguments
     *  \param[in] args arguments
     */
    template<typename... Args>
    ARCUPtr(Args... args) : sad::RCUPtr<T>(new T(args...), sad::RefCountable::delRefInstance)
    {
        this->get()->addRef();
    }
};

}