#include "renderer.h"
#include "input.h"
#include "fontmanager.h"
#include "orthocamera.h"
#include "png/picopngloader.h"
#include "os/thread.h"

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


void * thread(void * p)
{
	sad::Renderer r;
	sad::log::FileTarget * fl = new sad::log::FileTarget();
	bool b = fl->open((const char *)p);
	r.log()->addTarget( fl);
	r.init(sad::Settings(800,600, false));
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

