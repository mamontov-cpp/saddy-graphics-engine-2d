/*! \mainpage Saddy
	
	A simple graphics engine for games. If you see any errors in documentation, or can
	give idea for improving a documentation, we would appreciate your help
 */
/*! \file   renderer.h
    \author FreakyBlast, HiddenSeeker

    \brief  Declaration of Renderer - main class, for running 
	application is placed here.
*/
#pragma once
#include "settings.h"
#include "scene.h"
#include "sadpoint.h"
#include "log/log.h"
#include "timer.h"
#include "maybe.h"

// This is a  problem - we included a header just because we need a system ops
// When created sad::SystemEventDispatcher - give him an opportunity to work with
// it
#ifdef WIN32
#define NOMINMAX
#include <windows.h>
#endif

namespace sad
{
class Input;
class FontManager;
class TextureManager;
class Window;
class GLContext;
class MouseCursor;
class OpenGL;
/*! Can be a point or none, depending on context
 */
typedef sad::Maybe<sad::Point3D> MaybePoint3D;
/*! \class Renderer
    Class, that provides all rendering and window operations.
    It wraps a low-level system calls to provide a simple interface
    for window operations.		
 */
class Renderer
{
public:
    /*! Creates default renderer. Note, that you must call 
		sad::Renderer::init() to start working
	 */
	Renderer();
	/*! Frees system resources from a renderer, when destroyed 
	 */
	virtual ~Renderer();
    /*! Sets a current scene. 
		Use this function to set a scene. Note that the pointer can not be null.
		If you want to clear scene, use sad::Scene::clear instead
		\param[in] scene  a new scene
	  */
    virtual void setScene(Scene * scene);
	/*! Returns a current rendererd scene
		\return current scene
	 */
	virtual sad::Scene* scene() const;
	/*! Initializes renderer with specified settings
		 \param[in] _settings settings 
		 \return Success of operation
	 */
	virtual void init(const sad::Settings& _settings);
	/*! Runs a renderer. Note that execution flow will not exit this method, until
		window is closed. 
		If renderer is already running, does nothing
		\return whether successfully runned
	 */
	virtual bool run();
	/*! Closes a window, shutting down a main loop, allowing execution flow 
		to exit sad::Renderer::run()
	 */
	virtual void quit();
	/*! Returns global instance of renderer
	 	\return global instance of renderer
	 */
	static sad::Renderer* ref();
	/*! Returns current fps of renderer
	 */
	virtual double fps() const;
	/*! Sets a window title
		\param[in] s  new title
	 */
	virtual void setWindowTitle(const sad::String & s);
	/*! Makes window non-resizeable
	 */
	virtual void makeFixedSize();
	/*! Makes window resizeable
	 */
	virtual void makeResizeable();
	/*! Enables/disables fullscreen
	 */
	virtual void toggleFullscreen();	
    /*! Returns, whether rendered has entered a main loop
	 */
	virtual bool running();
	/*! Determines, whether renderer's window and context are valid
	 */
	virtual bool hasValidContext();
	/*! Returns a mouse cursor position, if can.
		\return mouse cursor position in OpenGL viewport coordinates.
	 */
	virtual sad::MaybePoint3D cursorPosition() const;
	/*! Sets a mouse cursor position, where p is point in client window 
		area coordinates
		\param[in] p point
	 */
	virtual void setCursorPosition(const sad::Point2D & p);
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
		\return log 
	 */
	sad::log::Log * log();
	/*! Returns a window of a renderer
		\return window
	 */
	sad::Window * window();
	/*! Returns an OpenGL context for a renderer
		\return OpenGL context
	 */
	sad::GLContext * context();
	/*! Returns settings for a renderer
		\return settings
	 */
	virtual const sad::Settings & settings() const;
	/*! Returns current cursor implementation
		\return cursor implementation
	 */
	virtual sad::MouseCursor* cursor() const;
	/*! Sets a cursor implementations
		\param[in] cursor cursor implementation
	 */
	virtual void setCursor(sad::MouseCursor * cursor);
	/*! Returns an OpenGL platform, where you could query all data
		about current platform spec
	 */
	virtual sad::OpenGL * opengl() const;
#ifdef WIN32
	/*! Function for processing system messages and pressed keys
		\param[in] hWnd Windows' handler
		\param[in] uMsg The Message
		\param[in] wParam Additional message information
		\param[in] lParam Additional message information
		\return Success of operation              
	 */
    LRESULT dispatchMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
protected:
	/*! Copying a renderer, due to held system resources is disabled
		\param[in] o other renderer
     */
	Renderer(const Renderer& o);
	/*! Copying a renderer, due to held system resources is disabled
		\param[in] o other renderer
		\return self-rederence
	 */
	Renderer& operator=(const Renderer& o);
	/*! A global instance for renderer, to make it local creation is
		procedures unnecessary. It's not a singleton, but can
		be used as global variable
	 */
	static sad::Renderer * m_instance; 
	/*! A window for working with GUI operations
	 */
	sad::Window * m_window;
	/*! An OpenGL Context for working with OpenGL
	 */
	sad::GLContext * m_context;
	/*! A local log, where all messages will be stored
	 */
	sad::log::Log  m_log;
	/*! A proxy, who manages all input events
	 */
	sad::Input       *    m_input_manager;    
	/*! A font collection, attached to a renderer
	 */
	sad::FontManager *    m_font_manager;     
	/*! A texture collection, attached to a renderer
	 */
	sad::TextureManager * m_texture_manager; 
	/*! Returns mouse cursor, associated with renderer
	 */
	sad::MouseCursor*       m_cursor;
	/*! An OpenGL platform implementation part
	 */
	sad::OpenGL*            m_opengl;

	/*! A timer, for computng FPS
	 */
	sad::Timer		     m_timer;            
	/*! A current scene. May be old, when user called sad::Renderer::setScene
		and sad::Scene is being rendered
	 */
	sad::Scene*               m_scene;	 
	/*! A new scene. For most time is NULL, but if user set new scene via
		sad::Renderer::setScene it could be not NULL
	 */
	sad::Scene*               m_new_scene;   

		
	/*! A settings for a renderer
	 */
	sad::Settings             m_glsettings;  
	/*! Determines whether sad::Renderer is entered main loop
	 */
	bool                 m_running;       

	// A FPS computing options
	/*! An elapsed frames count
	 */
	int                  m_frames;      
	/*! Whether timer should be reset on next iteration
	 */
	bool                 m_reset;          
	/*! Whether we should immediately set FPS 
	 */
	bool				 m_setimmediately; 
	/*!  Current FPS value
	 */
	double               m_fps;            

	/*! Destroys global instance of renderer
	 */
	static void destroyInstance();
	/*! Sets new fps value, but avoids fast jumps in fps
		\param[in] fps  fps value
	*/
	void setFPS(double fps);
	/*! Setups a OpenGL for first use
		\return success of operation
	 */
	bool initGLRendering();
	/*! Reshapes view
		\param[in] width Needed width
		\param[in] height Needed height
	 */
	void reshape(int width, int height);
    /*! Main loop of renderer. Here it performs rendering cycle
	 */
	void mainLoop();
    /*! Updates a scene
	 */
	void update();
	/*! Tries to swap both scened in tenderer, if need
		so.
	 */
	virtual void trySwapScenes();
};

}
