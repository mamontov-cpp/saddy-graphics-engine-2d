/*! \file context.h
  
    Defines a context, that can be used in Saddy
 */
#pragma once
#include "basiccontext.h"
#include "getvalue.h"
#include "pushvalue.h"

namespace sad
{

namespace dukpp03
{

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
