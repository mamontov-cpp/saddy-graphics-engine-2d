#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdio>
#include "dukpp-03/context.h"
#include "dukpp-03/renderer.h"
#include "sprite2d.h"
#include "log/log.h"
#include "log/filetarget.h"
#include "slurp.h"
#include "util/fs.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#include "sadsleep.h"
#include "pipeline/pipeline.h"
#include "glcontext.h"
#pragma warning(pop)


static sad::Timer timer;
static sad::Renderer* renderer;

static void check_expired_and_quit_after_4000ms()
{
    timer.stop();
    if (timer.elapsed() > 4000)
    {
        renderer->quit();
    }
}


struct PipelineTest : tpunit::TestFixture
{
public:
    PipelineTest() : tpunit::TestFixture(
       TEST(PipelineTest::testCommon)
    ) {}

    /*! A common test for exposed API
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppMemberFunctionMayBeConst
    void testCommon()
    {
        std::string error;
        sad::dukpp03::Context ctx;
        sad::dukpp03::Context* ctx2 = new sad::dukpp03::Context();
        ctx2->addRef();
        sad::dukpp03::Renderer* r = new sad::dukpp03::Renderer();
        r->addRef();
        sad::log::FileTarget* target = new sad::log::FileTarget();
        if (sad::util::fileExists("js_pipeline_test.log"))
        {
            remove("js_pipeline_test.log");
        }
        ASSERT_TRUE(target->open("js_pipeline_test.log"));
        r->log()->addTarget(target);

        ctx.registerGlobal("ctx", ctx2);
        ctx.registerGlobal("r", r);

        ctx2->setRenderer(r);
        ctx2->registerGlobal("r", r);

        bool eval_result = ctx.eval(
            "var p = sad.pipeline(r);"
            "p.beforeScene().eachFrame(\"bsef\", ctx, function() { r.log().debug(\"1\", \"1.js\", 1); });\n"
            "p.beforeScene().eachMS(\"bsms\", 2000, ctx, function() { r.log().debug(\"2\", \"1.js\", 1); });\n"
            "p.beforeScene().oneShot(\"bsos\", ctx, function() { r.log().debug(\"3\", \"1.js\", 1); });\n"
            "p.beforeScene().delayed(\"bsd\", 2000, ctx, function() { r.log().debug(\"4\", \"1.js\", 1); });\n"
            "p.afterScene().eachFrame(\"asef\", ctx, function() { r.log().debug(\"5\", \"1.js\", 1); });\n"
            "p.afterScene().eachMS(\"asms\", 2000, ctx, function() { r.log().debug(\"6\", \"1.js\", 1); });\n"
            "p.afterScene().oneShot(\"asos\", ctx, function() { r.log().debug(\"7\", \"1.js\", 1); });\n"
            "p.afterScene().delayed(\"asd\", 2000, ctx, function() { r.log().debug(\"8\", \"1.js\", 1); });\n"
            "p.beforeEvent(\"bsd\").eachFrame(\"beef\", ctx, function() { r.log().debug(\"9\", \"1.js\", 1); });\n"
            "p.beforeEvent(\"bsd\").eachMS(\"bems\", 2000, ctx, function() { r.log().debug(\"10\", \"1.js\", 1); });\n"
            "p.beforeEvent(\"bsd\").oneShot(\"beos\", ctx, function() { r.log().debug(\"11\", \"1.js\", 1); });\n"
            "p.beforeEvent(\"bsd\").delayed(\"bed\", 2000, ctx, function() { r.log().debug(\"12\", \"1.js\", 1); });\n"
            "p.afterEvent(\"bsd\").eachFrame(\"aeef\", ctx, function() { r.log().debug(\"13\", \"1.js\", 1); });\n"
            "p.afterEvent(\"bsd\").eachMS(\"aems\", 2000, ctx, function() { r.log().debug(\"14\", \"1.js\", 1); });\n"
            "p.afterEvent(\"bsd\").oneShot(\"aeos\", ctx, function() { r.log().debug(\"15\", \"1.js\", 1); });\n"
            "p.afterEvent(\"bsd\").delayed(\"aed\", 2000, ctx, function() { r.log().debug(\"16\", \"1.js\", 1); });\n",
            false, 
            &error
        );
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );

        r->init(sad::Settings(320, 230, false));
        timer.start();
        renderer = r;
        r->pipeline()->appendProcess(check_expired_and_quit_after_4000ms);
        r->run();

        ctx2->delRef();
        r->delRef();

        sad::Maybe<sad::String> s = sad::slurp("js_pipeline_test.log");
        ASSERT_TRUE(s.exists());
        ASSERT_TRUE(s.value().size() != 0);
        sad::String log = s.value();
        for(int i = 1; i <= 16; i++)
        {
            sad::String search_entry = "sad::Renderer::run(): ";
            search_entry << sad::String::number(i);
            ASSERT_TRUE( log.find(search_entry) != log.npos );
        }
    }


} __pipeline_test;