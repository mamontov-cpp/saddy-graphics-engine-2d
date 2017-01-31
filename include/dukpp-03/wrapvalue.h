/*! \file wrapvalue.h
 

    A value wrapper functions for wrapping objects
 */
#pragma once
#include "../object.h"

namespace sad
{
    
namespace dukpp03
{
    
class WrapValue
{
public:
    /*! Wraps sad::Object in context
        \param[in] ctx context
        \param[in] v variant
     */
    static void wrapSadObject(void* ctx, sad::Object* v);
    /*! Wraps sad::db::Object in context
        \param[in] ctx context
        \param[in] v variant
     */
    static void wrapSadDBObject(void* ctx, sad::db::Object* v);
    /*! Performs wrapping value for specified variant
        \param[in] context a context
        \param[in] variant a specified variant
        \param[in] wrapped was a value wrapped by generic binding previously
     */
    static void perform(void* context, void* variant, bool wrapped);
};

}

}
