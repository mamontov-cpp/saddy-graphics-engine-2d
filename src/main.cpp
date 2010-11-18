//#include "stdafx.h"
#include "..\include\renderer.h"
#include "input.h"
#include "testnode.h"
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")

void rend_quit(const sad::Event & o)
{
	sad::Renderer::instance().quit();
}
void rend_toggle(const sad::Event & o)
{
	sad::Renderer::instance().toggleFullscreen();
}

int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,  int nCmdShow)
{

	sad::Scene sc;
	sc.add( new TestNode(0.0f,0.0f,0.2f) );
	sad::Renderer::instance().setCurrentScene(&sc);
	sad::Renderer::instance().setWindowTitle("My new sad window");
	
	sad::Input::inst()->bindKeyDown(KEY_ESC,rend_quit);
	sad::Input::inst()->bindKeyDown('F',rend_toggle);	
	//Here must be an initialization of engine, and running it

							 
	sad::Renderer::instance().init(sad::Settings(640,480,false));
	sad::Renderer::instance().run();

	return 0;
}

