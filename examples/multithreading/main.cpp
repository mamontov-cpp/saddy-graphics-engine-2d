/*! \mainpage Multithreading Example
    A simple example of two OpenGL windows running simultaneously
 */
/*! \file main.cpp
    
    A multithreading example is an example of two OpenGL windows in one process
    running simultaneously.
 */
#include <renderer.h>
#include <input/controls.h>
#include <keymouseconditions.h>
#include <input/handlers.h>
#include <font.h>
#include <orthographiccamera.h>
#include <sadthread.h>
#include <sprite2d.h>
#include <label.h>
#include "game\inventory.h"
#include "game\item.h"
#include <texturemappedfont.h>
#include <freetype/font.h>
#include <animations/animationstyping.h>
#include <animations/animationstexturecoordinatescontinuous.h>
#include <animations/animationscolor.h>
#include <animations/animationsparallel.h>
#include <animations/animationsinstance.h>

#include "3rdparty/tpunit++/tpunit++.hpp"


#ifdef WIN32
#include <windows.h>
#endif

/*! \class EventHandler
    A simple handler which, depending on settings cand quit renderer's main loop
    or just move sprite adapter to a new point, where user clicked. It's used 
    mainly to implement two behaviours
    
    1) When user presses Escape key, then window closes and the game ends

    2) When users clicks on some space on screen, the center of sprite (m_ad field)
       moves here.

    Current behaviour depends on m_quit variable, which is if set to true quits it
 */
class EventHandler: public sad::input::AbstractHandler
{
 private:
     sad::Renderer * m_renderer; //!< A current renderer, which working controls belong to
     sad::Sprite2D * m_ad;       //!< A sprite, which should be moved when user clicks. Could be NULL if m_quit is true
     bool m_quit;                //!< Whether we should quit renderer on event call
     bool m_messagebox;          //!< Whether we should show message box on event call
 public:
     /*! A new handler just consists from these three fields
         \param[in] r renderer
         \param[in] a sprite
         \param[in] quit whether we should quit on call
         \param[in] messagebox  whether we should show message box on call
      */
     EventHandler(sad::Renderer * r, sad::Sprite2D * a, bool quit, bool messagebox)
     {
         m_renderer = r;
         m_ad = a;
         m_quit = quit;
         m_messagebox = messagebox;
     }
     /*! This method is called, when  event, specified when passing handler to 
         sad::Input is occured in window. Currently it implements behaviour specified
         in header of class
      */
     virtual void invoke(const sad::input::AbstractEvent & e)
     {
        if (m_quit)
        {
            m_renderer->quit();
        }
        else 
        {
            if (m_messagebox)
            {
                bool result = m_renderer->information("Right mouse button triggered!", "You\'ve pressed right mouse button");
                if (result)
                {
                    printf("Message box successfully triggered\n");
                }
                else
                {
                    printf("Unable to show message box\n");
                }
            }
            else
            {
                const sad::input::MouseMoveEvent& ev = static_cast<const sad::input::MouseMoveEvent&>(e);
                // Move sprite center to a position
                m_ad->moveTo(ev.pos2D());
            }
        }
     }
     /*! This is convenient function for  implementation of sad::Input, which avoids calling
         handlers with no valid pointer to functions inside of these. So we must tell it, that
         our handler is safe and it can call it.
         \return bool false
      */
     virtual bool empty() 
     {
        return false;
     }
};	

/*! This is simple thread function, which inits a renderer, with simple scene of
    two kind of fonts and sprite. Also it creates separate log for work, and sets 
    it's separate callbacks to exit on Escape and move sprite on user click.

    This demo does not use own assets, instead it loads in from "game" example and
    IFaceEditor plugin.

    \param[in] p log name as pointer to characters.
    \return 0 in most cases
 */
int thread(void * p)
{
    /* Firstly, we create our own renderer, which will do all kind of tasks
     */ 
    sad::Renderer r;
    /*! Create and set scene for renderer;
     */
    sad::Scene * scene = new sad::Scene();
    r.setScene(scene);
    r.tree()->factory()->registerResource<sad::freetype::Font>();
    
    /* Setup the logging. We redirect all messages to a file, passed as parameter to thread
       variable
     */
    sad::log::FileTarget * fl = new sad::log::FileTarget();
    bool b = fl->open(static_cast<const char *>(p));
    if (!b)
    {
        SL_LOCAL_DEBUG("Failed to open local file", r);
    }
    r.log()->addTarget( fl);
    
    /* Create 800x600 window in windowed mode and toggle a fixed size of window
     */
    r.init(sad::Settings(800,600, false));
    r.makeFixedSize();
    
    

    /* Bind built-ing scene to renderer 
     */
    scene->setRenderer(&r);

    /*! Load resources
     */
    sad::Vector<sad::resource::Error *> errors = r.loadResources("examples/multithreading.json");
    if (errors.size() != 0)
    {

        SL_LOCAL_FATAL(sad::resource::format(errors), r);
        sad::util::free(errors);
        return 1;
    } 
    

    // Create a simple animations
    sad::animations::Typing* typing = new sad::animations::Typing();
    typing->setLooped(true);
    typing->setTime(2000);
    r.tree("")->root()->addResource("typing", typing);

    sad::animations::Color* color = new sad::animations::Color();
    color->setLooped(true);
    color->setTime(2000);
    color->setMinColor(sad::AColor(255, 255, 255, 0));
    color->setMaxColor(sad::AColor(255, 0, 0, 0));	
    r.tree("")->root()->addResource("color", color);

    sad::animations::Parallel* parallel = new sad::animations::Parallel();
    parallel->setLooped(true);
    parallel->add(typing);
    parallel->add(color);
    parallel->setTime(2000);
    r.tree("")->root()->addResource("parallel", parallel);
    
    sad::animations::TextureCoordinatesContinuous* tcc = new sad::animations::TextureCoordinatesContinuous();
    tcc->setLooped(true);
    tcc->setTime(5000);
    tcc->setStartRect(sad::Rect2D(0, 0, 512, 512));
    tcc->setStartRect(sad::Rect2D(128, 128, 512, 512));
    r.tree("")->root()->addResource("tcc", tcc);


    /* Create simple sprite. 512x512 is a size of texture and it's passed as second parameter
     */
    sad::Sprite2D * a = new sad::Sprite2D("tex1", sad::Rect2D(sad::Point2D(0,0), sad::Point2D(512,512)), sad::Rect2D(sad::Point2D(0,0), sad::Point2D(512,512)));
    scene->add(a);
    /* Add two labels with different fonts
     */
    sad::Label * l1 = new sad::Label("ftfont", sad::Point2D(300,200), "FTFont");
    sad::Label * l2 = new sad::Label("tmfont", sad::Point2D(400,400), 
        "TextureMappedFont is a nice option for those who don't want to work with\ncomplex stuff"
    );
    l1->setColor(255, 255, 255, 0);
    l2->setColor(255, 255, 255, 0);
    l2->setBreakText(sad::Label::LBT_BREAK_WORD);
    l2->setMaximalLineWidth(15);	
    scene->add(l1);
    scene->add(l2);

    sad::animations::Instance* parallelinstance = new sad::animations::Instance();
    parallelinstance->setAnimation(parallel);
    parallelinstance->setObject(l1);
    r.animations()->add(parallelinstance);

    sad::animations::Instance* tccinstance = new sad::animations::Instance();
    tccinstance->setAnimation(tcc);
    tccinstance->setObject(a);
    r.animations()->add(tccinstance);

    /* Here we bind two different handlers with keydown
     */
    sad::input::Controls * c = r.controls();
    c->add(*sad::input::ET_KeyPress & sad::Esc, new EventHandler(&r, NULL, true, false));
    c->add(*sad::input::ET_MousePress & sad::MouseLeft, new EventHandler(&r, a, false, false));
    c->add(*sad::input::ET_MousePress & sad::MouseRight, new EventHandler(&r, a, false, true));
    
    /* Start main rendering loop. Execution will not progress further, until user closes window
       or press Escape
     */
    r.run();

    // Return default value
    return 0;
}


// Runs built-in tests for Inventory and item
#define RUN_TESTS

/* This macro manupilation is to make console window hidden in MSVC window
 */
int main(int argc, char** argv)
{
#ifdef RUN_TESTS
	int result = tpunit::Tests::Run();
	return result;
#else
	sad::Renderer::ref()->information("Multithreading demo", "This is demo for multithreading");
	// Here we create two waitable threads
	sad::Thread a(thread, static_cast<void *>(const_cast<char*>("thread1.txt")));
	sad::Thread b(thread, static_cast<void *>(const_cast<char*>("thread2.txt")));

	// Run them
	a.run();
	b.run();

	// And wait
	a.wait();
	b.wait();
	return 0;
#endif // RUN_TESTS
}

