/*! \mainpage Saddy
    
    A simple graphics engine for games. If you see any errors in documentation, or can
    give idea for improving a documentation, we would appreciate your help
 */
/*! \file   include/renderer.h
    \author FreakyBlast

    \brief  Declaration of Renderer - main class, for running 
    application is placed here.
*/
#pragma once
#include "log/log.h"

#include "resource/resourcefile.h"
#include "resource/tree.h"
#include "resource/error.h"

#include "settings.h"
#include "scene.h"
#include "sadpoint.h"
#include "sadptrhash.h"
#include "timer.h"
#include "maybe.h"
#include "temporarilyimmutablecontainer.h"
#include "sadptrvector.h"
#include "sadptrhash.h"
#include "sadmutex.h"
#include "primitiverenderer.h"
#include "texture.h"
#include "clipboard.h"

#include "imageformats/loader.h"

#include "animations/animationsanimations.h"

#include "util/pointercallback.h"

namespace sad
{

class Input;
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

namespace db
{
    class Database;
}

namespace util
{
    class SwapLayersTask;
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
friend class sad::util::SwapLayersTask;
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
    /*! Returns a clipboard object
        \return clipboard
     */
    sad::Clipboard* clipboard();
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
    /*! Sets fps interpolation for a renderer
        \param[in] i interpolation delegate
     */
    void setFPSInterpolation(sad::FPSInterpolation * i);
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
    /*! Loads resources to tree (default if treename is not supplied) from filename
        \param[in] filename a name of loaded files
        \param[in] treename a name of tree
        \return error list
     */
    sad::Vector<sad::resource::Error *> loadResources(
        const sad::String & filename,
        const sad::String & treename = ""
    );
    /*! Loads resources to tree (default if treename is not supplied) from filename
        \param[in] filename a name of loaded files
        \param[in] treename a name of tree
        \return error list
     */
    sad::Maybe<sad::String> tryLoadResources(
        const sad::String & filename,
        const sad::String & treename = ""
    );
    /*! Gets resource by name for a specified tree
        \param[in] resourcename a resource name
        \param[in] treename a name of tree, where resource should be taken from
     */
    template<
        typename _ResourceType
    >
    _ResourceType * resource(const sad::String & resourcename, const sad::String & treename = "")
    {
        sad::resource::Tree * tree = this->tree(treename);
        _ResourceType * result = NULL;
        if (tree)
        {
            result = tree->get<_ResourceType>(resourcename);
        }
        return result;
    }
    /*! Gets texture by name for a specified tree
        \param[in] resourcename a resource name
        \param[in] treename a name of tree, where resource should be taken from
     */
    sad::Texture * texture(const sad::String & resourcename, const sad::String & treename = "");
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
    virtual void reshape(int width, int height);
    /*! Adds scene to renderer
        \param[in] s scene
     */
    inline void addScene(sad::Scene* s)
    {
        this->add(s);
    }
    /*! Inserts scene into renderer
        \param[in] s scene
        \param[in] position a position
     */
    void insert(sad::Scene* s, size_t position);
    /*! Inserts scene into renderer
        \param[in] s scene
        \param[in] position a position
     */
    inline void insertScene(sad::Scene* s, size_t position)
    {
        this->insert(s, position);
    }
    /*! Removes scene from a renderer
        \param[in] s scene
     */
    inline void removeScene(sad::Scene* s)
    {
        this->remove(s);
    }
    /*! Clears scenes from a renderer
     */
    inline void clearScenes()
    {
        this->clear();
    }
    /*! Adds new scene to scene container
        \param[in] scene a scene to be rendered
     */
    void add(sad::Scene* scene);
    /*! Swap layers between two scenes
        \param[in] s1 first scene
        \param[in] s2 second scene
     */
    void swapLayers(sad::Scene* s1, sad::Scene* s2);
    /*! Returns order in which layer will be rendered. Higher order means the scene will
        be rendered later
        \param[in] s scene
        \return order, -1 if not found
     */
    int layer(sad::Scene* s);
    /*! Sets scene order to be rendered, removing from previous point
        \param[in] s a scene
        \param[in] layer a layer ordering to be set
     */
    void setLayer(sad::Scene * s, unsigned int layer);
    /*! Returns total scene objects in renderer
        \return total scene objects
     */
    unsigned int totalSceneObjects() const;
    /*! Sets primitive renderer
        \param[in] r renderer for primitives
     */
    void setPrimitiveRenderer(sad::PrimitiveRenderer * r);
    /*! Returns primitive renderer, which can be used to render primitives
        \return renderer for primitives
     */
    sad::PrimitiveRenderer * render() const;
    /*! Fetches path to executable without it's name and last delimiter. Returns empty string if fails
        \return executable path
     */
    const sad::String & executablePath() const;
    /*! Returns a resource tree, by it's mark in rendererer. By default, returns default tree,
        which guaranteed to exist, unless being explicitly removed by programmer
        \param[in] name name of resource tree
        \return tree if exists, or NULL if not found
     */
    sad::resource::Tree * tree(const sad::String & name = "") const;
    /*! Removes a tree from a renderer an returns owning pointer to it
        \param[in] name a name for resource tree
        \return tree if exists, or NULL if not found
     */
    sad::resource::Tree * takeTree(const sad::String & name);
    /*! Inserts new tree, replacing existing tree if needed. Existing tree's memory will be freed, if needed.
        If tree is NULL, nothing is done.
        \param[in] name a name for a tree
        \param[in] tree an inserted tree
     */
    void addTree(const sad::String & name, sad::resource::Tree * tree);
    /*! Removes a tree, freeeing it's  memory if it's exiss
        \param[in] name a name for a tree
     */
    void removeTree(const sad::String & name);
    /*! Returns true if current thread is renderer's own thread, where it's performs rendering
        \return whether current thread is renderer's
     */
    bool isOwnThread() const;
    /*! Tries to add new database to renderer. A renderer takes ownership on database.
        If database already exists, insertion is not performed and method returns false.
        \param[in] name a name of database
        \param[in] database a new database 
        \return result of insertion
     */
    bool addDatabase(const sad::String & name, sad::db::Database * database);
    /*! Removes database from a renderer, destroying it
        \param[in] name a name of database.
     */
    void removeDatabase(const sad::String & name);
    /*! Returns stored database by it's name
        \param[in] name a name for database
        \return database or  NULL if not found
     */
    sad::db::Database * database(const sad::String & name) const;
    /*! An animations list of renderer
     */
    sad::animations::Animations* animations() const;
    /*! Locks rendering of scenes
     */
    void lockRendering();
    /*! Unlocks rendering of scenes
     */
    void unlockRendering();
    /*! Sets new loader for a renderer
        \param[in] format a format, which loader is attached to
        \param[in] loader a loader data
     */
    void setTextureLoader(const sad::String& format, sad::imageformats::Loader* loader);
    /*! Returns a loader for texture
        \param[in] format a format
        \return NULL, if loader not found
     */
    sad::imageformats::Loader* textureLoader(const sad::String& format) const;
    /*! Shows message box for error message
        \param[in] title a title of window
        \param[in] message a text of message window
        \return whether message box is created
     */
    bool error(const sad::String& title, const sad::String& message);
    /*! Shows message box for warning message
        \param[in] title a title of window
        \param[in] message a text of message window
        \return whether message box is created
     */
    bool warning(const sad::String& title, const sad::String& message);
    /*! Shows message box for information message
        \param[in] title a title of window
        \param[in] message a text of message window
        \return whether message box is created
     */
    bool information(const sad::String& title, const sad::String& message);
    /*! Adds emergency shutdown callback
        \param[in] cb callback
     */
    void addEmergencyShutdownCallback(sad::util::PointerCallback<sad::Renderer>* cb);
    /*! Adds emergency shutdown callback
        \param[in] cb callback
     */
    void addEmergencyShutdownCallback(void (*cb)());
    /*! Adds emergency shutdown callback
        \param[in] o object
        \param[in] cb callback
     */
    template<
        typename _Object,
        typename _CalledObject
    >
    void addEmergencyShutdownCallback(_Object* o, void (_CalledObject::*cb)())
    {
        addEmergencyShutdownCallback(new sad::util::MethodZeroArgCallback<sad::Renderer, _Object, _CalledObject>(o, cb));
    }
    /*! Sets global translation offset for all of scenes
        \param[in] v vector
     */
    void setGlobalTranslationOffset(const sad::Vector3D& v);
    /*! Returns global translation offset for all of scenes
        \return global translation offset
     */
    const sad::Vector3D& globalTranslationOffset() const;
protected:
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
    /*! A resource trees, marked by a label, in case you need several
     */
    sad::Hash<sad::String, sad::resource::Tree*> m_resource_trees;
    /*! An inner renderer databases
     */
    sad::Hash<sad::String, sad::db::Database*> m_databases;
    /*! A lock for database operations
     */
    sad::Mutex m_database_lock;
    /*! A list for loaders for textures
     */
    sad::PtrHash<sad::String, sad::imageformats::Loader> m_texture_loaders;
    /*! A list of animations
     */
    sad::animations::Animations* m_animations;
    /*! Clipboard for working with system clipboard
     */
    sad::Clipboard m_clipboard;
    
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
    /*! A context thread id, storead as void
     */
    void*   m_context_thread;
    /*! A mutex, which locks rendering of scenes
     */
    sad::Mutex m_lockrendering;
    /*! An emergency shutdown callbacks
     */
    sad::PtrVector<sad::util::PointerCallback<sad::Renderer> > m_emergency_shutdown_callbacks;

    /*! A global translation offset, that should be applied to all of scenes cameras
     */
    sad::Vector3D m_global_translation_offset;

    /*! Copying a renderer, due to held system resources is disabled
    \param[in] o other renderer
    */
    Renderer(const Renderer& o);
    /*! Copying a renderer, due to held system resources is disabled
    \param[in] o other renderer
    \return self-rederence
    */
    Renderer& operator=(const Renderer& o);
    /*! Initializes window and context. Do not call this functions, unless you want to run renderer's loop manualy,
        via runOnce()
        \return true in case everything is ok, otherwise false
    */
    virtual bool initRendererBeforeLoop();
    /*! Runs a renderer. Note, that window should be created before start, and
        context should be initialized before method fired.

        Note, that you MUST call initRendererBeforeLoop before calling this function and call deinitRendererAfterLoop, when done
    */
    virtual void runOnce();
    /*! Destroys window and context, makes required cleanups, when renderer is done.
        Call this, after main loop of renderer is finished 
     */
    virtual void deinitRendererAfterLoop();
    /*! Destroys global instance of renderer
     */
    static void destroyInstance();
    /*! Setups a OpenGL for first use
        \return success of operation
     */
    bool initGLRendering();
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
    /*! Inserts a scene into specified position.
        This function is hidden, since it should be used in some advanced containers
        \param[in] s a scene to be added
        \param[in] position a used position
     */
    virtual void insertNow(sad::Scene* s, size_t position);
};

}
