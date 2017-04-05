#include "dukpp-03-irrklang/dukpp-03-irrklang.h"
#include "dukpp-03-irrklang/jsstopcallback.h"

#include "dukpp-03/renderer.h"


#include <db/dbtypename.h>

#include <irrklang/sound.h>
#include <irrklang/singlesound.h>

#include <cassert>



static bool _engineIsCurrentlyPlaying(::irrklang::ISoundEngine* e, const sad::String& s)
{
    return e->isCurrentlyPlaying(s.c_str());
}

static ::irrklang::ISound* _enginePlay2DString(::irrklang::ISoundEngine* e, const sad::String& s, bool playLooped, bool startPaused)
{
    return e->play2D(s.c_str(), playLooped, startPaused);
}

static ::irrklang::ISound* _enginePlay2DSource(::irrklang::ISoundEngine* e,  ::irrklang::ISoundSource* s, bool playLooped, bool startPaused)
{
    return e->play2D(s, playLooped, startPaused);
}


static sad::Maybe<sad::irrklang::Sound*> _queryFromSadRenderer(sad::Renderer* r, const sad::String& tree, const sad::String& name)
{
    sad::Maybe<sad::irrklang::Sound*> result;
    sad::resource::Tree* sourceTree = r->tree(tree);
    if (sourceTree)
    {
        sad::irrklang::Sound* sound = sourceTree->get<sad::irrklang::Sound>(name);
        if (sound)
        {
            result.setValue(sound);
        }
    }
    return result;
}

static sad::Maybe<sad::irrklang::Sound*> _queryFromDukpp03Renderer(sad::dukpp03::Renderer* r, const sad::String& tree, const sad::String& name)
{
    return _queryFromSadRenderer(r, tree, name);
}


static sad::Maybe<sad::irrklang::Sound*> _queryFromGlobalRenderer(const sad::String& tree, const sad::String& name)
{
    return _queryFromSadRenderer(sad::Renderer::ref(), tree, name);
}

static void _addCallback(sad::irrklang::Sound* snd, sad::dukpp03::Context* ctx,  sad::dukpp03::CompiledFunction f)
{
    snd->addCallback(new  sad::dukpp03irrklang::JSStopCallback(ctx, f));
}



void sad::dukpp03irrklang::init(sad::dukpp03::Context* ctx)
{
    // Bindings for  sad::irrklang::Engine
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
    // Bindings for  ::irrklang::ISoundEngine
    {
        bool result = ctx->eval("_IrrKlangSoundEngine = function() {};");    
        assert(result);


        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        bool (::irrklang::ISoundEngine::*isCurrentlyPlaying1)(::irrklang::ISoundSource*) = &::irrklang::ISoundEngine::isCurrentlyPlaying;

        c->setPrototypeFunction("_IrrKlangSoundEngine");
        c->addMethod("isCurrentlyPlayingSoundSource",  sad::dukpp03::bind_method::from(isCurrentlyPlaying1));
        c->addMethod("stopAllSounds",  sad::dukpp03::bind_method::from(&::irrklang::ISoundEngine::stopAllSounds));
        c->addMethod("setAllSoundsPaused",  sad::dukpp03::bind_method::from(&::irrklang::ISoundEngine::setAllSoundsPaused));
        c->addMethod("getSoundVolume",  sad::dukpp03::bind_method::from(&::irrklang::ISoundEngine::getSoundVolume));
        c->addMethod("setSoundVolume",  sad::dukpp03::bind_method::from(&::irrklang::ISoundEngine::setSoundVolume));
        
        ctx->addClassBinding("::irrklang::ISoundEngine", c);

        ctx->registerCallable(
            "_IrrKlangEngineIsCurrentlyPlaying", 
            sad::dukpp03::make_function::from(_engineIsCurrentlyPlaying)
        );

        ctx->registerCallable(
            "_IrrKlangEnginePlay2DString", 
            sad::dukpp03::make_function::from(_enginePlay2DString)
        );
        ctx->registerCallable(
            "_IrrKlangEnginePlay2DSource", 
            sad::dukpp03::make_function::from(_enginePlay2DSource)
        );
        

        result = ctx->eval("_IrrKlangSoundEngine.isCurrentlyPlaying= function(s) { if (typeof s == \"string\") return _IrrKlangEngineIsCurrentlyPlaying(this, s); else return  this.isCurrentlyPlayingSoundSource(s);   };  ");
        assert(result);

        result = ctx->eval(
            "_IrrKlangSoundEngine.play2D = function(s, playLooped, startPaused) {"
                "var _p  = (typeof playLooped == \"undefined\") ? false : playLooped;"
                "var _sp = (typeof startPaused == \"undefined\") ? false : startPaused;"
                "if (typeof s == \"string\") return _IrrKlangEnginePlay2DString(this, s, _p, _sp); "
                                       "else return _IrrKlangEnginePlay2DSource(this, s, _p, _sp);"
            "};  "
        );
        assert(result);

    }

    // Bindings for  sad::irrklang::Sound
    {
        bool result = ctx->eval("_SadIrrKlangSound = function() {};");
        assert(result);


        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        
        c->setPrototypeFunction("_SadIrrKlangSound");
        c->addMethod("setDefaultVolume", sad::dukpp03::bind_method::from(&sad::irrklang::Sound::setDefaultVolume));
        c->addMethod("play2D", sad::dukpp03::bind_method::from(&sad::irrklang::Sound::play2D));
        c->addMethod("s", sad::dukpp03::bind_method::from(&sad::irrklang::Sound::s));
        c->addMethod("isPlaying", sad::dukpp03::bind_method::from(&sad::irrklang::Sound::isPlaying));

        ctx->addClassBinding("sad::irrklang::Sound", c);

        ctx->registerCallable(
            "_SadIrrKlangSoundQueryFromSadRenderer",
            sad::dukpp03::make_function::from(_queryFromGlobalRenderer)
        );

        ctx->registerCallable(
            "_SadIrrKlangSoundQueryFromDukpp03Renderer",
            sad::dukpp03::make_function::from(_queryFromDukpp03Renderer)
        );
        ctx->registerCallable(
            "_SadIrrKlangSoundQueryFromGlobalRenderer",
            sad::dukpp03::make_function::from(_queryFromGlobalRenderer)
        );
        ctx->registerCallable(
            "_SadIrrKlangSoundAddCallback",
            sad::dukpp03::make_function::from(_addCallback)
        );


        result = ctx->eval("sad.irrklang.Sound = _SadIrrKlangSound; sad.irrklang.Sound.addCallback = function(ctx, f) { _SadIrrKlangSoundAddCallback(this, ctx, f);   };");
        assert(result);

        result = ctx->eval(
            "sad.irrklang.Sound.query = function(renderer, tree, name) {"
            "if (arguments.length == 1) return _SadIrrKlangSoundQueryFromGlobalRenderer(\"\", renderer);"
            "if (arguments.length == 2) { "
            "    if (typeof renderer == \"string\") {"
            "        return _SadIrrKlangSoundQueryFromGlobalRenderer(renderer, tree);  "
            "    } else { "
            "         var result = null; "
            "         try  { "
            "              result = _SadIrrKlangSoundQueryFromSadRenderer(renderer, \"\", tree); "
            "         } catch (ex) {"
            "              result = _SadIrrKlangSoundQueryFromDukpp03Renderer(renderer, \"\", tree); "
            "         }"
            "        return result;"
            "    }"
            "}"
            "var result = null; try { result =  SadIrrKlangSoundQueryFromSadRenderer(renderer, tree, name);  } catch (ex) { result = _SadIrrKlangSoundQueryFromDukpp03Renderer(renderer, tree, name); }"
            "return result;"
            "};  "
        );
        assert(result);
    }

    // Bindings for  ::irrklang::ISoundSource
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addMethod("getPlayLength", sad::dukpp03::bind_method::from(&::irrklang::ISoundSource::getPlayLength));
        c->addMethod("getDefaultVolume", sad::dukpp03::bind_method::from(&::irrklang::ISoundSource::getDefaultVolume));
        c->addMethod("setDefaultVolume", sad::dukpp03::bind_method::from(&::irrklang::ISoundSource::setDefaultVolume));
        c->addMethod("getDefaultMinDistance", sad::dukpp03::bind_method::from(&::irrklang::ISoundSource::getDefaultMinDistance));
        c->addMethod("setDefaultMinDistance", sad::dukpp03::bind_method::from(&::irrklang::ISoundSource::setDefaultMinDistance));
        c->addMethod("getDefaultMaxDistance", sad::dukpp03::bind_method::from(&::irrklang::ISoundSource::getDefaultMaxDistance));
        c->addMethod("setDefaultMaxDistance", sad::dukpp03::bind_method::from(&::irrklang::ISoundSource::setDefaultMaxDistance));
        
        ctx->addClassBinding("::irrklang::ISoundSource", c);
    }
    // Bindings for  ::irrklang::ISound
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addMethod("setIsPaused", sad::dukpp03::bind_method::from(&::irrklang::ISound::setIsPaused));
        c->addMethod("getIsPaused", sad::dukpp03::bind_method::from(&::irrklang::ISound::getIsPaused));
        c->addMethod("stop", sad::dukpp03::bind_method::from(&::irrklang::ISound::stop));
        c->addMethod("setVolume", sad::dukpp03::bind_method::from(&::irrklang::ISound::setVolume));
        c->addMethod("getVolume", sad::dukpp03::bind_method::from(&::irrklang::ISound::getVolume));
        c->addMethod("isFinished", sad::dukpp03::bind_method::from(&::irrklang::ISound::isFinished));
        c->addMethod("getPlayPosition", sad::dukpp03::bind_method::from(&::irrklang::ISound::getPlayPosition));
        c->addMethod("setPlayPosition", sad::dukpp03::bind_method::from(&::irrklang::ISound::setPlayPosition));
        c->addMethod("getPlaybackSpeed", sad::dukpp03::bind_method::from(&::irrklang::ISound::getPlaybackSpeed));
        c->addMethod("setPlaybackSpeed", sad::dukpp03::bind_method::from(&::irrklang::ISound::setPlaybackSpeed));
        c->addMethod("getSoundSource", sad::dukpp03::bind_method::from(&::irrklang::ISound::getSoundSource));
        c->addMethod("isLooped", sad::dukpp03::bind_method::from(&::irrklang::ISound::isLooped));
        c->addMethod("setIsLooped", sad::dukpp03::bind_method::from(&::irrklang::ISound::setIsLooped));
        
        ctx->addClassBinding("::irrklang::ISound", c);
    }
    // Bindings for  sad::irrklang::SingleSound
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::irrklang::SingleSound>("SadIrrKlangSingleSound");
        c->addMethod("_play2D", sad::dukpp03::bind_method::from(&sad::irrklang::SingleSound::play2D));
        c->addMethod("setVolume", sad::dukpp03::bind_method::from(&sad::irrklang::SingleSound::setVolume));
        c->addMethod("stop", sad::dukpp03::bind_method::from(&sad::irrklang::SingleSound::stop));
        ctx->addClassBinding("sad::irrklang::SingleSound", c);
 
        bool result = ctx->eval("sad.irrklang.SingleSound = SadIrrKlangSingleSound; ");
        assert(result);

        result = ctx->eval("sad.irrklang.SingleSound.play2D = function(source, volume, looped) {"
            "if (typeof volume == \"undefined\") { return this._play2D(source, 1.0, false); };"
            "if (typeof looped == \"undefined\") { return this._play2D(source, volume, false); };"
            "return this._play2D(source, volume, looped);"
            "};"
        );
        assert(result);
        
        ctx->addClassBinding("sad::irrklang::SingleSound", c);
    }

    ctx->registerCallable(
        "_SadIrrKlangInit",
        sad::dukpp03::make_function::from(sad::dukpp03irrklang::init)
    );
    bool init_result = ctx->eval("sad.irrklang.init = _SadIrrKlangInit");
    assert(init_result);
}

DECLARE_COMMON_TYPE(::irrklang::ISoundSource)
DECLARE_COMMON_TYPE(::irrklang::ISoundEngine)
DECLARE_COMMON_TYPE(::irrklang::ISound)
