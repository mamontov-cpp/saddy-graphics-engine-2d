#include "dukpp-03/context.h"
#include "dukpp-03/renderer.h"
#include "dukpp-03/thread.h"
#include "dukpp-03/mutex.h"
#include "dukpp-03/semaphore.h"

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


template<typename T> static void __add_target_to_log(sad::log::Log* lg, T* a)
{
    lg->addTarget(a);
}

template<typename T> static void __remove_target_from_log(sad::log::Log* lg, T* a)
{
    lg->removeTarget(a);
}


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
}

