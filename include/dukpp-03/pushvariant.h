/*! \file pushvariant.h
 
    Contains definitions with pushing variants into stack
 */
#pragma once
#include "basiccontext.h"

namespace sad
{

namespace db
{

class Variant;    

}
    
namespace dukpp03
{

/*! Pushes variant on stack
    \param[in] ctx a context
    \param[in] v variant
 */    
void pushVariant(sad::dukpp03::BasicContext* ctx, const sad::db::Variant& v);

}

}