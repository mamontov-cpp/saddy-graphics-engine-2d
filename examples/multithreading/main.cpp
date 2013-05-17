#include "renderer.h"
#include "input.h"
#include "fontmanager.h"
#include "orthocamera.h"
#include "png/picopngloader.h"
#include "os/thread.h"
#include "input.h"
#include "sprite2dadapter.h"
#include "label.h"
#include "ftfont.h"
#ifdef WIN32
#ifndef MINGW
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")
#endif
#endif


void rend_quit(const sad::Event & o)
{
	sad::Renderer::ref()->quit();
}


class EventHandler: public sad::EventHandler
{
 private:
	 sad::Renderer * m_renderer;
	 Sprite2DAdapter * m_ad;
	 bool m_quit;
 public:
	 EventHandler(sad::Renderer * r, Sprite2DAdapter * a, bool quit)
	 {
		 m_renderer = r;
		 m_ad = a;
		 m_quit = quit;
	 }

	 virtual void operator()(const sad::Event & o)
	 {
		if (m_quit)
		{
			m_renderer->quit();
		}
		else 
		{
		}
	 }
};	


void * thread(void * p)
{
	sad::Renderer r;
	sad::log::FileTarget * fl = new sad::log::FileTarget();
	bool b = fl->open((const char *)p);
	r.log()->addTarget( fl);
	r.init(sad::Settings(800,600, false));
	r.getCurrentScene()->setCamera(new OrthoCamera(false,&r));
	
	FTFont * fnt1=new FTFont();
	bool res1= fnt1->load("ifaceed/EMPORIUM.ttf", 22);
	if (res1 == false) {
		SL_LOCAL_FATAL("Failed to load font...", r);
		return NULL;
  	}
	fnt1->setColor(hst::acolor(255,0,0,0));
	r.fonts()->add(fnt1, "font");
    
	r.getCurrentScene()->add(
		new Label(fnt1, "Awesome", pointf(300,200), &r)
	);
	

	r.controls()->bindKeyDown(KEY_ESC,  new EventHandler(&r, NULL, true));
	r.run();
	return NULL;
}


#include<math.h>
#include<time.h>

#ifdef WIN32
#ifndef MSVC_RELEASE
int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,  int nCmdShow)
#else
int main(int argc, char** argv)
#endif
#else
int main(int argc, char** argv)
#endif
{
	os::thread a(thread,"thread1.txt", true);
	os::thread b(thread,"thread2.txt", true);
	a.run();
	b.run();
	a.wait();
	b.wait();
	return 0;
}

