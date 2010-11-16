/*! \file   render.h
\author FreakyBlast

\brief  Declaration of Renderer.

*/

#ifndef Render_h__
#define Render_h__


#include "settings.h"
#include "scene.h"
#include "texture.h"
#include "testnode.h"


namespace Sad{
	/*!\class The main render class 
	*/
	class Renderer
	{
		Renderer(void);
		Renderer(const Renderer& other);
		Renderer& operator=(const Renderer&);


		unsigned long m_starttimer;
		Scene* m_currentscene;
		sadWindow m_currentwindow;
		int m_fps;
		Settings m_glsettings;

        /*! Registers class for WindowCreateEx (in window system)
		\param[in] application Structure with information about window:
		                       hInstance and className
		\return Success of operation
		*/
        bool RegisterWindowClass(Application& application);

        /*! Function for processing system messages and pressed keys
		\param[in] hWnd Windows' handler
		\param[in] uMsg The Message
		\param[in] wParam Additional message information
		\param[in] lParam Additional message information
		\return Success of operation              
		*/
        static LRESULT CALLBACK WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		/*! Sets timer
		*/
		inline void setTimer(void){ m_starttimer = clock();}

		/*! Reshapes view
		\param[in] width Needed width
		\param[in] height Needed height
		*/
		void reshape(int width, int height);

       /*! Main loop of renderer. Here performs rendering cycle
	   */
		void mainLoop();

		/*! Create window setting (specific for operating system)
		\return Success of operation
		*/
		bool createWindow();

		/*! Create GLContext upon window
		\return Success of operation
		*/
		bool createGLContext();

		/*! Enable/disable fullscreen
		*/
		void toggleFullscreen();	

		/*! Changes resolution of screen
		\return Success of operation
		*/
		bool changeScreenResolution (int width, int height, int bitsPerPixel);

       /*! Updates a scene
	   */
		void update();

       /*! Zeroize hWnd, hDC, hRC,
		   destroys contexts and window
		\return Success of operation
		*/
		bool destroyWindowGL();


		/*! Sets a current window
		\param[in] Structure of window
		*/
		inline void setCurrentWindow(sadWindow* window){m_currentwindow=*window;}


	public:

		/*! Sets a current scene
		\param[in] Scene to be set as current
		*/
		inline void setCurrentScene(Scene* scene) {m_currentscene=scene; }


		~Renderer(void);

		/*! Gets a current window in the form sadWindow
		\return Current window
		*/
		inline sadWindow* getCurrentWindow(void){return &m_currentwindow;}

		inline void setTitle(const hst::string& newTitle){m_currentwindow.title = newTitle;}

		/*! Fills a structure SadWindow
		\param[in] _setting Settings of GLContext
		\param[out] window Structure to filling
		\return Success of operation
		*/
		bool init(const Sad::Settings& _settings);


		/*! Runs renderer*/
		void run();


		/*! Quit
		*/
		void quit(void);


		/*! Returns instance - Renderer is a classic singleton
		\return Reference to the alone Renderer object
		*/
		static Renderer& instance();

		/*! Returns a current scene, stored in m_currentscene
		\return Current scene
		*/
		inline Scene* getCurrentScene()const      {return m_currentscene;}


		inline int fps()const                     {return m_fps;         }

		/*! Elapsed time in milliseconds
		\return Time in milliseconds
		*/
		inline int elapsedInMSeconds(){return clock() - m_starttimer;}

		/*! Elapsed time in seconds
		\return Time in seconds
		*/
		inline int elapsedInSeconds()
		{return (clock()/CLOCKS_PER_SEC) - (m_starttimer/CLOCKS_PER_SEC);}


	};
	
	extern bool isProgramLooping;			// Window Creation Loop																// Between Fullscreen / Windowed Mode
	
}




#endif // Render_h__