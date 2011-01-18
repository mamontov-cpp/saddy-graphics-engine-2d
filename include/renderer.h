/*! \file   renderer.h
    \author FreakyBlast, HiddenSeeker

    \brief  Declaration of Renderer is placed here.
*/
#pragma once
#include "settings.h"
#include "scene.h"


namespace sad
{
	/*! \class Renderer
        Class, that provides all rendering and window operations.
        It wraps a low-level system calls to provide a simple interface
        for graphic operations.		
	*/
	class Renderer
	{
		Renderer(void);
		Renderer(const Renderer& other);
		Renderer& operator=(const Renderer&);


		unsigned long        m_starttimer;  //!< Timer
		Scene*               m_currentscene;//!< Current scene
		Scene*               m_chscene;     //!< Scene to be changed
		sad::Window          m_window;      //!< Window
		int                  m_fps;         //!< Current FPS value
		Settings             m_glsettings;  //!< Settings
		hst::string          m_windowtitle; //!< Title of window
        bool                 m_running;     //!< Sets, whether we are running now
        bool                 m_created;     //!< Whether we are created a window
		/*! Setups a OpenGL for first use
		    \return success of operation
		*/
		bool initGLRendering();
		/*! Creates an OpenGL window
		*/
		bool createWindow();
        /*! Releases a rendering context of window
		*/
		void releaseWindow();
		/*! Adjusts video mode for work (fullscreen/windowed), returning styles
		*/
		void adjustVideoMode(unsigned long & style, unsigned long & ex_style);
		/*! Setups a pixel format descriptor
		*/
		bool setupPFD();

		/*! Maps window coordinates, to OGL coordinate system
		*/
        void mapToOGL(int x,int y,float & px,float & py,float & pz);
#ifdef WIN32
		/*! Function for processing system messages and pressed keys
		    \param[in] hWnd Windows' handler
		    \param[in] uMsg The Message
		    \param[in] wParam Additional message information
		    \param[in] lParam Additional message information
		    \return Success of operation              
		*/
        static LRESULT CALLBACK WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
#ifdef LINUX
        /*! Sends fullscreen atom to toggle fullscreen
             \param[in] flag 1 to make it fullscreen,0 to make windowed
        */
	void sendFSAtom(int flag);
#endif
		/*! Sets timer
		*/
		inline void setTimer(void){ m_starttimer = clock();}

		/*! Reshapes view
		    \param[in] width Needed width
		    \param[in] height Needed height
		*/
		void reshape(int width, int height);

       /*! Main loop of renderer. Here it performs rendering cycle
	   */
		void mainLoop();

		/*! Create graphic context upon window
		    \return Success of operation
		*/
		bool createGLContext();

       /*! Updates a scene
	   */
		void update();

		/*! Sets a current window
		    \sa sad::Window
		    \param[in] window structure
		*/
		inline void setCurrentWindow(sad::Window * window) { m_window=*window;}

	public:
		/*! Sets a current scene.
		    DEPRECATED: Can only be used, when renderer is not started.
			            Use ::pushScene() instead.
		    \param[in] scene to be set as current
		*/
		inline void setCurrentScene(Scene* scene) 
		{
			if (m_currentscene) delete m_currentscene;
			m_currentscene=scene; 
		}
        /*! Sets a current scene. 
		    Use this function to set a scene, while running
		    \param[in] Scene to be set as current
		*/
        inline void pushScene(Scene * scene)
		{
			m_chscene=scene;
		}
        /*! Destructor */
		~Renderer(void);
		/*! Gets a current window
		    \sa sad::Window
		    \return Current window
		*/
		inline sad::Window * getCurrentWindow()  { return &m_window;}
		/*! Initializes renderer with settings
		    \param[in] _setting settings 
		    \return Success of operation
		*/
		bool init(const sad::Settings& _settings);
		/*! Runs a renderer
		*/
		void run();
		/*! Quits a renderer
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
		/*! elapsed time in milliseconds
		    \return Time in milliseconds
		*/
		inline int elapsedInMSeconds(){return clock() - m_starttimer;}

		/*! Sets a window title
		    \param[in] s  new title
		*/
		void setWindowTitle(const hst::string & s);
		/*! Enable/disable fullscreen
		*/
		void toggleFullscreen();	
        /*! Returns, whether we are running
		*/
		inline bool running() { return m_running; }
	};
	
}
