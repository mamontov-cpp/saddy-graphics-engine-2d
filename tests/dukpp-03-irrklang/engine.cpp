#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdio>
#include "dukpp-03/context.h"
#include "dukpp-03-irrklang/dukpp-03-irrklang.h"
#include "irrklang/sound.h"
#include "sprite2d.h"
#include "sadsleep.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


struct EngineTest : tpunit::TestFixture
{
public:
    EngineTest() : tpunit::TestFixture(
       TEST(EngineTest::testEngine),
       TEST(EngineTest::testSoundEngine),
       TEST(EngineTest::testSound)
    ) {}

    /*! A common test for exposed engine API
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testEngine()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        sad::dukpp03irrklang::init(&ctx);
        bool eval_result = ctx.eval("sad.irrklang.Engine.ref()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );

        eval_result = ctx.eval("sad.irrklang.Engine.eref()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );

        eval_result = ctx.eval("sad.irrklang.Engine.ref().get()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );

        eval_result = ctx.eval("sad.irrklang.Engine.ref().tryLoad(\"1.ogg\")", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
    }

    /*! A common test for exposed main engine API
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testSoundEngine()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        sad::dukpp03irrklang::init(&ctx);
        bool eval_result = ctx.eval("sad.irrklang.Engine.eref().isCurrentlyPlaying(\"1\")", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );

        eval_result = ctx.eval("sad.irrklang.Engine.eref().stopAllSounds()", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );

        eval_result = ctx.eval("sad.irrklang.Engine.eref().setAllSoundsPaused(false)", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );

        eval_result = ctx.eval("sad.irrklang.Engine.eref().play2D(\"1\")", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
    }

    void testSound()
    {
        // Load must be successfull to work
        sad::Renderer r;
        sad::resource::Tree* tree = new sad::resource::Tree();
        tree->setStoreLinks(true);
        tree->setRenderer(&r);
        // In debug, sad::fretype::Factory fonts becomes in font
        tree->factory()->registerResource<sad::irrklang::Sound>();
        tree->factory()->registerDefaultFileTypeFor<sad::irrklang::Sound>();

        r.addTree("", tree);
        sad::Vector<sad::resource::Error *> errors = tree->loadFromFile("tests/sound.json");

        int count = errors.size();
        sad::util::free(errors);
        ASSERT_TRUE(count == 0);

        std::string error;
        sad::dukpp03::Context ctx;
        sad::dukpp03irrklang::init(&ctx);
        ctx.registerGlobal("r", &r); 
        
        bool eval_result = ctx.eval("sad.irrklang.Sound.query(r, \"sound\");", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );

        eval_result = ctx.eval("sad.irrklang.Sound.query(r, \"sound\").setDefaultVolume(0.8);", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );

        eval_result = ctx.eval(" var c = new sad.Context(); sad.irrklang.init(c); sad.irrklang.Sound.query(r, \"sound\").addCallback(c, function(o) { console.log(o); });", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );

        eval_result = ctx.eval("sad.irrklang.Sound.query(r, \"sound\").play2D(1.0, false);", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );

        eval_result = ctx.eval("sad.irrklang.Sound.query(r, \"sound\").isPlaying();", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );

        eval_result = ctx.eval("sad.irrklang.Sound.query(r, \"sound\").s();", false, &error);
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );


        sad::sleep(5000);
    }


} _engine_test;