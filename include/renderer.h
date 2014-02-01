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
#include "temporarilyimmutablecontainer.h"
#include "sadptrvector.h"
#include "primitiverenderer.h"

namespace sad
{
class Input;
class FontManager;
class TextureManager;
class Window;
class GLContext;
class MainLoop;
class MouseCursor;
class OpenGL;
class FPSInterpolation;
namespace pipeline
{
	class Pipeline;
}
namespace input
{
	class Controls;
}
/*! Can be a point or none, depending on context
 */
typedef sad::Maybe<sad::Point3D> MaybePoint3D;
/*! \class Renderer
    Class, that provides all rendering and window operations.
    It wraps a low-level system calls to provide a simple interface
    for window operations. 

	It should be noted, that each renderer should run in it's separate thread,
	because it maintains a main loop and to prevent some problems with window
	event handling. Note, that one sad::Renderer can run in main thread, but only
	if there are no other windows attached to it.
 */
class Renderer: public TemporarilyImmutableContainer<sad::Scene>
{
public:
    /*! Creates default renderer. Note, that you must call 
		sad::Renderer::init() to start working
	 */
	Renderer();
	/*! Frees system resources from a renderer, when destroyed 
	 */
	virtual ~Renderer();
    /*! Sets a current scene, clearing scene container and adding a new scene. 
		Use this function to set a scene. Note that the pointer can not be null.
		If you want to clear scene, use sad::Scene::clear instead
		\param[in] scene  a new scene
	  */
    virtual void setScene(Scene * scene);
	/*! Returns a current rendererd scene
		\return current scene
	 */
	virtual const sad::Vector<sad::Scene*>& scenes() const;
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
	sad::FontManager* fonts();
	/*! Texture manager information
		\return texture manager information
	 */
	sad::TextureManager* textures();
	/*! Returns a log for renderer
		\return log 
	 */
	sad::log::Log* log();
	/*! Returns a window of a renderer
		\return window
	 */
	sad::Window* window();
	/*! Returns an OpenGL context for a renderer
		\return OpenGL context
	 */
	sad::GLContext* context();
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
	virtual sad::OpenGL* opengl() const;
	/*! Returns current main loop for renderer
		\return main loop for a renderer
	 */
	virtual sad::MainLoop* mainLoop() const;
	/*! Returns current FPS interpolation for renderer
		\return fps interpolation instance
	 */
	virtual sad::FPSInterpolation* fpsInterpolation() const;
	/*! Returns a pipeline for renderer
		\return pipeline
	 */
	virtual sad::pipeline::Pipeline* pipeline() const;
	/*! Returns a controls. You can use it to define your own actions
		\return controls
	 */
	virtual sad::input::Controls* controls() const;

	/*! This method is called, when somebody performs emergency shutdown.
		In current implementation, this method is called when  console window
		of application is closed on Windows OS. Note, that you SHOULD NOT call
		this method under any circumstances
	 */
	virtual void emergencyShutdown();
	/*! Maps a point coordinates to current viewport coordinates. If window and context
		are not created returns default values
		\param[in] p first point
		\return 3D point
	 */
	sad::Point3D mapToViewport(const sad::Point2D & p);
	/*! Reshape a system of coordinates to deal with width and height
		\param[in] width Needed width
		\param[in] height Needed height
	 */
	void reshape(int width, int height);
	/*! Adds new scene to scene container
		\param[in] scene a scene to be rendered
	 */
	void add(sad::Scene * scene);
	/*! Returns order in which layer will be rendered. Higher order means the scene will
		be rendered later
		\param[in] s scene
		\return order, -1 if not found
	 */
	int layer(sad::Scene * s);
	/*! Sets scene order to be rendered, removing from previous point
		\param[in] s a scene
		\param[in] layer a layer ordering to be set
	 */
	void setLayer(sad::Scene * s, unsigned int layer);
	/*! Sets primitive renderer
		\param[in] r renderer for primitives
	 */
	void setPrimitiveRenderer(sad::PrimitiveRenderer * r);
	/*! Returns primitive renderer, which can be used to render primitives
		\return renderer for primitives
	 */
	sad::PrimitiveRenderer * render() const;
	/*! Fetches path to executable. Returns empty string if fails
		\return executable path
	 */
	const sad::String & executablePath() const;
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
	static sad::Renderer* m_instance; 
	/*! A window for working with GUI operations
	 */
	sad::Window* m_window;
	/*! An OpenGL Context for working with OpenGL
	 */
	sad::GLContext* m_context;
	/*! A local log, where all messages will be stored
	 */
	sad::log::Log*  m_log; 
	/*! A font collection, attached to a renderer
	 */
	sad::FontManager*    m_font_manager;     
	/*! A texture collection, attached to a renderer
	 */
	sad::TextureManager* m_texture_manager; 
	/*! Returns mouse cursor, associated with renderer
	 */
	sad::MouseCursor*       m_cursor;
	/*! An OpenGL platform implementation part
	 */
	sad::OpenGL*            m_opengl;
	/*! A main loop handler, attached to renderer
	 */
	sad::MainLoop*          m_main_loop;
    /*! An interpolation for FPS
	 */
	sad::FPSInterpolation* m_fps_interpolation;
	/*! A scenes to be rendered
	 */
	sad::PtrVector<sad::Scene> m_scenes; 
	/*! A renderer for the primitives
	 */ 
	sad::PrimitiveRenderer *  m_primitiverenderer;
	/*! An input controls for user action callbacks
	 */
	sad::input::Controls*     m_controls;
	
	/*! A pipeline, as processes and tasks, which wille be performed in any time
		of runtime
	 */
	sad::pipeline::Pipeline*  m_pipeline;
	/*! Defines,  whether system pipeline, like FPS adding and resetting tasks was added to pipeline
	 */
	bool m_added_system_pipeline_tasks;
	/*! A cached path to executable file
	 */
	sad::String  m_executable_cached_path;
	
	/*! A settings for a renderer
	 */
	sad::Settings        m_glsettings;  
	/*! Destroys global instance of renderer
	 */
	static void destroyInstance();
	/*! Setups a OpenGL for first use
		\return success of operation
	 */
	bool initGLRendering();
    /*! Updates a scene
	 */
	void update();
	/*! Inits pipeline with data
	 */
	virtual void initPipeline();
	/*! Cleans a pipeline from renderer data
	 */
	virtual void cleanPipeline();
	/*! Called before rendering of scene
	 */
	virtual void startRendering();
	/*! Sequentially renders all scenes
	 */
	virtual void renderScenes();
	/*! Called at end when we must finished rendering scenes, at end of rendering
	 */
	virtual void finishRendering();
	/*! Adds a scene to scene list
		\param[in] s scene to be added
	 */
	virtual void addNow(sad::Scene * s);
	/*! Removes a scene to scene list
		\param[in] s scene to be removed
	 */
	virtual void removeNow(sad::Scene * s);
	/*! Clears scene from a scene list
	 */
	virtual void clearNow();
};

}
