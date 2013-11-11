#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <stdio.h>
#include <window.h>
#include <renderer.h>
#include <sadsleep.h>
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
	   win.setCreationSize(sad::Size2I(640, 480));
	   win.setRenderer(&r);
	   win.create();
	   sad::sleep(50);
	   win.destroy();
   }

} sad_window_test;