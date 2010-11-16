//#include "stdafx.h"
#include "..\include\renderer.h"
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")


int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,  int nCmdShow)
{

	sad::Scene sc;
	sc.add( new TestNode(0.5f,0.5f,0.2f) );
	Sad::Renderer::instance().setCurrentScene(&sc);

	//Here must be an initialization of engine, and running it

							 
	Sad::Renderer::instance().init(Sad::Settings(640,480,false));
	Sad::Renderer::instance().run();

	return 0;
}

