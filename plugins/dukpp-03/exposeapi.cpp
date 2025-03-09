#include "dukpp-03/context.h"
#include "dukpp-03/renderer.h"
#include "dukpp-03/thread.h"
#include "dukpp-03/mutex.h"
#include "dukpp-03/semaphore.h"
#include "dukpp-03/jscontrols.h"
#include "dukpp-03/jspipelinestep.h"
#include "dukpp-03/jspipelinedelayedtask.h"

#include <sadpoint.h>
#include <sadsize.h>
#include <sadrect.h>
#include <timer.h>
#include <p2d/vector.h>
#include <slurp.h>
#include <spit.h>
#include <mousecursor.h>
#include <opengl.h>
#include <sprite2d.h>
#include <way.h>

#include <renderer.h>
#include <window.h>
#include <util/fs.h>

#include <cstdio>

#include <settings.h>

#include <log/log.h>
#include <log/consoletarget.h>
#include <log/filetarget.h>

#include <resource/tree.h>
#include <resource/resourcefile.h>

#include <db/dbtable.h>
#include <db/dbdatabase.h>

#include <input/events.h>

#include <pipeline/pipeline.h>

#include <keycodes.h>


template<typename T> static void __add_target_to_log(sad::log::Log* lg, T* a)
{
    lg->addTarget(a);
}

template<typename T> static void __remove_target_from_log(sad::log::Log* lg, T* a)
{
    lg->removeTarget(a);
}


// sad.pipeline.beforeScene

static void __before_scene_each_frame(
    sad::Renderer* r,
    const sad::String& name, 
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction function
)
{
    sad::pipeline::Step* step = new sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_EACH_FRAME>(
        ctx,
        function
    );
    step->mark(name);
    step->setSource(sad::pipeline::StepSource::ST_USER);
    r->pipeline()->insertBefore("sad::Renderer::renderScenes", step, name);
}


static void __before_scene_each_ms(
    sad::Renderer* r,
    const sad::String& name, 
    int interval,
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction function
)
{
    sad::pipeline::Step* step = new sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_EACH_MS>(
        interval,
        ctx,
        function
    );
    step->mark(name);
    step->setSource(sad::pipeline::StepSource::ST_USER);
    r->pipeline()->insertBefore("sad::Renderer::renderScenes", step, name);
}

static void __before_scene_one_shot(
    sad::Renderer* r,
    const sad::String& name, 
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction function
)
{
    sad::pipeline::Step* step = new sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_ONE_SHOT>(
        ctx,
        function
    );
    step->mark(name);
    step->setSource(sad::pipeline::StepSource::ST_USER);
    r->pipeline()->insertBefore("sad::Renderer::renderScenes", step, name);
}



static void __before_scene_delayed(
    sad::Renderer* r,
    const sad::String& name, 
    int interval,
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction function
)
{
    sad::pipeline::Step* step = new sad::dukpp03::JSPipelineDelayedTask(
        interval,
        ctx,
        function
    );
    step->mark(name);
    step->setSource(sad::pipeline::StepSource::ST_USER);
    r->pipeline()->insertBefore("sad::Renderer::renderScenes", step, name);
}


// sad.pipeline.afterScene


static void __after_scene_each_frame(
    sad::Renderer* r,
    const sad::String& name, 
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction function
)
{
    sad::pipeline::Step* step = new sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_EACH_FRAME>(
        ctx,
        function
    );
    step->mark(name);
    step->setSource(sad::pipeline::StepSource::ST_USER);
    r->pipeline()->insertAfter("sad::animations::Animations::process", step, name);
}


static void __after_scene_each_ms(
    sad::Renderer* r,
    const sad::String& name, 
    int interval,
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction function
)
{
    sad::pipeline::Step* step = new sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_EACH_MS>(
        interval,
        ctx,
        function
    );
    step->mark(name);
    step->setSource(sad::pipeline::StepSource::ST_USER);
    r->pipeline()->insertAfter("sad::animations::Animations::process", step, name);
}

static void __after_scene_one_shot(
    sad::Renderer* r,
    const sad::String& name, 
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction function
)
{
    sad::pipeline::Step* step = new sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_ONE_SHOT>(
        ctx,
        function
    );
    step->mark(name);
    step->setSource(sad::pipeline::StepSource::ST_USER);
    r->pipeline()->insertAfter("sad::animations::Animations::process", step, name);
}



static void __after_scene_delayed(
    sad::Renderer* r,
    const sad::String& name, 
    int interval,
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction function
)
{
    sad::pipeline::Step* step = new sad::dukpp03::JSPipelineDelayedTask(
        interval,
        ctx,
        function
    );
    step->mark(name);
    step->setSource(sad::pipeline::StepSource::ST_USER);
    r->pipeline()->insertAfter("sad::animations::Animations::process", step, name);
}


// sad.pipeline.beforeEvent

static void __before_event_each_frame(
    sad::Renderer* r,
    const sad::String& event_name,
    const sad::String& name, 
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction function
)
{
    sad::pipeline::Step* step = new sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_EACH_FRAME>(
        ctx,
        function
    );
    step->mark(name);
    step->setSource(sad::pipeline::StepSource::ST_USER);
    r->pipeline()->insertBefore(event_name, step, name);
}


static void __before_event_each_ms(
    sad::Renderer* r,
    const sad::String& event_name,
    const sad::String& name, 
    int interval,
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction function
)
{
    sad::pipeline::Step* step = new sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_EACH_MS>(
        interval,
        ctx,
        function
    );
    step->mark(name);
    step->setSource(sad::pipeline::StepSource::ST_USER);
    r->pipeline()->insertBefore(event_name, step, name);
}

static void __before_event_one_shot(
    sad::Renderer* r,
    const sad::String& event_name,
    const sad::String& name, 
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction function
)
{
    sad::pipeline::Step* step = new sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_ONE_SHOT>(
        ctx,
        function
    );
    step->mark(name);
    step->setSource(sad::pipeline::StepSource::ST_USER);
    r->pipeline()->insertBefore(event_name, step, name);
}



static void __before_event_delayed(
    sad::Renderer* r,
    const sad::String& event_name,
    const sad::String& name, 
    int interval,
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction function
)
{
    sad::pipeline::Step* step = new sad::dukpp03::JSPipelineDelayedTask(
        interval,
        ctx,
        function
    );
    step->mark(name);
    step->setSource(sad::pipeline::StepSource::ST_USER);
    r->pipeline()->insertBefore(event_name, step, name);
}


// sad.pipeline.afterEvent

static void __after_event_each_frame(
    sad::Renderer* r,
    const sad::String& event_name,
    const sad::String& name, 
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction function
)
{
    sad::pipeline::Step* step = new sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_EACH_FRAME>(
        ctx,
        function
    );
    step->mark(name);
    step->setSource(sad::pipeline::StepSource::ST_USER);
    r->pipeline()->insertAfter(event_name, step, name);
}


static void __after_event_each_ms(
    sad::Renderer* r,
    const sad::String& event_name,
    const sad::String& name, 
    int interval,
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction function
)
{
    sad::pipeline::Step* step = new sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_EACH_MS>(
        interval,
        ctx,
        function
    );
    step->mark(name);
    step->setSource(sad::pipeline::StepSource::ST_USER);
    r->pipeline()->insertAfter(event_name, step, name);
}

static void __after_event_one_shot(
    sad::Renderer* r,
    const sad::String& event_name,
    const sad::String& name, 
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction function
)
{
    sad::pipeline::Step* step = new sad::dukpp03::JSPipelineStep<sad::dukpp03::JSPipelineStepType::SDJST_ONE_SHOT>(
        ctx,
        function
    );
    step->mark(name);
    step->setSource(sad::pipeline::StepSource::ST_USER);
    r->pipeline()->insertAfter(event_name, step, name);
}



static void __after_event_delayed(
    sad::Renderer* r,
    const sad::String& event_name,
    const sad::String& name, 
    int interval,
    sad::dukpp03::Context* ctx, 
    sad::dukpp03::CompiledFunction function
)
{
    sad::pipeline::Step* step = new sad::dukpp03::JSPipelineDelayedTask(
        interval,
        ctx,
        function
    );
    step->mark(name);
    step->setSource(sad::pipeline::StepSource::ST_USER);
    r->pipeline()->insertAfter(event_name, step, name);
}

static void exposeWay(sad::dukpp03::Context* ctx);

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
     // Exposing sad::Log   
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::log::Log>("SadLog");
        void (sad::log::Log::*f)(const char*, const char*, int) = &sad::log::Log::fatal;
        c->addMethod("fatal",  sad::dukpp03::bind_method::from(f));
        f = &sad::log::Log::critical;
        c->addMethod("critical",  sad::dukpp03::bind_method::from(f));
        f = &sad::log::Log::warning;
        c->addMethod("warning",  sad::dukpp03::bind_method::from(f));
        f = &sad::log::Log::message;
        c->addMethod("message",  sad::dukpp03::bind_method::from(f));
        f = &sad::log::Log::debug;
        c->addMethod("debug",  sad::dukpp03::bind_method::from(f));
        void (sad::log::Log::*g)(const char*, const char*, int, const sad::String&) = &sad::log::Log::user;
        c->addMethod("user",  sad::dukpp03::bind_method::from(g));
        f = &sad::log::Log::saddyInternal<sad::String>;
        c->addMethod("internal",  sad::dukpp03::bind_method::from(f));

        void (sad::log::Log::*p1)(const sad::String&) = &sad::log::Log::pushSubsystem;
        void (sad::log::Log::*p2)(const sad::String&, const char*, int) = &sad::log::Log::pushSubsystem;
        c->addMethod("pushSubsystem",  sad::dukpp03::bind_method::from(p1));
        c->addMethod("pushSubsystem",  sad::dukpp03::bind_method::from(p2));
        c->addMethod("popSubsystem",  sad::dukpp03::bind_method::from(&sad::log::Log::popSubsystem));        
        c->setPrototypeFunction("SadLog");

        ctx->registerCallable("SadLogRef", sad::dukpp03::make_function::from(sad::log::Log::ref));

        ctx->addClassBinding("sad::log::Log", c);
    }
    // Exposing sad::log::ConsoleTarget
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::log::ConsoleTarget>("SadLogConsoleTarget");
        c->addObjectConstructor<sad::log::ConsoleTarget, sad::String>("SadLogConsoleTarget");
        c->addObjectConstructor<sad::log::ConsoleTarget, sad::String, int>("SadLogConsoleTarget");
        c->addObjectConstructor<sad::log::ConsoleTarget, sad::String, int, bool>("SadLogConsoleTarget");
        c->addObjectConstructor<sad::log::ConsoleTarget, sad::String, int, bool, bool>("SadLogConsoleTarget");       
        c->setPrototypeFunction("SadLogConsoleTarget");

        ctx->addClassBinding("sad::log::ConsoleTarget", c);
    }
    // Exposing sad::log::FileTarget
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::log::FileTarget>("SadLogFileTarget");
        c->addObjectConstructor<sad::log::FileTarget, sad::String>("SadLogFileTarget");
        c->addObjectConstructor<sad::log::FileTarget, sad::String, int>("SadLogFileTarget");
        c->addMethod("open",  sad::dukpp03::bind_method::from(&sad::log::FileTarget::open));
        c->setPrototypeFunction("SadLogFileTarget");

        ctx->addClassBinding("sad::log::FileTarget", c);
    }
    // Exposing addTarget and removeTarget
    {
        ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>* addTarget = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
        addTarget->add(sad::dukpp03::make_function::from(__add_target_to_log<sad::log::ConsoleTarget>));
        addTarget->add(sad::dukpp03::make_function::from(__add_target_to_log<sad::log::FileTarget>));
        ctx->registerCallable("SadInternalLogAddTarget", addTarget);

        ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>* removeTarget = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
        removeTarget->add(sad::dukpp03::make_function::from(__remove_target_from_log<sad::log::ConsoleTarget>));
        removeTarget->add(sad::dukpp03::make_function::from(__remove_target_from_log<sad::log::FileTarget>));
        ctx->registerCallable("SadInternalLogRemoveTarget", removeTarget);
    }
    // Exposing sad::Mutex
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::dukpp03::Mutex>("SadMutex");
        c->addMethod("lock",   sad::dukpp03::rebind_method::to<sad::dukpp03::Mutex>::from(&sad::Mutex::lock));
        c->addMethod("unlock",   sad::dukpp03::rebind_method::to<sad::dukpp03::Mutex>::from(&sad::Mutex::unlock));
        c->setPrototypeFunction("SadMutex");

        ctx->addClassBinding("sad::dukpp03::Mutex", c);        
    }
    // Exposing sad::Semaphore
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::dukpp03::Semaphore>("SadSemaphore");
        c->addObjectConstructor<sad::dukpp03::Semaphore, unsigned int>("SadSemaphore");
        c->addObjectConstructor<sad::dukpp03::Semaphore, unsigned int, unsigned int>("SadSemaphore");
        c->addMethod("consume",   sad::dukpp03::rebind_method::to<sad::dukpp03::Semaphore>::from(&sad::Semaphore::consume));
        c->addMethod("release",   sad::dukpp03::rebind_method::to<sad::dukpp03::Semaphore>::from(&sad::Semaphore::release));
        c->addMethod("value",   sad::dukpp03::rebind_method::to<sad::dukpp03::Semaphore>::from(&sad::Semaphore::value));
        c->setPrototypeFunction("SadSemaphore");


        ctx->addClassBinding("sad::dukpp03::Semaphore", c);        
    }
    // Exposing sad::MouseCursor
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::MouseCursor>("SadMouseCursor");
        c->addMethod("position",   sad::dukpp03::bind_method::from(&sad::MouseCursor::position));
        c->addMethod("setPosition",   sad::dukpp03::bind_method::from(&sad::MouseCursor::setPosition));
        c->addMethod("show",   sad::dukpp03::bind_method::from(&sad::MouseCursor::show));
        c->addMethod("hide",   sad::dukpp03::bind_method::from(&sad::MouseCursor::hide));

        void (sad::MouseCursor::*f)(sad::Sprite2D*) = &sad::MouseCursor::setImage;
        c->addMethod("setImage",   sad::dukpp03::bind_method::from(f));

        c->addMethod("clearCursorImage",   sad::dukpp03::bind_method::from(&sad::MouseCursor::clearCursorImage));
        c->setPrototypeFunction("SadMouseCursor");

        ctx->addClassBinding("sad::MouseCursor", c);        
    }
    // Exposing sad::OpenGL
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addMethod("version",   sad::dukpp03::bind_method::from(&sad::OpenGL::version));
        c->addMethod("versionAsDouble",   sad::dukpp03::bind_method::from(&sad::OpenGL::versionAsDouble));
        c->addMethod("vendor",   sad::dukpp03::bind_method::from(&sad::OpenGL::vendor));
        c->addMethod("versionAsString",   sad::dukpp03::bind_method::from(&sad::OpenGL::versionAsString));
        c->addMethod("rendererString",   sad::dukpp03::bind_method::from(&sad::OpenGL::rendererString));
        c->addMethod("glslShadingLanguageVersion",   sad::dukpp03::bind_method::from(&sad::OpenGL::glslShadingLanguageVersion));
        c->addMethod("extensions",   sad::dukpp03::bind_method::from(&sad::OpenGL::extensions));
        c->addMethod("supportsExtension",   sad::dukpp03::bind_method::from(&sad::OpenGL::supportsExtension));

        ctx->addClassBinding("sad::OpenGL", c);        
    }
    // Exposing sad::resource::Tree
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::resource::Tree>("SadResourceTree");
        c->addObjectConstructor<sad::resource::Tree, sad::Renderer*>("SadResourceTree");
        c->addObjectConstructor<sad::resource::Tree, sad::dukpp03::Renderer*>("SadResourceTree");
        c->addMethod("tryLoadFromString",   sad::dukpp03::bind_method::from(&sad::resource::Tree::tryLoadFromString));
        c->addMethod("tryLoadFromFile",   sad::dukpp03::bind_method::from(&sad::resource::Tree::tryLoadFromFile));
        
        bool (sad::resource::Tree::*f)(const sad::String&) = &sad::resource::Tree::unload;
        c->addMethod("unload",   sad::dukpp03::bind_method::from(f));

        bool (sad::resource::Tree::*g)(sad::resource::ResourceFile*) = &sad::resource::Tree::unload;
        c->addMethod("unload",   sad::dukpp03::bind_method::from(g));

        c->addMethod("file",   sad::dukpp03::bind_method::from(&sad::resource::Tree::file));
        c->addMethod("files",   sad::dukpp03::bind_method::from(&sad::resource::Tree::files));
        c->addMethod("renderer",   sad::dukpp03::bind_method::from(&sad::resource::Tree::renderer));
        c->addMethod("setRenderer",   sad::dukpp03::bind_method::from(&sad::resource::Tree::setRenderer));
        c->addMethod("shouldStoreLinks",   sad::dukpp03::bind_method::from(&sad::resource::Tree::shouldStoreLinks));
        c->addMethod("setStoreLinks",   sad::dukpp03::bind_method::from(&sad::resource::Tree::setStoreLinks));
        c->addMethod("subtrees", sad::dukpp03::bind_method::from(&sad::resource::Tree::subtrees));
        c->addMethod("addSubtree", sad::dukpp03::bind_method::from(&sad::resource::Tree::addSubtree));
        c->addMethod("removeSubtree", sad::dukpp03::bind_method::from(&sad::resource::Tree::removeSubtree));

        c->setPrototypeFunction("SadResourceTree");

        ctx->addClassBinding("sad::resource::Tree", c);
    }
    // Exposing sad::resource::File
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addMethod("isAnonymous",  sad::dukpp03::bind_method::from(&sad::resource::ResourceFile::isAnonymous));
        c->addMethod("name",  sad::dukpp03::bind_method::from(&sad::resource::ResourceFile::name));
        c->addMethod("setName",  sad::dukpp03::bind_method::from(&sad::resource::ResourceFile::setName));
        c->addMethod("tryLoad",  sad::dukpp03::bind_method::from(&sad::resource::ResourceFile::tryLoad));
        c->addMethod("tryReload",  sad::dukpp03::bind_method::from(&sad::resource::ResourceFile::tryReload));

        ctx->addClassBinding("sad::resource::ResourceFile", c);  
    }
    // Exposing sad::db::Database
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::db::Database>("SadDBDatabase");
        c->setPrototypeFunction("SadDBDatabase");
        sad::String (sad::db::Database::*save)() = &sad::db::Database::save;
        c->addMethod("save",  sad::dukpp03::bind_method::from(save));
        c->addMethod("saveToFile",  sad::dukpp03::bind_method::from(&sad::db::Database::saveToFile));
        c->addMethod("load",  sad::dukpp03::bind_method::from(&sad::db::Database::load));
        c->addMethod("loadFromFile",  sad::dukpp03::bind_method::from(&sad::db::Database::loadFromFile));
        c->addMethod("tryLoadFrom",  sad::dukpp03::bind_method::from(&sad::db::Database::loadFromFile));
        c->addMethod("addPropertyOfType",  sad::dukpp03::bind_method::from(&sad::db::Database::addPropertyOfType));
        c->addMethod("removeProperty",  sad::dukpp03::bind_method::from(&sad::db::Database::removeProperty));
        c->addMethod("setDBProperty",  sad::dukpp03::bind_method::from(&sad::db::Database::setDBProperty));
        c->addMethod("getDBProperty",  sad::dukpp03::bind_method::from(&sad::db::Database::getDBProperty));
        c->addMethod("hasDBProperty",  sad::dukpp03::bind_method::from(&sad::db::Database::hasDBProperty));
        c->addMethod("addTable",  sad::dukpp03::bind_method::from(&sad::db::Database::addTable));
        c->addMethod("removeTable",  sad::dukpp03::bind_method::from(&sad::db::Database::removeTable));
        c->addMethod("table",  sad::dukpp03::bind_method::from(&sad::db::Database::table));
        c->addMethod("queryByName",  sad::dukpp03::bind_method::from(&sad::db::Database::queryByName));
        c->addMethod("queryByMinorId",  sad::dukpp03::bind_method::from(&sad::db::Database::queryByMinorId));
        c->addMethod("queryByMajorId",  sad::dukpp03::bind_method::from(&sad::db::Database::queryByMajorId));
        c->addMethod("tableList",  sad::dukpp03::bind_method::from(&sad::db::Database::tableList));
        c->addMethod("setRenderer",  sad::dukpp03::bind_method::from(&sad::db::Database::setRenderer));
        c->addMethod("renderer",  sad::dukpp03::bind_method::from(&sad::db::Database::renderer));
        c->addMethod("setDefaultTreeName",  sad::dukpp03::bind_method::from(&sad::db::Database::setDefaultTreeName));
        c->addMethod("defaultTreeName",  sad::dukpp03::bind_method::from(&sad::db::Database::defaultTreeName));
        c->addMethod("saveSnapshot",  sad::dukpp03::bind_method::from(&sad::db::Database::saveSnapshot));
        c->addMethod("snapshotsCount",  sad::dukpp03::bind_method::from(&sad::db::Database::snapshotsCount));
        c->addMethod("restoreSnapshot",  sad::dukpp03::bind_method::from(&sad::db::Database::restoreSnapshot));
        c->addMethod("tablesAreEmpty",  sad::dukpp03::bind_method::from(&sad::db::Database::tablesAreEmpty));
        c->addMethod("propertyNames",  sad::dukpp03::bind_method::from(&sad::db::Database::propertyNames));

        sad::db::Object* (sad::db::Database::*objectByName)(const sad::String &) const = &sad::db::Database::objectByName; 
        c->addMethod("objectByName",  sad::dukpp03::bind_method::from(objectByName));

        ctx->addClassBinding("sad::db::Database", c);  
    }
    // Exposing sad::db::Table
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::db::Table>("SadDBTable");
        c->setPrototypeFunction("SadDBTable");
        c->addMethod("add",  sad::dukpp03::bind_method::from(&sad::db::Table::add));
        c->addMethod("remove",  sad::dukpp03::bind_method::from(&sad::db::Table::remove));
        c->addMethod("queryById",  sad::dukpp03::bind_method::from(&sad::db::Table::queryById));
        c->addMethod("queryByMinorId",  sad::dukpp03::bind_method::from(&sad::db::Table::queryByMinorId));
        c->addMethod("queryByName",  sad::dukpp03::bind_method::from(&sad::db::Table::queryByName));
        c->addMethod("objectByName",  sad::dukpp03::bind_method::from(&sad::db::Table::objectByName));
        c->addMethod("queryByMajorId",  sad::dukpp03::bind_method::from(&sad::db::Table::queryByMajorId));
        c->addMethod("database",  sad::dukpp03::bind_method::from(&sad::db::Table::database));
        c->addMethod("setDatabase",  sad::dukpp03::bind_method::from(&sad::db::Table::setDatabase));
        c->addMethod("objectList",  sad::dukpp03::bind_method::from(&sad::db::Table::objectList));
        c->addMethod("objectListOfType",  sad::dukpp03::bind_method::from(&sad::db::Table::objectListOfType));
        c->addMethod("clear",  sad::dukpp03::bind_method::from(&sad::db::Table::clear));
        c->addMethod("empty",  sad::dukpp03::bind_method::from(&sad::db::Table::empty));
        
        ctx->addClassBinding("sad::db::Table", c);  
    }

    // Exposing events
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::input::QuitEvent>("SadInputQuitEvent");
        c->setPrototypeFunction("SadInputQuitEvent");
        ctx->addClassBinding("sad::input::QuitEvent", c);  
    }
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::input::MouseLeaveEvent>("SadInputMouseLeaveEvent");
        c->setPrototypeFunction("SadInputMouseLeaveEvent");
        ctx->addClassBinding("sad::input::MouseLeaveEvent", c);  
    }
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::input::ActivateEvent>("SadInputActivateEvent");
        c->setPrototypeFunction("SadInputActivateEvent");
        ctx->addClassBinding("sad::input::ActivateEvent", c);  
    }
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::input::DeactivateEvent>("SadInputDeactivateEvent");
        c->setPrototypeFunction("SadInputDeactivateEvent");
        ctx->addClassBinding("sad::input::DeactivateEvent", c);  
    }
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::input::KeyPressEvent>("SadInputKeyPressEvent");
        c->setPrototypeFunction("SadInputKeyPressEvent");
        c->addMethod("key", sad::dukpp03::rebind_method::to<sad::input::KeyPressEvent>::from(&sad::input::KeyEvent::key));
        c->addAccessor("readableKey", sad::dukpp03::getter::to<sad::input::KeyPressEvent>::from(&sad::input::KeyEvent::ReadableKey), sad::dukpp03::setter::to<sad::input::KeyPressEvent>::from(&sad::input::KeyEvent::ReadableKey));
        c->addAccessor("altHeld", sad::dukpp03::getter::to<sad::input::KeyPressEvent>::from(&sad::input::KeyEvent::AltHeld), sad::dukpp03::setter::to<sad::input::KeyPressEvent>::from(&sad::input::KeyEvent::AltHeld));
        c->addAccessor("shiftHeld", sad::dukpp03::getter::to<sad::input::KeyPressEvent>::from(&sad::input::KeyEvent::ShiftHeld), sad::dukpp03::setter::to<sad::input::KeyPressEvent>::from(&sad::input::KeyEvent::ShiftHeld));
        c->addAccessor("ctrlHeld", sad::dukpp03::getter::to<sad::input::KeyPressEvent>::from(&sad::input::KeyEvent::CtrlHeld), sad::dukpp03::setter::to<sad::input::KeyPressEvent>::from(&sad::input::KeyEvent::CtrlHeld));

        ctx->addClassBinding("sad::input::KeyPressEvent", c);  	    
    }
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::input::KeyReleaseEvent>("SadInputKeyReleaseEvent");
        c->setPrototypeFunction("SadInputKeyReleaseEvent");
        c->addMethod("key", sad::dukpp03::rebind_method::to<sad::input::KeyReleaseEvent>::from(&sad::input::KeyEvent::key));
        c->addAccessor("readableKey", sad::dukpp03::getter::to<sad::input::KeyReleaseEvent>::from(&sad::input::KeyEvent::ReadableKey), sad::dukpp03::setter::to<sad::input::KeyReleaseEvent>::from(&sad::input::KeyEvent::ReadableKey));
        c->addAccessor("altHeld", sad::dukpp03::getter::to<sad::input::KeyReleaseEvent>::from(&sad::input::KeyEvent::AltHeld), sad::dukpp03::setter::to<sad::input::KeyReleaseEvent>::from(&sad::input::KeyEvent::AltHeld));
        c->addAccessor("shiftHeld", sad::dukpp03::getter::to<sad::input::KeyReleaseEvent>::from(&sad::input::KeyEvent::ShiftHeld), sad::dukpp03::setter::to<sad::input::KeyReleaseEvent>::from(&sad::input::KeyEvent::ShiftHeld));
        c->addAccessor("ctrlHeld", sad::dukpp03::getter::to<sad::input::KeyReleaseEvent>::from(&sad::input::KeyEvent::CtrlHeld), sad::dukpp03::setter::to<sad::input::KeyReleaseEvent>::from(&sad::input::KeyEvent::CtrlHeld));

        ctx->addClassBinding("sad::input::KeyReleaseEvent", c);  	    
    }
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::input::MouseMoveEvent>("SadInputMouseMoveEvent");
        c->setPrototypeFunction("SadInputMouseMoveEvent");
        c->addMethod("pos2D", sad::dukpp03::rebind_method::to<sad::input::MouseMoveEvent>::from(&sad::input::MouseCursorEvent::pos2D));
        c->addMethod("setPoint", sad::dukpp03::rebind_method::to<sad::input::MouseMoveEvent>::from(&sad::input::MouseCursorEvent::setPoint));
        c->addAccessor("Point3D", sad::dukpp03::getter::to<sad::input::MouseMoveEvent>::from(&sad::input::MouseCursorEvent::Point3D), sad::dukpp03::setter::to<sad::input::MouseMoveEvent>::from(&sad::input::MouseCursorEvent::Point3D));
        c->addAccessor("Point", sad::dukpp03::getter::to<sad::input::MouseMoveEvent>::from(&sad::input::MouseCursorEvent::Point), sad::dukpp03::setter::to<sad::input::MouseMoveEvent>::from(&sad::input::MouseCursorEvent::Point));

        ctx->addClassBinding("sad::input::MouseMoveEvent", c);  	    
    }
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::input::MouseEnterEvent>("SadInputMouseEnterEvent");
        c->setPrototypeFunction("SadInputMouseEnterEvent");
        c->addMethod("pos2D", sad::dukpp03::rebind_method::to<sad::input::MouseEnterEvent>::from(&sad::input::MouseCursorEvent::pos2D));
        c->addMethod("setPoint", sad::dukpp03::rebind_method::to<sad::input::MouseEnterEvent>::from(&sad::input::MouseCursorEvent::setPoint));
        c->addAccessor("Point3D", sad::dukpp03::getter::to<sad::input::MouseEnterEvent>::from(&sad::input::MouseCursorEvent::Point3D), sad::dukpp03::setter::to<sad::input::MouseEnterEvent>::from(&sad::input::MouseCursorEvent::Point3D));
        c->addAccessor("Point", sad::dukpp03::getter::to<sad::input::MouseEnterEvent>::from(&sad::input::MouseCursorEvent::Point), sad::dukpp03::setter::to<sad::input::MouseEnterEvent>::from(&sad::input::MouseCursorEvent::Point));

        ctx->addClassBinding("sad::input::MouseEnterEvent", c);  	    
    }
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::input::MousePressEvent>("SadInputMousePressEvent");
        c->setPrototypeFunction("SadInputMousePressEvent");
        c->addMethod("pos2D", sad::dukpp03::rebind_method::to<sad::input::MousePressEvent>::from(&sad::input::MouseCursorEvent::pos2D));
        c->addMethod("setPoint", sad::dukpp03::rebind_method::to<sad::input::MousePressEvent>::from(&sad::input::MouseCursorEvent::setPoint));
        c->addMethod("button", sad::dukpp03::rebind_method::to<sad::input::MousePressEvent>::from(&sad::input::MouseEvent::button));

        c->addAccessor("Point3D", sad::dukpp03::getter::to<sad::input::MousePressEvent>::from(&sad::input::MouseCursorEvent::Point3D), sad::dukpp03::setter::to<sad::input::MousePressEvent>::from(&sad::input::MouseCursorEvent::Point3D));
        c->addAccessor("Point", sad::dukpp03::getter::to<sad::input::MousePressEvent>::from(&sad::input::MouseCursorEvent::Point), sad::dukpp03::setter::to<sad::input::MousePressEvent>::from(&sad::input::MouseCursorEvent::Point));

        ctx->addClassBinding("sad::input::MousePressEvent", c);  	    
    }
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::input::MouseReleaseEvent>("SadInputMouseReleaseEvent");
        c->setPrototypeFunction("SadInputMouseReleaseEvent");
        c->addMethod("pos2D", sad::dukpp03::rebind_method::to<sad::input::MouseReleaseEvent>::from(&sad::input::MouseCursorEvent::pos2D));
        c->addMethod("setPoint", sad::dukpp03::rebind_method::to<sad::input::MouseReleaseEvent>::from(&sad::input::MouseCursorEvent::setPoint));
        c->addMethod("button", sad::dukpp03::rebind_method::to<sad::input::MouseReleaseEvent>::from(&sad::input::MouseEvent::button));

        c->addAccessor("Point3D", sad::dukpp03::getter::to<sad::input::MouseReleaseEvent>::from(&sad::input::MouseCursorEvent::Point3D), sad::dukpp03::setter::to<sad::input::MouseReleaseEvent>::from(&sad::input::MouseCursorEvent::Point3D));
        c->addAccessor("Point", sad::dukpp03::getter::to<sad::input::MouseReleaseEvent>::from(&sad::input::MouseCursorEvent::Point), sad::dukpp03::setter::to<sad::input::MouseReleaseEvent>::from(&sad::input::MouseCursorEvent::Point));

        ctx->addClassBinding("sad::input::MouseReleaseEvent", c);  	    
    }
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::input::MouseDoubleClickEvent>("SadInputMouseDoubleClickEvent");
        c->setPrototypeFunction("SadInputMouseDoubleClickEvent");
        c->addMethod("pos2D", sad::dukpp03::rebind_method::to<sad::input::MouseDoubleClickEvent>::from(&sad::input::MouseCursorEvent::pos2D));
        c->addMethod("setPoint", sad::dukpp03::rebind_method::to<sad::input::MouseDoubleClickEvent>::from(&sad::input::MouseCursorEvent::setPoint));
        c->addMethod("button", sad::dukpp03::rebind_method::to<sad::input::MouseDoubleClickEvent>::from(&sad::input::MouseEvent::button));

        c->addAccessor("Point3D", sad::dukpp03::getter::to<sad::input::MouseDoubleClickEvent>::from(&sad::input::MouseCursorEvent::Point3D), sad::dukpp03::setter::to<sad::input::MouseDoubleClickEvent>::from(&sad::input::MouseCursorEvent::Point3D));
        c->addAccessor("Point", sad::dukpp03::getter::to<sad::input::MouseDoubleClickEvent>::from(&sad::input::MouseCursorEvent::Point), sad::dukpp03::setter::to<sad::input::MouseDoubleClickEvent>::from(&sad::input::MouseCursorEvent::Point));

        ctx->addClassBinding("sad::input::MouseDoubleClickEvent", c);
    }
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::input::MouseWheelEvent>("SadInputMouseWheelEvent");
        c->setPrototypeFunction("SadInputMouseWheelEvent");
        c->addMethod("pos2D", sad::dukpp03::rebind_method::to<sad::input::MouseWheelEvent>::from(&sad::input::MouseCursorEvent::pos2D));
        c->addMethod("setPoint", sad::dukpp03::rebind_method::to<sad::input::MouseWheelEvent>::from(&sad::input::MouseCursorEvent::setPoint));    
        c->addAccessor("Point3D", sad::dukpp03::getter::to<sad::input::MouseWheelEvent>::from(&sad::input::MouseCursorEvent::Point3D), sad::dukpp03::setter::to<sad::input::MouseWheelEvent>::from(&sad::input::MouseCursorEvent::Point3D));
        c->addAccessor("Point", sad::dukpp03::getter::to<sad::input::MouseWheelEvent>::from(&sad::input::MouseCursorEvent::Point), sad::dukpp03::setter::to<sad::input::MouseWheelEvent>::from(&sad::input::MouseCursorEvent::Point));
        c->addAccessor("Delta", sad::dukpp03::getter::from(&sad::input::MouseWheelEvent::Delta), sad::dukpp03::setter::from(&sad::input::MouseWheelEvent::Delta));

        ctx->addClassBinding("sad::input::MouseWheelEvent", c);
    }
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::input::ResizeEvent>("SadInputResizeEvent");
        c->setPrototypeFunction("SadInputResizeEvent");
        c->addAccessor("OldSize", sad::dukpp03::getter::from(&sad::input::ResizeEvent::OldSize), sad::dukpp03::setter::from(&sad::input::ResizeEvent::OldSize));
        c->addAccessor("NewSize", sad::dukpp03::getter::from(&sad::input::ResizeEvent::NewSize), sad::dukpp03::setter::from(&sad::input::ResizeEvent::NewSize));

        ctx->addClassBinding("sad::input::ResizeEvent", c);  	    
    }
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addConstructor<sad::input::EmergencyShutdownEvent>("SadInputEmergencyShutdownEvent");
        c->setPrototypeFunction("SadInputEmergencyShutdownEvent");
        ctx->addClassBinding("sad::input::EmergencyShutdownEvent", c);
    }
    ctx->registerCallable("SadIsValidKeyboardKey", sad::dukpp03::make_function::from(sad::isValidKeyboardKey));
    ctx->registerCallable("SadDefaultKeyIfNotValid", sad::dukpp03::make_function::from(sad::defaultKeyIfNotValidAsInt));
    ctx->registerCallable("SadKeyToStringAsInt", sad::dukpp03::make_function::from(sad::keyToStringAsInt));
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        c->addObjectConstructor<sad::dukpp03::JSControls>("SadInputJSControls");
        c->setPrototypeFunction("SadInputJSControls");
        c->addMethod("addRenderer", sad::dukpp03::bind_method::from(&sad::dukpp03::JSControls::addToRenderer));
        c->addMethod("enable", sad::dukpp03::bind_method::from(&sad::dukpp03::JSControls::enable));
        c->addMethod("disable", sad::dukpp03::bind_method::from(&sad::dukpp03::JSControls::disable));
        c->addMethod("enabled", sad::dukpp03::bind_method::from(&sad::dukpp03::JSControls::enabled));
        c->addMethod("onEvent", sad::dukpp03::bind_method::from(&sad::dukpp03::JSControls::onEvent));
        c->addMethod("unbind", sad::dukpp03::bind_method::from(&sad::dukpp03::JSControls::unbind));

        ctx->addClassBinding("sad::dukpp03::JSControls", c);  
    }
    // A pipeline binding
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
        void (sad::pipeline::Pipeline::*f)(const sad::String&) = &sad::pipeline::Pipeline::removeByMarkWith;
        c->addMethod("removeByMarkWith", sad::dukpp03::bind_method::from(f));
        c->addMethod("enableByMark", sad::dukpp03::bind_method::from(&sad::pipeline::Pipeline::enableByMark));
        c->addMethod("disableByMark", sad::dukpp03::bind_method::from(&sad::pipeline::Pipeline::disableByMark));
        c->addMethod("isStepEnabled", sad::dukpp03::bind_method::from(&sad::pipeline::Pipeline::isStepEnabled));


        ctx->addClassBinding("sad::pipeline::Pipeline", c);  

        // Before scene
        ctx->registerCallable(
            "SadPipelineBeforeSceneEachFrame", 
            sad::dukpp03::make_function::from(__before_scene_each_frame)
        );
        ctx->registerCallable(
            "SadPipelineBeforeSceneEachMS", 
            sad::dukpp03::make_function::from(__before_scene_each_ms)
        );
        ctx->registerCallable(
            "SadPipelineBeforeSceneOneShot", 
            sad::dukpp03::make_function::from(__before_scene_one_shot)
        );
        ctx->registerCallable(
            "SadPipelineBeforeSceneDelayed", 
            sad::dukpp03::make_function::from(__before_scene_delayed)
        );

        // After scene
        ctx->registerCallable(
            "SadPipelineAfterSceneEachFrame", 
            sad::dukpp03::make_function::from(__after_scene_each_frame)
        );
        ctx->registerCallable(
            "SadPipelineAfterSceneEachMS", 
            sad::dukpp03::make_function::from(__after_scene_each_ms)
        );
        ctx->registerCallable(
            "SadPipelineAfterSceneOneShot", 
            sad::dukpp03::make_function::from(__after_scene_one_shot)
        );
        ctx->registerCallable(
            "SadPipelineAfterSceneDelayed", 
            sad::dukpp03::make_function::from(__after_scene_delayed)
        );

        // Before event
        ctx->registerCallable(
            "SadPipelineBeforeEventEachFrame", 
            sad::dukpp03::make_function::from(__before_event_each_frame)
        );
        ctx->registerCallable(
            "SadPipelineBeforeEventEachMS", 
            sad::dukpp03::make_function::from(__before_event_each_ms)
        );
        ctx->registerCallable(
            "SadPipelineBeforeEventOneShot", 
            sad::dukpp03::make_function::from(__before_event_one_shot)
        );
        ctx->registerCallable(
            "SadPipelineBeforeEventDelayed", 
            sad::dukpp03::make_function::from(__before_event_delayed)
        );

        // After event
        ctx->registerCallable(
            "SadPipelineAfterEventEachFrame", 
            sad::dukpp03::make_function::from(__after_event_each_frame)
        );
        ctx->registerCallable(
            "SadPipelineAfterEventEachMS", 
            sad::dukpp03::make_function::from(__after_event_each_ms)
        );
        ctx->registerCallable(
            "SadPipelineAfterEventOneShot", 
            sad::dukpp03::make_function::from(__after_event_one_shot)
        );
        ctx->registerCallable(
            "SadPipelineAfterEventDelayed", 
            sad::dukpp03::make_function::from(__after_event_delayed)
        );
    }

    exposeWay(ctx);
}


static void exposeWay(sad::dukpp03::Context* ctx)
{
    {
        sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();

        c->addObjectConstructor<sad::Way>("SadWay");

        c->addMethod("setObjectName", sad::dukpp03::bind_method::from(&sad::Way::setObjectName));
        c->addMethod("objectName", sad::dukpp03::bind_method::from(&sad::Way::objectName));

        c->addAccessor("MajorId", sad::dukpp03::getter::from(&sad::Way::MajorId), sad::dukpp03::setter::from(&sad::Way::MajorId));
        c->addAccessor("MinorId", sad::dukpp03::getter::from(&sad::Way::MinorId), sad::dukpp03::setter::from(&sad::Way::MinorId));

        c->addMethod("getPointInTime", sad::dukpp03::bind_method::from(&sad::Way::getPointInTime));
        c->addMethod("setPoint", sad::dukpp03::bind_method::from(&sad::Way::setPoint));
        c->addMethod("addPoint", sad::dukpp03::bind_method::from(&sad::Way::addPoint));
        c->addMethod("insertPoint", sad::dukpp03::bind_method::from(&sad::Way::insertPoint));
        c->addMethod("removePoint", sad::dukpp03::bind_method::from(&sad::Way::removePoint));
        c->addMethod("closed", sad::dukpp03::bind_method::from(&sad::Way::closed));
        c->addMethod("setClosed", sad::dukpp03::bind_method::from(&sad::Way::setClosed));
        c->addMethod("makeClosed", sad::dukpp03::bind_method::from(&sad::Way::makeClosed));
        c->addMethod("makeOpen", sad::dukpp03::bind_method::from(&sad::Way::makeOpen));
        c->addMethod("setTotalTime", sad::dukpp03::bind_method::from(&sad::Way::setTotalTime));
        c->addMethod("totalTime", sad::dukpp03::bind_method::from(&sad::Way::totalTime));
        c->addMethod("wayPoints", sad::dukpp03::bind_method::from(&sad::Way::wayPoints));
        c->addMethod("construct", sad::dukpp03::bind_method::from(&sad::Way::construct));
        c->addMethod("startConstruction", sad::dukpp03::bind_method::from(&sad::Way::startConstruction));
        c->addMethod("serializableName", sad::dukpp03::bind_method::from(&sad::Way::serializableName));

        c->setPrototypeFunction("SadWay");

        ctx->addClassBinding("sad::Way", c);
    }
}
