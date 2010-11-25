/*! \file   renderer.h
\author FreakyBlast

\brief  Declaration of Renderer.

*/

#ifndef Render_h__
#define Render_h__

#include "settings.h"
#include "scene.h"


namespace sad
{
	/*!\class The main render class 
	*/
	class Renderer
	{
		Renderer(void);
		Renderer(const Renderer& other);
		Renderer& operator=(const Renderer&);


		unsigned long        m_starttimer;
		Scene*               m_currentscene;
		Scene*               m_chscene;
		sad::Window          m_window;
		int                  m_fps;
		Settings             m_glsettings;
		hst::string          m_windowtitle; //!< Title of window
        bool                 m_running;     //!< Sets, whether we are running now
        bool                 m_created;     //!< Whether we are created a window
		/*! Setups a OpenGL for first use
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


		/*! Create GLContext upon window
		\return Success of operation
		*/
		bool createGLContext();


       /*! Updates a scene
	   */
		void update();



		/*! Sets a current window
		\param[in] Structure of window
		*/
		inline void setCurrentWindow(sad::Window * window) { m_window=*window;}


	public:

		/*! Sets a current scene
		\param[in] Scene to be set as current
		*/
		inline void setCurrentScene(Scene* scene) 
		{
			if (m_currentscene) delete m_currentscene;
			m_currentscene=scene; 
		}
        /*! Sets a current scene. Use this function to set a scene, while running
		\param[in] Scene to be set as current
		*/
        inline void pushScene(Scene * scene)
		{
			m_chscene=scene;
		}

		~Renderer(void);

		/*! Gets a current window in the form sadWindow
		\return Current window
		*/
		inline sad::Window * getCurrentWindow()  { return &m_window;}


		/*! Fills a structure SadWindow
		\param[in] _setting Settings of GLContext
		\param[out] window Structure to filling
		\return Success of operation
		*/
		bool init(const sad::Settings& _settings);


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

		/*! Sets a window title
		    \param[in] s  new title
		*/
		void setWindowTitle(const hst::string & s);
		/*! Enable/disable fullscreen
		*/
		void toggleFullscreen();	


		inline bool running() { return m_running; }
	};
	
	
}




#endif // Render_h__
