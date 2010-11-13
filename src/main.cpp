#include "stdafx.h"

#include <windows.h>
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")
#include "log.h"
#include "templates/hhash.hpp"
#include "texture.h"


int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,  int nCmdShow)
{
	sad::Texture test;
	test.loadPNG(hst::string("test.PNG"));
	return 0;
}

