#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include <window.h>
#include <renderer.h>
#include <sadsleep.h>
#include <timer.h>
#include <log/consoletarget.h>
#include "3rdparty/tpunit++/tpunit++.hpp"
#pragma warning(pop)


#ifdef WIN32
long __stdcall sad_renderer_window_proc(
	struct HWND__ * hWnd,
	unsigned int uMsg,
	unsigned int wParam,
	long lParam
)
{
	return DefWindowProcA(hWnd,uMsg,wParam,lParam);
}
#endif

/*!
 * Tests object logic
 */
struct sadWindowTest : tpunit::TestFixture
{
 public:
   sadWindowTest() : tpunit::TestFixture(
	   TEST(sadWindowTest::testOpenClose)
   ) {}
	
   /*! Test manually, no message should be created
    */
   void testOpenClose()
   {
	   sad::Renderer r;
	   r.log()->addTarget(new sad::log::ConsoleTarget());

	   sad::Window win;
	   win.setCreationSize(sad::Size2I(800, 600));
	   win.setRenderer(&r);
	   win.create();
	   win.show();
	   sad::sleep(2000);
	   win.setRect(sad::Rect2I(sad::Point2I(0, 0), sad::Point2I(640, 480)));
	   sad::Rect2I rect = win.rect();
	   sad::String message = str(
		   fmt::Format("Rect is {0} {1} {2} {3}")
		   << rect[0].x() 
		   << rect[0].y()
		   << rect.width()
		   << rect.height()
		   );
	   SL_LOCAL_MESSAGE(message, (r));
	   sad::sleep(2000);
	   win.hide();
	   win.destroy();
   }

} sad_window_test;