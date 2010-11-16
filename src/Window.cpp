
#include "..\include\window.h"													
#include "..\include\renderer.h"													


#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )

namespace Sad{
	Sad::Window::Window(void)
	{
	}

	Sad::Window::~Window(void)
	{
	}

#define WM_TOGGLEFULLSCREEN (WM_USER+1)									// Application Define Message For Toggling




	void Sad::Window::reshapeGL (int width, int height, Sad::Settings* _sets)									// Reshape The Window When It's Moved Or Resized
	{
		Renderer::instance().reshape(width, height, _sets);
	}



	bool Sad::Window::createWindow(sadWindow* wndparams){
		
	}

	bool Sad::Window::createGLContext(Sad::Settings* _settings,sadWindow* wndparams)									// This Code Creates Our OpenGL Window
	{

		// Initialization Will Be Done In WM_CREATE
	}


	bool Sad::Window::destroyWindowGL (Sad::sadWindow* window, Sad::Settings* settings)								// Destroy The OpenGL Window & Release Resources
	{

	}

	void Sad::Window::update(long long deltaTime){

	}

	void Sad::Window::initSettings(Sad::Settings* _GLSettings){

	}

}

