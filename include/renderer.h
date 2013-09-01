/*! \mainpage Saddy
	
	A simple graphics engine for games. If you see any errors in documentation, or can
	give idea for improving a documentation, we would appreciate your help
 */
/*! \file   renderer.h
    \author FreakyBlast, HiddenSeeker

    \brief  Declaration of Renderer is placed here.
*/
#pragma once
#include "settings.h"
#include "scene.h"
#include "primitives/hpoint.h"
#include "log/log.h"
#include "os/timer.h"
namespace sad
{
	class Input;
	class FontManager;
	class TextureManager;
	/*! \class Renderer
        Class, that provides all rendering and window operations.
        It wraps a low-level system calls to provide a simple interface
        for graphic operations.		
	*/
	class Renderer
	{
		Renderer(const Renderer& other);
		Renderer& operator=(const Renderer&);

		static sad::Renderer * m_instance;  //!< Instance of renderer

		sad::Log             m_log;

		sad::Input       *    m_input_manager;    //!< Input manager data
		sad::FontManager *    m_font_manager;     //!< Font manager
		sad::TextureManager * m_texture_manager; //!< Texture manager
		os::timer		     m_timer;            //!< Timer
		Scene*               m_currentscene;//!< Current scene
		Scene*               m_chscene;     //!< Scene to be changed
		sad::Window          m_window;      //!< Window
		double               m_fps;         //!< Current FPS value
		Settings             m_glsettings;  //!< Settings
		hst::string          m_windowtitle; //!< Title of window
        bool                 m_running;     //!< Sets, whether we are running now
        bool                 m_created;     //!< Whether we are created a window
		/*! Destroys instance of renderer
		 */
		static void destroyInstance();
		/*! Inits some window parameters
		 */
		void initWindowParameters();
		/*! Sets new fps value, but avoids fast jumps in fps
			\param[in] fps  fps value
		*/
		void setFPS(double fps);
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
        LRESULT dispatchMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		/*! Default message
		    \param[in] hWnd Windows' handler
		    \param[in] uMsg The Message
		    \param[in] wParam Additional message information
		    \param[in] lParam Additional message information
		    \return Success of operation              
		*/
        static LRESULT CALLBACK WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		/*! Converts parameters to client coordinates
			\param[in] lParam parameters
			\return point structure
		 */
		POINT _toClient(POINT  pnt);
		/*! Converts parameters to client coordinates
			\param[in] lParam parameters
			\return point structure
		 */
		POINT _toClient(LPARAM lParam);
#endif
#ifdef LINUX
        /*! Sends fullscreen atom to toggle fullscreen
             \param[in] flag 1 to make it fullscreen,0 to make windowed
        */
	void sendFSAtom(int flag);
        /*! DEPRECATED: Don't use
	*/
	bool XContextInit();
#endif
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
        Renderer(void);
		/*! Sets a current scene.
		    DEPRECATED: Can only be used, when renderer is not started.
			            Use sad::Renderer::pushScene() instead.
		    \param[in] scene to be set as current
		*/
		void setCurrentScene(sad::Scene* scene);
        /*! Sets a current scene. 
		    Use this function to set a scene, while running
		    \param[in] scene  a new scene
		*/
        void pushScene(Scene * scene);
        /*! Destructor */
		~Renderer(void);
		/*! Gets a current window
		    \sa sad::Window
		    \return Current window
		*/
		inline sad::Window * getCurrentWindow()  { return &m_window;}
		/*! Initializes renderer with specified settings
		    \param[in] _settings settings 
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
		static Renderer* ref();
		/*! Returns a current scene, stored in m_currentscene
		    \return Current scene
		*/
		inline Scene* getCurrentScene()const      {return m_currentscene;}
		inline double fps()const                  {return m_fps;         }
		/*! Sets a window title
		    \param[in] s  new title
		*/
		void setWindowTitle(const hst::string & s);
		 /*! Enables fixed size window
	     */
	    void toggleFixedOn();
	    /*! Disabled fixed size window
	    */
	    void toggleFixedOff();
		/*! Enable/disable fullscreen
		*/
		void toggleFullscreen();	
        /*! Returns, whether we are running
		*/
		inline bool running() { return m_running; }
		/*! Returns current settings
			\return settings
		*/
		const sad::Settings & settings() { return m_glsettings; }
		/*! Returns a mouse position
			\return mouse position in window coordinate.
		*/
		hst::point<hst::D3,float> mousePos();
		/*! Font manager data
			\return fonts, loaded into renderer
		 */
		sad::FontManager * fonts();
		/*! Texture manager information
			\return texture manager information
		 */
		sad::TextureManager * textures();
		/*! Constrols of sad Input
		 */
		sad::Input  * controls(); 
		/*! Returns a log for renderer
			\return log information
		 */
		sad::Log * log();
	};

	/*! Returns average render interval in milliseconds
		\return average render interval in milliseconds
	*/
	inline float avgRenderInterval() {
		return 1000.0f/(float)sad::Renderer::ref()->fps();
	}
}
