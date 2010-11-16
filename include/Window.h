/*! \file   window.h
\author FreakyBlast

*/

#ifndef GL_FRAMEWORK__INCLUDED
#define GL_FRAMEWORK__INCLUDED


#ifndef Window_h__
#define Window_h__

#include <windows.h>
#include "types.h"
#include "settings.h"

/*! \class SadWindow
This class represents working window of Saddy
*/
namespace Sad{
	class Window										
	{
	public:
		Window(void);
		~Window(void);

		/*! Terminating program
		\param[in] window Window to close
		*/
		void terminateApplication (sadWindow* window);	
		/*! Reshapes GL-Window to needed sizes
		\param[in] width Needed width
		\param[in] height Needed height
		*/
		void reshapeGL(int width, int height, Sad::Settings* _sets);

		/*! Update scene
		\param[in] Time elapsed since previous call of Update(long long)
		Performs update of scene according with time in milliseconds,
		elapsed since last call of this function.
		*/
		void update(long long DeltaTime);			

		/*! Initialization of GLWindow setting
		\param[out] _GLSettings Start setting for OpenGL-window
		TODO: we shall be write this function, also worth considering from where
		this settings will be taken.
		*/
		void initSettings(Sad::Settings* _GLSettings);				





		/*! Create GLContext upon window
		\param[in] _settings Settings of OpenGL 
		\param[out] wndParams Window parameters
		\return Success of operation
		*/
		bool createGLContext(Settings* _settings, sadWindow* wndparams);

		/*! Destroy GLContext from window
		\param[in] window Window to destroy.
		\return Success of operation
		*/
		bool destroyWindowGL(sadWindow* window, Settings* _GLSets);	

		/*! Changes screen resolution
		\param[in] width Aiming width
		\param[in] height Aiming height
		\param[in] bitsPerPixel Bits per per pixel
		\return Success of operation
		*/
		bool changeScreenResolution (int width, int height, int bitsPerPixel);
	};

}
#endif												// GL_FRAMEWORK__INCLUDED

#endif // Window_h__