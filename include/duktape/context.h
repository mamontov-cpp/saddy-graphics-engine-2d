/*! \file context.h
  
    Defines a context, that can be used in Saddy
 */
#pragma once
#include "variantinterface.h"
#include "mapinterface.h"
#include "timerinterface.h"


namespace sad
{

namespace duktape
{

/*! A default context
 */
typedef dukpp03::Context<sad::duktape::MapInterface, sad::duktape::VariantInterface, sad::duktape::TimerInterface> Context;

}

}
