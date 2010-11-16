#include "stdafx.h"

#include <windows.h>
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")
#include "log.h"
#include "testnode.h"
#include "texture.h"


int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,  int nCmdShow)
{
	//sad::Texture test;
	//test.loadPNG(hst::string("test.PNG"));
	sad::Scene sc;
	sc.add( new TestNode(0.5f,0.5f,0.2f),"u" );
	TestNode * node=sc.get<TestNode>("u");
	//Here must be an initialization of engine, and running it

	return 0;
}

