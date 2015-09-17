#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include <window.h>
#include <glcontext.h>
#include <renderer.h>
#include <sadsleep.h>
#include <timer.h>
#include <log/consoletarget.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


#ifdef USE_WINDOW_TEST

/*!
 * Tests object logic
 */
struct sadWindowTest : tpunit::TestFixture
{
 public:
   sadWindowTest() : tpunit::TestFixture(
       TEST(sadWindowTest::testCreateDestroyFixed),
       TEST(sadWindowTest::testFullscreenClose),
       TEST(sadWindowTest::testTitle),
       TEST(sadWindowTest::testGLContext)
   ) {}
    
   void putWindowRect(sad::Window & w, sad::Renderer & r)
    {
    sad::Rect2I rect = w.rect();
    sad::String message = str(
           fmt::Format("Rect is {0} {1} {2} {3}")
           << rect[0].x() 
           << rect[0].y()
           << rect.width()
           << rect.height()
    );
    SL_LOCAL_MESSAGE(message, (r));
    }
   /*! Test manually, no message should be created
    */
   void testCreateDestroyFixed()
   {
       sad::Renderer r;
       r.log()->addTarget(new sad::log::ConsoleTarget());

       sad::Window win;
       win.setCreationSize(sad::Size2I(800, 600));
       win.setRenderer(&r);
       win.create();
       win.show();
       win.makeFixedSize();
       sad::sleep(2000);
       win.makeResizeable();
       win.setRect(sad::Rect2I(sad::Point2I(0, 0), sad::Point2I(640, 480)));
       putWindowRect(win, r);
       sad::sleep(2000);	   
       win.hide();
       win.destroy();
   }

   /*! Test manually entering fullscreen
    */
   void testFullscreenClose() 
   {
       sad::Renderer r;
       r.log()->addTarget(new sad::log::ConsoleTarget());

       sad::Window win;
       win.create();
       win.show();
       sad::sleep(1000);
       // Window size is set asynchronously, so it could print any value
       putWindowRect(win, r);
       win.enterFullscreen();
       putWindowRect(win, r);
       sad::sleep(2000);
       putWindowRect(win, r);
       win.leaveFullscreen();
       putWindowRect(win, r);
       sad::sleep(1000);
       win.close();
       win.destroy();
   }

   /*! Sets title for item
    */
   void testTitle()
   {
       sad::Renderer r;
       r.log()->addTarget(new sad::log::ConsoleTarget());

       sad::Window win;
       win.create();
       win.show();
       sad::sleep(1000);
       win.setTitle("");
       sad::String a = "";
       for(int i = 0; i < 20; i++)
       {
           a  += "A";	
           win.setTitle(a);
           sad::sleep(150);
       }
       win.close();
       win.destroy();
   }

   /*! Tests creating an OpenGL context
    */
   void testGLContext()
   {
       sad::Renderer r;
       r.log()->addTarget(new sad::log::ConsoleTarget());

       sad::Window win;
       win.setRenderer(&r);
       sad::GLContext ctx;
       ASSERT_TRUE( win.create() );
       ASSERT_TRUE( ctx.createFor(&win) );

       ctx.destroy();
       win.destroy();
   }

} sad_window_test;

#endif
