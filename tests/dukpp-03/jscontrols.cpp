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
#include "input/controls.h"
#include "slurp.h"
#include "util/fs.h"
#define _INC_STDIO
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)



struct JSControlsTest : tpunit::TestFixture
{
public:
    JSControlsTest() : tpunit::TestFixture(
       TEST(JSControlsTest::testCommon)
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
        if (sad::util::fileExists("js_controls_test.log"))
        {
            remove("js_controls_test.log");
        }
        ASSERT_TRUE(target->open("js_controls_test.log"));
        r->log()->addTarget(target);

        ctx.registerGlobal("ctx", ctx2);
        ctx.registerGlobal("r", r);

        ctx2->setRenderer(r);
        ctx2->registerGlobal("r", r);

        bool eval_result = ctx.eval(
            "var input = new sad.input.JSControls(); input.addRenderer(r);\n"
            "input.onQuit(ctx, function() { r.log().debug(\"Quit\", \"1.js\", 1); });\n"
            "input.onActivate(ctx, function() { r.log().debug(\"Activate\", \"1.js\", 1); });\n"
            "input.onDeactivate(ctx, function() { r.log().debug(\"Deactivate\", \"1.js\", 1); });\n"
            "input.onEnter(ctx, function() { r.log().debug(\"Enter\", \"1.js\", 1); });\n"
            "input.onLeave(ctx, function() { r.log().debug(\"Leave\", \"1.js\", 1); });\n"
            "input.onKeyPress(ctx, function() { r.log().debug(\"KeyPress\", \"1.js\", 1); });\n"
            "input.onKeyRelease(ctx, function() { r.log().debug(\"KeyRelease\", \"1.js\", 1); });\n"
            "input.onMove(ctx, function() { r.log().debug(\"Move\", \"1.js\", 1); });\n"
            "input.onClick(ctx, function() { r.log().debug(\"Click\", \"1.js\", 1); });\n"
            "input.onRelease(ctx, function() { r.log().debug(\"Release\", \"1.js\", 1); });\n"
            "input.onDblClick(ctx, function() { r.log().debug(\"DblClick\", \"1.js\", 1); });\n"
            "input.onWheel(ctx, function() { r.log().debug(\"Wheel\", \"1.js\", 1); });\n"
            "input.onResize(ctx, function(e) { r.log().debug(\"Resize\" + e.OldSize.width, \"1.js\", 1); });\n",
            false, 
            &error
        );
        if (!eval_result)
        {
            printf("%s\n", error.c_str());
        }
        ASSERT_TRUE( eval_result );
        ASSERT_TRUE( error.size() == 0 );

        r->controls()->postEvent(sad::input::EventType::ET_Quit, sad::input::QuitEvent());
        r->controls()->postEvent(sad::input::EventType::ET_Activate, sad::input::ActivateEvent());
        r->controls()->postEvent(sad::input::EventType::ET_Deactivate, sad::input::DeactivateEvent());

        r->controls()->postEvent(sad::input::EventType::ET_MouseEnter, sad::input::MouseEnterEvent());
        r->controls()->postEvent(sad::input::EventType::ET_MouseLeave, sad::input::MouseLeaveEvent());
        r->controls()->postEvent(sad::input::EventType::ET_KeyPress, sad::input::KeyPressEvent());

        r->controls()->postEvent(sad::input::EventType::ET_KeyRelease, sad::input::KeyReleaseEvent());
        r->controls()->postEvent(sad::input::EventType::ET_MouseMove, sad::input::MouseMoveEvent());
        r->controls()->postEvent(sad::input::EventType::ET_MousePress, sad::input::MousePressEvent());

        r->controls()->postEvent(sad::input::EventType::ET_MouseRelease, sad::input::MouseReleaseEvent());
        r->controls()->postEvent(sad::input::EventType::ET_MouseDoubleClick, sad::input::MouseDoubleClickEvent());
        r->controls()->postEvent(sad::input::EventType::ET_MouseWheel, sad::input::MouseWheelEvent());
        r->controls()->postEvent(sad::input::EventType::ET_Resize, sad::input::ResizeEvent());


        ctx2->delRef();
        r->delRef();

        sad::Maybe<sad::String> s = sad::slurp("js_controls_test.log");
        ASSERT_TRUE(s.exists());
        ASSERT_TRUE(s.value().size() != 0);
    }


} _js_controls_test;