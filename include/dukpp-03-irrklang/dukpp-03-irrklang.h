/*! \file dukpp-03-irrklang.h
    
    An initialization function for dukpp-03-irrklang is placed here
 */
#pragma once
#include "../dukpp-03/context.h"

#include "../irrklang/engine.h"


namespace sad
{

namespace dukpp03
{

template<>
struct GetAddressOfType<sad::irrklang::Engine*, false, false>
{
public: 
    /*! Returns address of type, stored in variant.
        \param[in] v value
        \return empty maybe
     */
    inline static ::dukpp03::Maybe<sad::irrklang::Engine*> getAddress(sad::db::Variant* v)
    {
        return ::dukpp03::Maybe<sad::irrklang::Engine*>();
    }
};

}

}

namespace sad
{

namespace dukpp03irrklang
{



/*! Inits functions in this context
    \param[in] ctx a context
 */
void init(sad::dukpp03::Context* ctx);

}

}