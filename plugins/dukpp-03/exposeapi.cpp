#include "dukpp-03/context.h"
#include "dukpp-03/thread.h"

#include <sadpoint.h>
#include <sadsize.h>
#include <sadrect.h>
#include <timer.h>
#include <p2d/vector.h>
#include <slurp.h>
#include <spit.h>

#include <renderer.h>
#include <window.h>
#include <util/fs.h>

#include <cstdio>

#include <settings.h>

void sad::dukpp03::exposeAPI(sad::dukpp03::Context* ctx)
{
    // Exposing sad::Settings
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::Settings, unsigned int, unsigned int, bool>("SadSettings");
        c->addConstructor<sad::Settings, unsigned int, unsigned int, bool, sad::uchar, sad::uchar, float, float, float, bool, float>("SadSettings");
        c->addAccessor("width", sad::dukpp03::bind_method::from(&sad::Settings::width), sad::dukpp03::bind_method::from(&sad::Settings::setScreenWidth));
        c->addAccessor("height", sad::dukpp03::bind_method::from(&sad::Settings::height), sad::dukpp03::bind_method::from(&sad::Settings::setScreenHeight));
        c->addAccessor("isfullscreen", sad::dukpp03::bind_method::from(&sad::Settings::isFullscreen), sad::dukpp03::bind_method::from(&sad::Settings::setIsFullscreen));
        c->addAccessor("bpp", sad::dukpp03::bind_method::from(&sad::Settings::bpp), sad::dukpp03::bind_method::from(&sad::Settings::setBPP));
        c->addAccessor("depth", sad::dukpp03::bind_method::from(&sad::Settings::depth), sad::dukpp03::bind_method::from(&sad::Settings::setDepth));
        c->addAccessor("fov", sad::dukpp03::bind_method::from(&sad::Settings::fov), sad::dukpp03::bind_method::from(&sad::Settings::setFoV));
        c->addAccessor("znear", sad::dukpp03::bind_method::from(&sad::Settings::znear), sad::dukpp03::bind_method::from(&sad::Settings::setZNear));
        c->addAccessor("zfar", sad::dukpp03::bind_method::from(&sad::Settings::zfar), sad::dukpp03::bind_method::from(&sad::Settings::setZFar));
        c->addAccessor("ztest", sad::dukpp03::bind_method::from(&sad::Settings::ztest), sad::dukpp03::bind_method::from(&sad::Settings::setZTest));
        c->addAccessor("ztestvalue", sad::dukpp03::bind_method::from(&sad::Settings::ztestvalue), sad::dukpp03::bind_method::from(&sad::Settings::setZTestValue));
        c->addCloneValueObjectMethodFor<sad::Settings>();
        c->setPrototypeFunction("SadSettings");

        ctx->addClassBinding("sad::Settings", c);
    }

    // Exposing sad::Timer
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::Timer>("SadTimer");
        c->addMethod("start",  sad::dukpp03::bind_method::from(&sad::Timer::start));
        c->addMethod("stop",  sad::dukpp03::bind_method::from(&sad::Timer::stop));
        c->addMethod("elapsed",  sad::dukpp03::bind_method::from(&sad::Timer::elapsed));

        c->addCloneValueObjectMethodFor<sad::Timer>();
        c->setPrototypeFunction("SadTimer");

        ctx->addClassBinding("sad::Timer", c);
    }
    
    // Exposing sad::Thread
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::dukpp03::Thread, sad::dukpp03::Context*, sad::dukpp03::CompiledFunction>("SadThread");
        c->addMethod("run",  sad::dukpp03::rebind_method::to<sad::dukpp03::Thread>::from(&sad::Thread::run));
        c->addMethod("stop",  sad::dukpp03::rebind_method::to<sad::dukpp03::Thread>::from(&sad::Thread::stop));
        c->addMethod("exitCode",  sad::dukpp03::rebind_method::to<sad::dukpp03::Thread>::from(&sad::Thread::exitCode));
        c->addMethod("wait",  sad::dukpp03::bind_method::from(&sad::dukpp03::Thread::join));
        c->addMethod("wait",  sad::dukpp03::bind_method::from(&sad::dukpp03::Thread::waitFor));
        c->addMethod("join",  sad::dukpp03::bind_method::from(&sad::dukpp03::Thread::join));
        c->addMethod("waitFor",  sad::dukpp03::bind_method::from(&sad::dukpp03::Thread::waitFor));
        c->addMethod("running",  sad::dukpp03::rebind_method::to<sad::dukpp03::Thread>::from(&sad::Thread::running));
        
        c->setPrototypeFunction("SadThread");

        ctx->addClassBinding("sad::dukpp03::Thread", c);        
    }
    // Exposing sad::Window
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addMethod("setRect",   sad::dukpp03::bind_method::from(&sad::Window::setRect));
        c->addMethod("rect",   sad::dukpp03::bind_method::from(&sad::Window::rect));
        c->addMethod("show",   sad::dukpp03::bind_method::from(&sad::Window::show));
        c->addMethod("hide",   sad::dukpp03::bind_method::from(&sad::Window::hide));
        c->addMethod("title",   sad::dukpp03::bind_method::from(&sad::Window::title));
        c->addMethod("setTitle",   sad::dukpp03::bind_method::from(&sad::Window::setTitle));                
        c->addMethod("active",   sad::dukpp03::bind_method::from(&sad::Window::active));
        c->addMethod("minimized",   sad::dukpp03::bind_method::from(&sad::Window::minimized));
        c->addMethod("valid",   sad::dukpp03::bind_method::from(&sad::Window::valid));
        c->addMethod("fixed",   sad::dukpp03::bind_method::from(&sad::Window::fixed));
        c->addMethod("makeFixedSize",   sad::dukpp03::bind_method::from(&sad::Window::makeFixedSize));
        c->addMethod("makeResizeable",   sad::dukpp03::bind_method::from(&sad::Window::makeResizeable));
        c->addMethod("fullscreen",   sad::dukpp03::bind_method::from(&sad::Window::fullscreen));
        c->addMethod("enterFullscreen",   sad::dukpp03::bind_method::from(&sad::Window::enterFullscreen));
        c->addMethod("leaveFullscreen",   sad::dukpp03::bind_method::from(&sad::Window::leaveFullscreen));
        c->addMethod("hidden",   sad::dukpp03::bind_method::from(&sad::Window::hidden));
        c->addMethod("pushRect",   sad::dukpp03::bind_method::from(&sad::Window::pushRect));
        c->addMethod("popRect",   sad::dukpp03::bind_method::from(&sad::Window::popRect));
        c->addMethod("isGL3compatible",   sad::dukpp03::bind_method::from(&sad::Window::isGL3compatible));

        ctx->addClassBinding("sad::Window", c);        
    }
}

