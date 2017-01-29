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

/*! A basic context
 */
typedef ::dukpp03::Context<sad::dukpp03::MapInterface, sad::dukpp03::VariantInterface, sad::dukpp03::TimerInterface> BasicContext;

typedef ::dukpp03::make_fun<sad::dukpp03::BasicContext> make_function;
typedef ::dukpp03::register_constructor<sad::dukpp03::BasicContext> register_constructor;
typedef ::dukpp03::make_method<sad::dukpp03::BasicContext> make_method;
typedef ::dukpp03::bind_method<sad::dukpp03::BasicContext> bind_method;
typedef ::dukpp03::CompiledFunction<sad::dukpp03::BasicContext> CompiledFunction;
typedef ::dukpp03::getter<sad::dukpp03::BasicContext> getter;
typedef ::dukpp03::setter<sad::dukpp03::BasicContext> setter;
typedef ::dukpp03::ClassBinding<sad::dukpp03::BasicContext> ClassBinding;

/*! A context, that should be used, when working with library
 */
class Context: public sad::dukpp03::BasicContext
{
public:
    /*! Construct default context
     */
    Context();
    /*! Destructs a context
     */
    ~Context();
};


}

}
