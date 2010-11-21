//#include "stdafx.h"
#include "..\include\renderer.h"
#include "input.h"
#define FPS_CHECK
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

#include<math.h>

int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,  int nCmdShow)
{

	srand(NULL);

	sad::Scene * sc= new sad::Scene();


	for (int i=0;i<750;i++)
	sc->add( new TestNode((float)rand()/RAND_MAX*2.0f - 1.0f,(float)rand()/RAND_MAX*2.0f - 1.0f,0.02f) );


	sad::Renderer::instance().setCurrentScene(sc);
	sad::Renderer::instance().setWindowTitle("My new sad window");
	
	sad::Input::inst()->bindKeyDown(KEY_ESC,rend_quit);
	sad::Input::inst()->bindKeyDown('F',rend_toggle);	
	//Here must be an initialization of engine, and running it

							 
	sad::Renderer::instance().init(sad::Settings(640,480,false));
	sad::Renderer::instance().run();

	return 0;
}

