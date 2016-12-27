/*! \file context.h
  
    Defines a context, that can be used in Saddy
 */
#pragma once
#include "variantinterface.h"
#include "mapinterface.h"
#include "timerinterface.h"
#include "../3rdparty/dukpp-03/include/dukpp-03.h"

namespace sad
{

namespace dukpp03
{

/*! A default context
 */
typedef ::dukpp03::Context<sad::dukpp03::MapInterface, sad::dukpp03::VariantInterface, sad::dukpp03::TimerInterface> Context;

typedef ::dukpp03::make_fun<sad::dukpp03::Context> make_function;
typedef ::dukpp03::register_constructor<sad::dukpp03::Context> register_constructor;
typedef ::dukpp03::make_method<sad::dukpp03::Context> make_method;
typedef ::dukpp03::bind_method<sad::dukpp03::Context> bind_method;
typedef ::dukpp03::CompiledFunction<sad::dukpp03::Context> CompiledFunction;
typedef ::dukpp03::getter<sad::dukpp03::Context> getter;
typedef ::dukpp03::setter<sad::dukpp03::Context> setter;
typedef ::dukpp03::ClassBinding<sad::dukpp03::Context> ClassBinding;

}

}
