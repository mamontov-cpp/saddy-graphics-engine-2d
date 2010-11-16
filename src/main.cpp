//#include "stdafx.h"
#include "..\include\renderer.h"
#include "testnode.h"
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")


int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,  int nCmdShow)
{

	sad::Scene sc;
	sc.add( new TestNode(0.5f,0.5f,0.2f) );
	sad::Renderer::instance().setCurrentScene(&sc);

	//Here must be an initialization of engine, and running it

							 
	sad::Renderer::instance().init(sad::Settings(640,480,false));
	sad::Renderer::instance().run();

	return 0;
}

