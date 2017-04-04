#include "dukpp-03-irrklang/dukpp-03-irrklang.h"

#include <db/dbtypename.h>

#include <cassert>


void sad::dukpp03irrklang::init(sad::dukpp03::Context* ctx)
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addMethod("tryLoad",  sad::dukpp03::bind_method::from(&sad::irrklang::Engine::tryLoad));
    c->addMethod("get",  sad::dukpp03::bind_method::from(&sad::irrklang::Engine::get));
    
    ctx->addClassBinding("sad::irrklang::Engine", c);

    ctx->registerCallable(
        "SadIrrKlangEngineRef", 
        sad::dukpp03::make_function::from(sad::irrklang::Engine::ref)
    );
    ctx->registerCallable(
        "SadIrrKlangEngineERef", 
        sad::dukpp03::make_function::from(sad::irrklang::Engine::eref)
    );

    bool result = ctx->eval("sad.irrklang = {}; sad.irrklang.Engine = {}; sad.irrklang.Engine.ref = SadIrrKlangEngineRef; sad.irrklang.Engine.eref = SadIrrKlangEngineERef;");    

    assert(result);
}

DECLARE_COMMON_TYPE(::irrklang::ISoundSource)
DECLARE_COMMON_TYPE(::irrklang::ISoundEngine)
