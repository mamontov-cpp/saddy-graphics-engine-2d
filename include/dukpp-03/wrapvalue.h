/*! \file wrapvalue.h
 

    A value wrapper functions for wrapping objects
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include "../object.h"

namespace sad
{
    
namespace dukpp03
{
    
class WrapValue
{
public:
    /*! Performs wrapping value for specified variant
        \param[in] context a context
        \param[in] variant a specified variant
        \param[in] wrapped was a value wrapped by generic binding previously
     */
    static void perform(void* context, void* variant, bool wrapped);
};

}

}
