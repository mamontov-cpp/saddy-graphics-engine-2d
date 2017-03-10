/*! \file basiccontext.h
    
    Describes a basic context
 */
#pragma once
#include "variantinterface.h"
#include "mapinterface.h"
#include "timerinterface.h"
#include "wrapvalue.h"
#include "../3rdparty/dukpp-03/include/dukpp-03.h"
#include "../3rdparty/dukpp-03/include/object.h"

namespace sad
{

namespace dukpp03
{

/*! A basic context
 */
typedef ::dukpp03::Context<
    sad::dukpp03::MapInterface, 
    sad::dukpp03::VariantInterface, 
    sad::dukpp03::TimerInterface,
    sad::dukpp03::WrapValue
> BasicContext;

}

}
