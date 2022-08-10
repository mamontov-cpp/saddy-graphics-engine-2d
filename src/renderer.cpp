#include "renderer.h"

#include "scene.h"
#include "camera.h"
#include "window.h"
#include "sadscopedlock.h"
#include "glcontext.h"
#include "mainloop.h"
#include "mousecursor.h"
#include "opengl.h"
#include "fpsinterpolation.h"
#include "input/controls.h"

#include "pipeline/pipeline.h"

#include "os/windowhandles.h"
#include "os/glheaders.h"
#include "os/threadimpl.h"
#include "os/extensionfunctions.h"
#include "os/gltexturedgeometry3d.h"
#include "os/gltexturedgeometry2d.h"
#include "os/gluntexturedgeometry3d.h"
#include "os/gluntexturedgeometry2d.h"
#include "os/ubo.h"
#include "os/glspritegeometrystorages.h"
#include "os/glfontgeometries.h"

#include "db/dbdatabase.h"
#include "db/dbtypename.h"

#include "util/swaplayerstask.h"
#include "util/free.h"

#include "imageformats/bmploader.h"
#include "imageformats/pngloader.h"
#include "imageformats/tgaloader.h"
#include "imageformats/srgbaloader.h"

#include "fontshaderfunction.h"

#ifdef LINUX
    #include <stdio.h>
    #include <unistd.h>
#endif

#ifdef X11
    void SafeXInitThreads();
#endif

// ============================================================ PUBLIC METHODS ============================================================


sad::Renderer * sad::Renderer::m_instance = nullptr;

sad::Renderer::Renderer()
: 
m_window(new sad::Window()),
m_context(new sad::GLContext()),
m_log(new sad::log::Log()),
m_cursor(new sad::MouseCursor()),
m_opengl(new sad::OpenGL()),
m_main_loop(new sad::MainLoop()),
m_fps_interpolation(new sad::FPSInterpolation()),
m_primitive_renderer(new sad::PrimitiveRenderer()),
m_controls(new sad::input::Controls()),
m_animations(new sad::animations::Animations()),
m_pipeline(new sad::pipeline::Pipeline()),
m_added_system_pipeline_tasks(false),
m_default_textures_shader_3d(nullptr),
m_default_texture_shader_function_3d(nullptr),
m_default_no_textures_shader_3d(nullptr),
m_default_no_textures_shader_function_3d(nullptr),
m_default_textures_shader_2d(nullptr),
m_default_texture_shader_function_2d(nullptr),
m_default_no_textures_shader_2d(nullptr),
m_default_no_textures_shader_function_2d(nullptr),
m_default_font_shader(nullptr),
m_default_font_shader_function(nullptr),
m_default_font_line_shader(nullptr),
m_default_font_line_shader_function(nullptr),
m_gl_sprite_geometry_storages(nullptr),
m_free_texture_buffer_after_upload(false)
{
#ifdef X11
    SafeXInitThreads();
#endif  
    m_window->setRenderer(this);
    m_cursor->setRenderer(this);
    m_cursor->addRef();
    m_opengl->setRenderer(this);
    m_main_loop->setRenderer(this);

    m_camera_buffer = new sad::os::UBO(this, 32 * sizeof(GLfloat));

    setTextureLoader("BMP", new sad::imageformats::BMPLoader());
    setTextureLoader("TGA", new sad::imageformats::TGALoader());
    setTextureLoader("PNG", new sad::imageformats::PNGLoader());
    setTextureLoader("SRGBA", new sad::imageformats::SRGBALoader());
    setTextureLoader("SR5G6B5", new sad::imageformats::PixelStorageLoader(sad::imageformats::PixelStorageLoader::SR5G6B5Settings));
    setTextureLoader("SR4G4B4A4", new sad::imageformats::PixelStorageLoader(sad::imageformats::PixelStorageLoader::SR4G4B4A4Settings));
    setTextureLoader("SR3G3B2", new sad::imageformats::PixelStorageLoader(sad::imageformats::PixelStorageLoader::SR3G3B2Settings));



    sad::resource::Tree * defaulttree = new sad::resource::Tree(this);
    defaulttree->addRef();
    m_resource_trees.insert("", defaulttree);

    // Add stopping a main loop to quite events of controls to make window close
    // when user closes a window
    m_controls->add(*(sad::input::EventType::ET_Quit), m_main_loop, &sad::MainLoop::stop);

    m_gl_sprite_geometry_storages = new sad::os::GLSpriteGeometryStorages();
    m_gl_sprite_geometry_storages->setRenderer(this);

    // Set context thread
    m_context_thread = reinterpret_cast<void*>(sad::os::current_thread_id());
    // Init pipeline to make sure, that user can add actions after rendering step, before 
    // renderer started
    this->Renderer::initPipeline();
}


void sad::Renderer::reset()
{
    freeCurrentState();

    m_window = new sad::Window();
    m_context = new sad::GLContext();
    m_log = new sad::log::Log();
    m_cursor = new sad::MouseCursor();
    m_opengl = new sad::OpenGL();
    m_main_loop = new sad::MainLoop();
    m_fps_interpolation = new sad::FPSInterpolation();
    m_primitive_renderer = new sad::PrimitiveRenderer();
    m_controls = new sad::input::Controls();
    m_animations = new sad::animations::Animations();
    m_pipeline = new sad::pipeline::Pipeline();
    m_added_system_pipeline_tasks = false;
    m_default_textures_shader_3d = nullptr;
    m_default_texture_shader_function_3d = nullptr;
    m_default_no_textures_shader_3d = nullptr;
    m_default_no_textures_shader_function_3d = nullptr;
    m_default_textures_shader_2d = nullptr;
    m_default_texture_shader_function_2d = nullptr;
    m_default_no_textures_shader_2d = nullptr;
    m_default_no_textures_shader_function_2d = nullptr;
    m_default_font_shader = nullptr;
    m_default_font_shader_function = nullptr;
    m_default_font_line_shader = nullptr;
    m_default_font_line_shader_function = nullptr;
    m_gl_sprite_geometry_storages = nullptr;
    m_free_texture_buffer_after_upload = false;

    m_window->setRenderer(this);
    m_cursor->setRenderer(this);
    m_cursor->addRef();
    m_opengl->setRenderer(this);
    m_main_loop->setRenderer(this);

    m_camera_buffer = new sad::os::UBO(this, 32 * sizeof(GLfloat));

    setTextureLoader("BMP", new sad::imageformats::BMPLoader());
    setTextureLoader("TGA", new sad::imageformats::TGALoader());
    setTextureLoader("PNG", new sad::imageformats::PNGLoader());
    setTextureLoader("SRGBA", new sad::imageformats::SRGBALoader());
    setTextureLoader("SR5G6B5", new sad::imageformats::PixelStorageLoader(sad::imageformats::PixelStorageLoader::SR5G6B5Settings));
    setTextureLoader("SR4G4B4A4", new sad::imageformats::PixelStorageLoader(sad::imageformats::PixelStorageLoader::SR4G4B4A4Settings));
    setTextureLoader("SR3G3B2", new sad::imageformats::PixelStorageLoader(sad::imageformats::PixelStorageLoader::SR3G3B2Settings));



    sad::resource::Tree * defaulttree = new sad::resource::Tree(this);
    defaulttree->addRef();
    m_resource_trees.insert("", defaulttree);

    // Add stopping a main loop to quite events of controls to make window close
    // when user closes a window
    m_controls->add(*(sad::input::EventType::ET_Quit), m_main_loop, &sad::MainLoop::stop);

    m_gl_sprite_geometry_storages = new sad::os::GLSpriteGeometryStorages();
    m_gl_sprite_geometry_storages->setRenderer(this);

    // Set context thread
    m_context_thread = reinterpret_cast<void*>(sad::os::current_thread_id());
    // Init pipeline to make sure, that user can add actions after rendering step, before 
    // renderer started
    this->Renderer::initPipeline();
}

template<typename T>
inline void del_ref_if_not_nullptr(T*& obj)
{
    if (obj != nullptr)
    {
        obj->delRef();
        obj = nullptr;
    }
}

inline void destroy_shader_if_not_nullptr(sad::Shader* shader)
{
    if (shader != nullptr)
    {
        shader->tryDestroy();
    }
}

sad::Renderer::~Renderer(void)
{
    freeCurrentState();
}

void sad::Renderer::setScene(Scene * scene)
{
    clear();
    add(scene);
}

const sad::Vector<sad::Scene*>& sad::Renderer::scenes() const
{
    return m_scenes;
}


void sad::Renderer::init(const sad::Settings& _settings)
{
    SL_INTERNAL_SCOPE("sad::Renderer::init", (*this));
    m_gl_settings = _settings;
    m_window->setCreationSize(m_gl_settings.width(), m_gl_settings.height());
}

bool sad::Renderer::run()
{
    SL_INTERNAL_SCOPE("sad::Renderer::run()", *this);
 
    bool success = this->initRendererBeforeLoop();
 
    if (success)
    {
        mainLoop()->run();
        this->deinitRendererAfterLoop();
    }

    return success;
}

void sad::Renderer::quit()
{
    if (m_window->valid())
    {
        m_window->close();
    }
}

static sad::Mutex sad_renderer_instance_lock;

sad::Renderer* sad::Renderer::ref()
{
    if (sad::Renderer::m_instance == nullptr)
    {
        sad_renderer_instance_lock.lock();
        if (sad::Renderer::m_instance == nullptr)
        {
            sad::Renderer::m_instance = new sad::Renderer();
            atexit(sad::Renderer::destroyInstance);
        }
        sad_renderer_instance_lock.unlock();
    }
    return sad::Renderer::m_instance;
}

double sad::Renderer::fps() const
{
    return fpsInterpolation()->fps();
}

void sad::Renderer::setWindowTitle(const sad::String & s)
{
    m_window->setTitle(s);
}

void sad::Renderer::makeFixedSize()
{
    m_window->makeFixedSize();
}

void sad::Renderer::makeResizeable()
{
    m_window->makeResizeable();
}


void sad::Renderer::toggleFullscreen()
{
    if (m_window->fullscreen())
    {
        m_window->leaveFullscreen();
    } 
    else
    {
        m_window->enterFullscreen();
    }
}

bool sad::Renderer::running()
{
    return m_main_loop->running();
}

bool sad::Renderer::hasValidContext()
{
    return m_window->valid() && m_context->valid();
}


sad::MaybePoint3D sad::Renderer::cursorPosition() const
{
    return this->cursor()->position();
}

void sad::Renderer::setCursorPosition(const sad::Point2D & p)
{
    this->cursor()->setPosition(p);
}

// ReSharper disable once CppMemberFunctionMayBeConst
sad::log::Log * sad::Renderer::log()
{
    return m_log;
}

// ReSharper disable once CppMemberFunctionMayBeConst
sad::Window * sad::Renderer::window()
{
    return m_window;
}

// ReSharper disable once CppMemberFunctionMayBeConst
sad::GLContext * sad::Renderer::context()
{
    return m_context;
}

sad::Clipboard* sad::Renderer::clipboard()
{
    return &m_clipboard;
}

const sad::Settings & sad::Renderer::settings() const
{
    return m_gl_settings;
}

sad::MouseCursor* sad::Renderer::cursor() const
{
    return m_cursor;
}

void sad::Renderer::setCursor(sad::MouseCursor * cursor)
{
    m_cursor->delRef();
    m_cursor = cursor;
    m_cursor->addRef();
}

sad::OpenGL * sad::Renderer::opengl() const
{
    return m_opengl;
}

sad::MainLoop * sad::Renderer::mainLoop() const
{
    return m_main_loop;
}

void sad::Renderer::setFPSInterpolation(sad::FPSInterpolation * i)
{
    assert( i );
    delete m_fps_interpolation;
    m_fps_interpolation = i;
}

sad::FPSInterpolation * sad::Renderer::fpsInterpolation() const
{
    return m_fps_interpolation;
}

sad::pipeline::Pipeline* sad::Renderer::pipeline() const
{
    return m_pipeline;
}

sad::input::Controls* sad::Renderer::controls() const
{
    return m_controls;
}

sad::Vector<sad::resource::Error *> sad::Renderer::loadResources(
        const sad::String & filename,
        const sad::String & tree_name
)
{
    sad::Vector<sad::resource::Error *> result;
    if (m_resource_trees.contains(tree_name))
    {
        result = m_resource_trees[tree_name]->loadFromFile(filename);
    }
    else
    {
        result << new sad::resource::TreeNotFound(tree_name);
    }
    return result;
}


sad::Maybe<sad::String> sad::Renderer::tryLoadResources(
    const sad::String & filename,
    const sad::String & tree_name
)
{
    return sad::resource::errorsToString(this->loadResources(filename, tree_name));
}

sad::Texture * sad::Renderer::texture(
    const sad::String & resource_name, 
    const sad::String & tree_name
)
{
    return resource<sad::Texture>(resource_name, tree_name);  
}

template<typename K, typename V>
inline void unload_resources_from_hash(const sad::Hash<K, V>& resources)
{
    for (auto it = resources.const_begin(); it != resources.const_end(); ++it) 
    {
        it->second->unload();
    }
}

void sad::Renderer::emergencyShutdown()
{
    // Unload all textures, because after shutdown context will be lost
    // and glDeleteTextures could lead to segmentation fault
    for(sad::PtrHash<sad::String, sad::resource::Tree>::iterator it = m_resource_trees.begin();
        it != m_resource_trees.end();
        ++it)
    {
        it.value()->unloadResourcesFromGPU();
    }

    for(size_t i = 0; i < m_emergency_shutdown_callbacks.size(); i++)
    {
        m_emergency_shutdown_callbacks[i]->call(this);
    }

    unload_resources_from_hash(m_sizes_to_textured_geometry_3d);
    unload_resources_from_hash(m_sizes_to_textured_geometry_2d);
    unload_resources_from_hash(m_sizes_to_untextured_geometry_3d);
    unload_resources_from_hash(m_sizes_to_untextured_geometry_2d);
    m_gl_sprite_geometry_storages->unloadFromGPU();

    destroy_shader_if_not_nullptr(m_default_textures_shader_3d);
    destroy_shader_if_not_nullptr(m_default_no_textures_shader_3d);
    destroy_shader_if_not_nullptr(m_default_textures_shader_2d);
    destroy_shader_if_not_nullptr(m_default_no_textures_shader_2d);
    destroy_shader_if_not_nullptr(m_default_font_shader);
    destroy_shader_if_not_nullptr(m_default_font_line_shader);

    for (size_t i = 0; i < m_gl_font_geometries.size(); i++)
    {
        m_gl_font_geometries[i]->unload();
    }

    m_camera_buffer->tryUnload();


    // Destroy context and window, so nothing could go wrong
    this->context()->destroy();
    this->window()->destroy();
    // Stop main loop
    this->mainLoop()->stop();
}

sad::Point3D sad::Renderer::mapToViewport(const sad::Point2D & p)
{
    sad::Point3D result;
    if (window()->valid() && context()->valid())
    {
        sad::Point2D windowpoint = this->window()->toClient(p);
        result = this->context()->mapToViewport(windowpoint, m_gl_settings.ztest());
    }
    return result;
}

void sad::Renderer::reshape(int width, int height)
{
    SL_INTERNAL_SCOPE(fmt::Format("sad::Renderer::reshape({0}, {1})") << width << height, *this);

    if (width == 0) {
        width = 1;
    }
    // Reset viewport for window
    glViewport (0, 0, width, height);
    
    // Clear projection matrix
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
  
    //  Set perspective projection
    GLfloat aspectratio = static_cast<GLfloat>(width)/static_cast<GLfloat>(height);
    gluPerspective(
        m_gl_settings.fov(), 
        aspectratio,
        m_gl_settings.znear(), 
        m_gl_settings.zfar()
    );      
    
    // Clear model-view matrix
    glMatrixMode (GL_MODELVIEW);                                        
    glLoadIdentity ();  
}

void sad::Renderer::insert(sad::Scene* s, size_t position)
{
    if (s)
    {
        s->setRenderer(this);
        this->sad::TemporarilyImmutableContainer<sad::Scene>::insert(s, position);
    }
}

void sad::Renderer::add(sad::Scene * scene)
{
    if (scene)
        scene->setRenderer(this);
    this->sad::TemporarilyImmutableContainer<sad::Scene>::add(scene);
}

void sad::Renderer::swapLayers(sad::Scene* s1, sad::Scene* s2)
{
    int layer1 = this->layer(s1);
    int layer2 = this->layer(s2);
    if (layer1 != -1 && layer2 != -2)
    {
        sad::pipeline::AbstractTask* t = new sad::util::SwapLayersTask(this, s1, s2, layer1, layer2);
        this->pipeline()->insertStep(sad::pipeline::PipelineInsertionType::PIT_END, t);
    }
}

int  sad::Renderer::layer(sad::Scene * s)
{
    std::vector<sad::Scene*>::iterator it = std::find(m_scenes.begin(), m_scenes.end(), s);
    if (it == m_scenes.end())
    {
        return -1;
    }
    return it - m_scenes.begin();
}

void sad::Renderer::setLayer(sad::Scene * s, unsigned int layer)
{
    int oldlayer = this->layer(s);
    if (s)
    {
        s->setRenderer(this);
    }

    if (oldlayer != -1)
    {
        m_scenes.removeAt(oldlayer);
        if (static_cast<int>(layer) > oldlayer)
        {
            layer--;
        }
    }
        
    if (layer >= m_scenes.count())
    {
        m_scenes << s;
    }
    else
    {
        m_scenes.insert(s, layer);
    }
}

unsigned int sad::Renderer::totalSceneObjects() const
{
    unsigned int result = 0;
    for(size_t i = 0; i < m_scenes.size(); i++)
    {
        result += m_scenes[i]->objectCount();
    }
    return result;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void sad::Renderer::setPrimitiveRenderer(sad::PrimitiveRenderer * r)
{
    delete m_primitive_renderer; 
    m_primitive_renderer = r;
}


sad::PrimitiveRenderer * sad::Renderer::render() const
{
    return m_primitive_renderer;
}

#ifdef LINUX
// http://www.gnu.org/software/hurd/user/tlecarrour/porting_guide_for_dummies.html
static char *readlink_malloc(const char *filename)
{
    int size = 100;

    while (1) 
    {
        char *buff = (char*)malloc(size);
        if (buff == nullptr)
            return nullptr;
        int nchars = readlink(filename, buff, size);
        if (nchars < 0)
        {
            free(buff);
            return nullptr;
        }
        if (nchars < size) 
        {
            buff[nchars] = '\0';
            return buff;
        }
        free (buff);
        size *= 2;
    }
}
#endif

const sad::String & sad::Renderer::executablePath() const
{
    if (m_executable_cached_path.length() == 0)
    {
#ifdef WIN32
        char result[_MAX_PATH+1];
        GetModuleFileNameA(nullptr, result, _MAX_PATH);
        sad::String * path = &(const_cast<sad::Renderer*>(this)->m_executable_cached_path);
        *path =  result;        
        int pos = path->getLastOccurrence("\\");
        if (pos > 0)
        {
            *path = path->subString(0, pos);
        }
#endif

#ifdef LINUX
        char proc[32];
        sprintf(proc, "/proc/%d/exe", getpid());
        char * buffer = readlink_malloc(proc);
        if(buffer != 0)
        {       
            sad::String * path = &(const_cast<sad::Renderer*>(this)->m_executable_cached_path);     
            *path = buffer;
            free(buffer);
            int pos = path->getLastOccurrence("/");
            if (pos > 0)
            {
                *path = path->subString(0, pos);
            }
        }
#endif
    }
    return m_executable_cached_path;
}

sad::resource::Tree * sad::Renderer::tree(const sad::String & name) const
{
    if (m_resource_trees.contains(name))
    {
        return m_resource_trees[name];
    }
    return nullptr;
}

sad::resource::Tree * sad::Renderer::takeTree(const sad::String & name)
{
    if (m_resource_trees.contains(name))
    {
        sad::resource::Tree * result =  m_resource_trees[name];
        result->setRenderer(nullptr);
        m_resource_trees.remove(name);
        // A caller should call delRef() after done
        return result;
    }
    return nullptr;
}

void sad::Renderer::addTree(const sad::String & name, sad::resource::Tree * tree)
{
    if (!tree)
    {
        return;
    }
    if (m_resource_trees.contains(name))
    {
        sad::resource::Tree * result =  m_resource_trees[name];
        m_resource_trees.remove(name);
        result->delRef();
    }
    tree->addRef();
    tree->setRenderer(this);
    m_resource_trees.insert(name, tree);
}


void sad::Renderer::removeTree(const sad::String & name)
{
    if (m_resource_trees.contains(name))
    {
        sad::resource::Tree * result =  m_resource_trees[name];
        m_resource_trees.remove(name);
        result->delRef();
    }   
}


bool sad::Renderer::isOwnThread() const
{
    return (reinterpret_cast<void*>(sad::os::current_thread_id()) == m_context_thread);
}

bool sad::Renderer::addDatabase(const sad::String & name, sad::db::Database * database)
{
    sad::ScopedLock lock(&m_database_lock);
    assert( database );
    if (m_databases.contains(name))
    {
        return false;
    }
    database->setRenderer(this);
    database->addRef();
    m_databases.insert(name, database);
    return true;
}

void sad::Renderer::removeDatabase(const sad::String & name)
{
    sad::ScopedLock lock(&m_database_lock);
    if (m_databases.contains(name))
    {
        m_databases[name]->delRef();
        m_databases.remove(name);
    }
}

sad::db::Database * sad::Renderer::database(const sad::String & name) const
{
    sad::ScopedLock lock(&(const_cast<sad::Renderer*>(this)->m_database_lock));
    if (m_databases.contains(name))
    {
        return m_databases[name];
    }
    return nullptr;
}

sad::animations::Animations* sad::Renderer::animations() const
{
    return m_animations;
}

void sad::Renderer::lockRendering()
{
    m_lock_rendering.lock(); 
}

void sad::Renderer::unlockRendering()
{
    m_lock_rendering.unlock();   
}

void sad::Renderer::setTextureLoader(const sad::String& format, sad::imageformats::Loader* loader)
{
    if (m_texture_loaders.contains(format))
    {
        delete m_texture_loaders[format]; //-V515
        m_texture_loaders[format] = loader;
    }
    else
    {
        m_texture_loaders.insert(format, loader);
    }
}

sad::imageformats::Loader* sad::Renderer::textureLoader(const sad::String& format) const
{
    sad::imageformats::Loader* l = nullptr;
    if (m_texture_loaders.contains(format))
    {
        l = m_texture_loaders[format];
    }
    return l;
}

#ifdef X11

bool SDL_MessageBoxError(
  const char * title,
  const char * message
);
bool SDL_MessageBoxWarning(
  const char * title,
  const char * message
);
bool SDL_MessageBoxInformation(
  const char * title,
  const char * message
);

#endif

#ifdef WIN32

bool SDL_MessageBoxError(
  const char * title,
  const char * message
);
bool SDL_MessageBoxWarning(
  const char * title,
  const char * message
);
bool SDL_MessageBoxInformation(
  const char * title,
  const char * message
);

#endif

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
bool sad::Renderer::error(const sad::String& title, const sad::String& message)
{
    // ReSharper disable once CppInitializedValueIsAlwaysRewritten
    bool result = false;
#ifdef X11
    result = SDL_MessageBoxError(title.c_str(), message.c_str());
#endif

#ifdef WIN32
    result = SDL_MessageBoxError(title.c_str(), message.c_str());
#endif

    return result;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
bool sad::Renderer::warning(const sad::String& title, const sad::String& message)
{
    // ReSharper disable once CppInitializedValueIsAlwaysRewritten
    bool result = false;
#ifdef X11
    result =  SDL_MessageBoxWarning(title.c_str(), message.c_str());
#endif

#ifdef WIN32
    result =  SDL_MessageBoxWarning(title.c_str(), message.c_str());
#endif

    return result;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
bool sad::Renderer::information(const sad::String& title, const sad::String& message)
{
    // ReSharper disable once CppInitializedValueIsAlwaysRewritten
    bool result = false;
#ifdef X11
    result = SDL_MessageBoxInformation(title.c_str(), message.c_str());
#endif

#ifdef WIN32
    result = SDL_MessageBoxInformation(title.c_str(), message.c_str());
#endif
    return result;
}

void sad::Renderer::addEmergencyShutdownCallback(sad::util::PointerCallback<sad::Renderer>* cb)
{
    m_emergency_shutdown_callbacks << cb;
}

void sad::Renderer::addEmergencyShutdownCallback(void (*cb)())
{
    m_emergency_shutdown_callbacks << new sad::util::FreeZeroArgCallback<sad::Renderer>(cb);
}

void sad::Renderer::setGlobalTranslationOffset(const sad::Vector3D& v)
{
    m_global_translation_offset = v;
    for(auto scene : m_scenes) {
         if (scene) {
            sad::Camera& camera = scene->camera();
            camera.clearTransformCache();
        }
    }
}

const sad::Vector3D& sad::Renderer::globalTranslationOffset() const
{
    return m_global_translation_offset;
}

sad::os::GLTexturedGeometry3D* sad::Renderer::texturedGeometry3DForPoints(unsigned int points)
{
    if (points == 0)
    {
        return nullptr;
    }
    if (!m_sizes_to_textured_geometry_3d.contains(points))
    {
        sad::os::GLTexturedGeometry3D* g = new sad::os::GLTexturedGeometry3D(this, points);
        m_sizes_to_textured_geometry_3d.insert(points, g);
        return g;
    }
    else
    {
        return m_sizes_to_textured_geometry_3d[points];
    }
}

sad::os::GLTexturedGeometry2D* sad::Renderer::texturedGeometry2DForPoints(unsigned int points)
{
    if (points == 0)
    {
        return nullptr;
    }
    if (!m_sizes_to_textured_geometry_2d.contains(points))
    {
        sad::os::GLTexturedGeometry2D* g = new sad::os::GLTexturedGeometry2D(this, points);
        m_sizes_to_textured_geometry_2d.insert(points, g);
        return g;
    }
    else
    {
        return m_sizes_to_textured_geometry_2d[points];
    }
}

sad::os::GLUntexturedGeometry3D* sad::Renderer::untexturedGeometry3DForPoints(unsigned int points)
{
    if (points == 0)
    {
        return nullptr;
    }
    if (!m_sizes_to_untextured_geometry_3d.contains(points))
    {
        sad::os::GLUntexturedGeometry3D* g = new sad::os::GLUntexturedGeometry3D(this, points);
        m_sizes_to_untextured_geometry_3d.insert(points, g);
        return g;
    }
    else
    {
        return m_sizes_to_untextured_geometry_3d[points];
    }
}

sad::os::GLUntexturedGeometry2D* sad::Renderer::untexturedGeometry2DForPoints(unsigned int points)
{
    if (points == 0)
    {
        return nullptr;
    }
    if (!m_sizes_to_untextured_geometry_2d.contains(points))
    {
        sad::os::GLUntexturedGeometry2D* g = new sad::os::GLUntexturedGeometry2D(this, points);
        m_sizes_to_untextured_geometry_2d.insert(points, g);
        return g;
    }
    else
    {
        return m_sizes_to_untextured_geometry_2d[points];
    }
}

sad::ShaderFunction* sad::Renderer::defaultShaderFunctionForTextures3d() 
{
    this->tryInitShaders();
    return m_default_texture_shader_function_3d;
}

sad::ShaderFunction* sad::Renderer::defaultShaderFunctionWithoutTextures3d()
{
    this->tryInitShaders();
    return m_default_no_textures_shader_function_3d;
}

sad::ShaderFunction* sad::Renderer::defaultShaderFunctionForTextures2d()
{
    this->tryInitShaders();
    return m_default_texture_shader_function_2d;
}

sad::ShaderFunction* sad::Renderer::defaultShaderFunctionWithoutTextures2d()
{
    this->tryInitShaders();
    return m_default_no_textures_shader_function_2d;
}

sad::FontShaderFunction* sad::Renderer::defaultFontShaderFunction()
{
    this->tryInitShaders();
    return m_default_font_shader_function;
}

sad::FontShaderFunction* sad::Renderer::defaultFontLineShaderFunction()
{
    this->tryInitShaders();
    return m_default_font_line_shader_function;
}


sad::os::UBO* sad::Renderer::cameraObjectBuffer() const
{
    return m_camera_buffer;
}

sad::os::GLTexturedGeometry2D* sad::Renderer::takeTextured2D()  const
{
    return m_gl_sprite_geometry_storages->takeTextured2D();
}

sad::os::GLTexturedGeometry3D* sad::Renderer::takeTextured3D() const
{
    return m_gl_sprite_geometry_storages->takeTextured3D();
}

sad::os::GLUntexturedGeometry2D* sad::Renderer::takeUntextured2D() const
{
    return m_gl_sprite_geometry_storages->takeUntextured2D();
}

sad::os::GLUntexturedGeometry3D* sad::Renderer::takeUntextured3D() const
{
    return m_gl_sprite_geometry_storages->takeUntextured3D();
}

void sad::Renderer::storeGeometry(sad::os::GLTexturedGeometry2D* g)
{
    m_gl_sprite_geometry_storages->store(g);
}

void sad::Renderer::storeGeometry(sad::os::GLTexturedGeometry3D* g)
{
    m_gl_sprite_geometry_storages->store(g);
}

void sad::Renderer::storeGeometry(sad::os::GLUntexturedGeometry2D* g)
{
    m_gl_sprite_geometry_storages->store(g);
}

void sad::Renderer::storeGeometry(sad::os::GLUntexturedGeometry3D* g)
{
    m_gl_sprite_geometry_storages->store(g);
}

void sad::Renderer::addFontGeometries(sad::os::GLFontGeometries* g)
{
    if (std::find(m_gl_font_geometries.begin(), m_gl_font_geometries.end(), g) == m_gl_font_geometries.end())
    {
        m_gl_font_geometries.add(g);
    }
}

void sad::Renderer::removeFontGeometries(sad::os::GLFontGeometries* g)
{
    m_gl_font_geometries.removeFirst(g);
    if (g)
    {
        g->unload();
    }
}

void sad::Renderer::setShouldFreeTextureBuffersAfterUpload(bool new_value)
{
    m_free_texture_buffer_after_upload = new_value;
}

bool sad::Renderer::shouldFreeTextureBuffersAfterUpload() const
{
    return m_free_texture_buffer_after_upload;
}

// ============================================================ PROTECTED METHODS ============================================================

bool sad::Renderer::initRendererBeforeLoop()
{
    SL_INTERNAL_SCOPE("sad::Renderer::initRendererBeforeLoop()", *this);
    bool success = true;
    if (m_window->valid() == false)
    {
        success = m_window->create();
        if (!success)
        {
            SL_LOCAL_FATAL("Cannot create window\n", *this);
        }
    }


    // Try to create context if needed
    if (m_context->valid() == false && success)
    {
        // Set context thread
        m_context_thread = reinterpret_cast<void*>(sad::os::current_thread_id());
        success = m_context->createFor(m_window);
        if (!success)
        {
            SL_LOCAL_FATAL("Failed to create OpenGL context\n", *this);
            m_window->destroy();
        }
        else
        {
            m_window->initialize();
            this->initGLRendering();
        }
    }

    if (success)
    {
        this->initPipeline();
        this->cursor()->insertHandlersIfNeeded();
        this->mainLoop()->initMainLoop();
        this->opengl()->extensionFunctions()->tryInit();
    }

    return success;
}

void sad::Renderer::runOnce()
{
    assert(m_window->valid());
    assert(m_context->valid());


    mainLoop()->run(SAD_MAIN_LOOP_RUN_ONLY_ONCE);
}


void sad::Renderer::deinitRendererAfterLoop()
{
    SL_INTERNAL_SCOPE("sad::Renderer::deinitRendererAfterLoop()", *this);
    this->mainLoop()->deinitMainLoop();
    cursor()->removeHandlersIfNeeded();
    cleanPipeline();

    unload_resources_from_hash(m_sizes_to_textured_geometry_3d);
    unload_resources_from_hash(m_sizes_to_textured_geometry_2d);
    unload_resources_from_hash(m_sizes_to_untextured_geometry_3d);
    unload_resources_from_hash(m_sizes_to_untextured_geometry_2d);

    destroy_shader_if_not_nullptr(m_default_textures_shader_3d);
    destroy_shader_if_not_nullptr(m_default_no_textures_shader_3d);
    destroy_shader_if_not_nullptr(m_default_textures_shader_2d);
    destroy_shader_if_not_nullptr(m_default_no_textures_shader_2d);
    destroy_shader_if_not_nullptr(m_default_font_shader);
    destroy_shader_if_not_nullptr(m_default_font_line_shader);
    m_camera_buffer->tryUnload();

    for (size_t i = 0; i < m_gl_font_geometries.size(); i++)
    {
        m_gl_font_geometries[i]->unload();
    }


    m_context->destroy();
    m_window->destroy();
}


void sad::Renderer::destroyInstance()
{
    delete  sad::Renderer::m_instance;
}

bool sad::Renderer::initGLRendering()
{
    SL_INTERNAL_SCOPE("sad::Renderer::initGLRendering()", *this);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f,0.0f,0.0f,0.0f); 
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);

    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    
    const char * version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    if (version!=nullptr)
    {
        SL_LOCAL_INTERNAL(sad::String("running OpenGL ")+sad::String(version), *this);
        if (version[0]>'1' || version[2] >= '4')
            glHint(GL_GENERATE_MIPMAP_HINT,GL_NICEST);
    }

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    
    reshape(m_gl_settings.width(),m_gl_settings.height());
    
    glFinish();
    return true;
}

void sad::Renderer::initPipeline()
{
    if (this->m_added_system_pipeline_tasks == false)
    {
        this->pipeline()
            ->systemPrependSceneRenderingWithProcess(this, &sad::Renderer::fpsInterpolation, &sad::FPSInterpolation::start)
            ->mark("sad::FPSInterpolation::start");
        this->pipeline()
            ->systemPrependSceneRenderingWithProcess(this, &sad::Renderer::startRendering)
            ->mark("sad::Renderer::startRendering");

        this->pipeline()
            ->systemAppendProcess(this, &sad::Renderer::cursor, &sad::MouseCursor::renderCursorIfNeedTo)
            ->mark("sad::MouseCursor::renderCursorIfNeedTo");
        this->pipeline()
            ->systemAppendProcess(this, &sad::Renderer::finishRendering)
            ->mark("sad::Renderer::finishRendering");
        this->pipeline()
            ->systemAppendProcess(this, &sad::Renderer::fpsInterpolation, &sad::FPSInterpolation::stop)
            ->mark("sad::FPSInterpolation::stop");
        m_added_system_pipeline_tasks =  true;
    }
    //We should append rendering task to pipeline to make sure, that scene would be rendered.
    if (this->pipeline()->contains("sad::Renderer::renderScenes") == false)
    {
        this->pipeline()
            ->appendProcess(this, &sad::Renderer::renderScenes)
            ->mark("sad::Renderer::renderScenes");
        this->pipeline()
            ->appendProcess(m_animations, &sad::animations::Animations::process)
            ->mark("sad::animations::Animations::process");
    }
}

void sad::Renderer::cleanPipeline()
{
    this->pipeline()->clear();
    this->initPipeline();
}

void sad::Renderer::startRendering()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void sad::Renderer::renderScenes()
{
    this->lockRendering();
    this->performQueuedActions();
    this->lockChanges();
    for(size_t i = 0; i < m_scenes.count(); i++)
    {
        sad::Scene * s = m_scenes[i];
        if (s)
        {
            if (s->active())
            {
                s->render();
            }
        }
    }
    this->unlockChanges();
    this->performQueuedActions();
    this->unlockRendering();
}

// Temporarily disabled due to issues with explicitly calling glFinish() on Windows 10 GTX770, since
// sometimes window becomes unresponsible and even crashes on such cases. Removing it does nothing,
// everything still goes normally. 
#define NO_GL_FINISH

void sad::Renderer::finishRendering()
{
#ifndef NO_GL_FINISH 
    glFinish();
#endif
    context()->swapBuffers();
}

void sad::Renderer::addNow(sad::Scene * s)
{
    if (s)
    {
        s->addRef();
        m_scenes << s;
    }
}

void sad::Renderer::removeNow(sad::Scene * s)
{
    if (s)
    {
        s->clearRenderer();
        s->delRef();
    }
    m_scenes.removeAll(s);
}

void sad::Renderer::clearNow()
{
    for(unsigned int i = 0; i < m_scenes.size(); i++)
    {
        m_scenes[i]->clearRenderer();
        m_scenes[i]->delRef();
    }
    m_scenes.clear();
}

void sad::Renderer::insertNow(sad::Scene* s, size_t position)
{
    if (s)
    {
        s->addRef();
        m_scenes.insert(s, position);
    }
}

void sad::Renderer::tryInitShaders()
{
    if (this->m_default_textures_shader_3d == nullptr)
    {
        m_shader_init_mutex.lock();

        m_default_textures_shader_3d = new sad::Shader();
        m_default_textures_shader_3d->addRef();
        m_default_textures_shader_3d->setRenderer(this);
        m_default_textures_shader_3d->setVertexProgram(
            "#version 300 es\n"
            "layout(location = 0) in vec3 position;\n"
            "layout (std140) uniform _SGLCameraInfo\n"
            "{\n"
            "mat4 _sglModelViewMatrix;\n"
            "mat4 _sglProjectionMatrix;\n"
            "};\n"
            "in vec2 vertTexCoord;\n"
            "out vec2 fragTexCoord;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    fragTexCoord = vertTexCoord;\n"
            "    vec4 tmp = vec4(position.x, position.y, position.z, 1.0);\n"
            "    gl_Position = (_sglProjectionMatrix * _sglModelViewMatrix) * tmp;\n"
            "}\n"
        );
        m_default_textures_shader_3d->setFragmentProgram(
            "#version 300 es\n"
            "precision mediump float;"
            "in vec2 fragTexCoord;\n"
            "out vec4 color;\n"
            "uniform sampler2D _defaultTexture;\n"
            "uniform vec4 _gl_Color;"
            "void main()\n"
            "{"
            "    color = texture(_defaultTexture, fragTexCoord) * _gl_Color;\n"
            "}"
        );

        m_default_no_textures_shader_3d = new sad::Shader();
        m_default_no_textures_shader_3d->addRef();
        m_default_no_textures_shader_3d->setRenderer(this);
        m_default_no_textures_shader_3d->setVertexProgram(
            "#version 300 es\n"
            "layout(location = 0) in vec3 position;\n"
            "layout (std140) uniform _SGLCameraInfo\n"
            "{\n"
            "mat4 _sglModelViewMatrix;\n"
            "mat4 _sglProjectionMatrix;\n"
            "};\n"
            "\n"
            "void main()\n"
            "{\n"
            "    vec4 tmp = vec4(position.x, position.y, position.z, 1.0);\n"
            "    gl_Position = (_sglProjectionMatrix *_sglModelViewMatrix)  * tmp;\n"
            "}\n"
        );
        m_default_no_textures_shader_3d->setFragmentProgram(
            "#version 300 es\n"
            "precision mediump float;"
            "uniform vec4 _gl_Color;"
            "out vec4 color;\n"
            "void main()\n"
            "{"
            "    color = _gl_Color;\n"
            "}"
        );


        m_default_textures_shader_2d = new sad::Shader();
        m_default_textures_shader_2d->addRef();
        m_default_textures_shader_2d->setRenderer(this);
        m_default_textures_shader_2d->setVertexProgram(
            "#version 300 es\n"
            "layout(location = 0) in vec2 position;\n"
            "layout (std140) uniform _SGLCameraInfo\n"
            "{\n"
            "mat4 _sglModelViewMatrix;\n"
            "mat4 _sglProjectionMatrix;\n"
            "};\n"
            "in vec2 vertTexCoord;\n"
            "out vec2 fragTexCoord;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    fragTexCoord = vertTexCoord;\n"
            "    vec4 tmp = vec4(position.x, position.y, 0.0, 1.0);\n"
            "    gl_Position = (_sglProjectionMatrix * _sglModelViewMatrix) * tmp;\n"
            "}\n"
        );
        m_default_textures_shader_2d->setFragmentProgram(
            "#version 300 es\n"
            "precision mediump float;"
            "in vec2 fragTexCoord;\n"
            "out vec4 color;\n"
            "uniform sampler2D _defaultTexture;\n"
            "uniform vec4 _gl_Color;"
            "void main()\n"
            "{"
            "    color = texture(_defaultTexture, fragTexCoord) * _gl_Color;\n"
            "}"
        );

        m_default_no_textures_shader_2d = new sad::Shader();
        m_default_no_textures_shader_2d->addRef();
        m_default_no_textures_shader_2d->setRenderer(this);
        m_default_no_textures_shader_2d->setVertexProgram(
            "#version 300 es\n"
            "layout(location = 0) in vec2 position;\n"
            "layout (std140) uniform _SGLCameraInfo\n"
            "{\n"
            "mat4 _sglModelViewMatrix;\n"
            "mat4 _sglProjectionMatrix;\n"
            "};\n"
            "\n"
            "void main()\n"
            "{\n"
            "    vec4 tmp = vec4(position.x, position.y, 0.0, 1.0);\n"
            "    gl_Position = (_sglProjectionMatrix *_sglModelViewMatrix)  * tmp;\n"
            "}\n"
        );
        m_default_no_textures_shader_2d->setFragmentProgram(
            "#version 300 es\n"
            "precision mediump float;"
            "uniform vec4 _gl_Color;"
            "out vec4 color;\n"
            "void main()\n"
            "{"
            "    color = _gl_Color;\n"
            "}"
        );


        m_default_font_shader = new sad::Shader();
        m_default_font_shader->addRef();
        m_default_font_shader->setRenderer(this);
        m_default_font_shader->setVertexProgram(
            "#version 300 es\n"
            "layout(location = 0) in vec2 position;\n"
            "layout(location = 1) in vec2 vertTexCoord;\n"
            "layout (std140) uniform _SGLCameraInfo\n"
            "{\n"
            "mat4 _sglModelViewMatrix;\n"
            "mat4 _sglProjectionMatrix;\n"
            "};\n"
            "out vec2 fragTexCoord;\n"
            "out vec4 fragColorData;\n"
            "uniform vec2 center;\n"
            "uniform float angle;\n"
            "uniform vec4 _gl_Color;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    fragTexCoord = vertTexCoord;\n"
            "    fragColorData = _gl_Color;\n"
            "    float dx = position.x;"
            "    float dy = position.y;"
            "    float x  = center.x + (dx * cos(angle) - dy * sin(angle));"
            "    float y  = center.y + (dx * sin(angle) + dy * cos(angle));"
            "    vec4 tmp = vec4(x, y, 0.0, 1.0);\n"
            "    gl_Position = (_sglProjectionMatrix * _sglModelViewMatrix) * tmp;\n"
            "}\n"
        );
        m_default_font_shader->setFragmentProgram(
            "#version 300 es\n"
            "precision mediump float;"
            "in vec2 fragTexCoord;\n"
            "in vec4 fragColorData;\n"
            "out vec4 color;\n"
            "uniform sampler2D _defaultTexture;\n"
            "void main()\n"
            "{"
            "    color = texture(_defaultTexture, fragTexCoord) * fragColorData;\n"
            "}"
        );


        m_default_font_line_shader = new sad::Shader();
        m_default_font_line_shader->addRef();
        m_default_font_line_shader->setRenderer(this);
        m_default_font_line_shader->setVertexProgram(
            "#version 300 es\n"
            "layout(location = 0) in vec2 position;\n"
            "layout (std140) uniform _SGLCameraInfo\n"
            "{\n"
            "mat4 _sglModelViewMatrix;\n"
            "mat4 _sglProjectionMatrix;\n"
            "};\n"
            "uniform vec2 center;\n"
            "uniform float angle;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    float dx = position.x;\n"
            "    float dy = position.y;\n"
            "    float x  = center.x + (dx * cos(angle) - dy * sin(angle));\n"
            "    float y  = center.y + (dx * sin(angle) + dy * cos(angle));\n"
            "    vec4 tmp = vec4(x, y, 0.0, 1.0);\n"
            "    gl_Position = (_sglProjectionMatrix * _sglModelViewMatrix) * tmp;\n"
            "}\n"
        );
        m_default_font_line_shader->setFragmentProgram(
            "#version 300 es\n"
            "precision mediump float;"
            "uniform vec4 _gl_Color;"
            "out vec4 color;\n"
            "void main()\n"
            "{"
            "    color = _gl_Color;\n"
            "}"
        );

        m_default_texture_shader_function_3d = new sad::ShaderFunction();
        m_default_texture_shader_function_3d->setShader(m_default_textures_shader_3d);
        m_default_texture_shader_function_3d->addRef();

        m_default_no_textures_shader_function_3d = new sad::ShaderFunction();
        m_default_no_textures_shader_function_3d->setShader(m_default_no_textures_shader_3d);
        m_default_no_textures_shader_function_3d->addRef();

        m_default_texture_shader_function_2d = new sad::ShaderFunction();
        m_default_texture_shader_function_2d->setShader(m_default_textures_shader_2d);
        m_default_texture_shader_function_2d->addRef();

        m_default_no_textures_shader_function_2d = new sad::ShaderFunction();
        m_default_no_textures_shader_function_2d->setShader(m_default_no_textures_shader_2d);
        m_default_no_textures_shader_function_2d->addRef();

        m_default_font_shader_function = new sad::FontShaderFunction();
        m_default_font_shader_function->setShader(m_default_font_shader);
        m_default_font_shader_function->addRef();

        m_default_font_line_shader_function = new sad::FontShaderFunction();
        m_default_font_line_shader_function->setShader(m_default_font_line_shader);
        m_default_font_line_shader_function->addRef();

        m_shader_init_mutex.unlock();
    }
}

void sad::Renderer::freeCurrentState()
{
    // Force clearing of scenes, so resource links should be preserved
    for (size_t i = 0; i < m_scenes.size(); i++)
    {
        m_scenes[i]->clear();
    }

    delete m_animations;
    delete m_primitive_renderer;
    delete m_camera_buffer;
    m_cursor->delRef();

    // Force freeing resources, to make sure, that pointer to context will be valid, when resource
    // starting to be freed.
    for (sad::Hash<sad::String, sad::resource::Tree*>::iterator it = m_resource_trees.begin();
        it != m_resource_trees.end();
        ++it)
    {
        it.value()->delRef();
    }
    m_resource_trees.clear();


    del_ref_if_not_nullptr(m_default_texture_shader_function_3d);
    del_ref_if_not_nullptr(m_default_no_textures_shader_function_3d);
    del_ref_if_not_nullptr(m_default_textures_shader_3d);
    del_ref_if_not_nullptr(m_default_no_textures_shader_3d);

    del_ref_if_not_nullptr(m_default_texture_shader_function_2d);
    del_ref_if_not_nullptr(m_default_no_textures_shader_function_2d);
    del_ref_if_not_nullptr(m_default_textures_shader_2d);
    del_ref_if_not_nullptr(m_default_no_textures_shader_2d);

    del_ref_if_not_nullptr(m_default_font_shader_function);
    del_ref_if_not_nullptr(m_default_font_shader);

    del_ref_if_not_nullptr(m_default_font_line_shader);
    del_ref_if_not_nullptr(m_default_font_line_shader_function);


    delete m_pipeline;
    delete m_controls;
    delete m_window;
    delete m_context;
    delete m_opengl;
    delete m_main_loop;
    delete m_fps_interpolation;
    delete m_log;

    for (sad::Hash<sad::String, sad::db::Database*>::iterator it = m_databases.begin();
        it != m_databases.end();
        ++it)
    {
        it.value()->delRef();
    }
    m_databases.clear();
    m_gl_font_geometries.clear();

    sad::util::free_values(m_sizes_to_textured_geometry_3d);
    sad::util::free_values(m_sizes_to_textured_geometry_2d);
    sad::util::free_values(m_sizes_to_untextured_geometry_3d);
    sad::util::free_values(m_sizes_to_untextured_geometry_2d);
    m_sizes_to_textured_geometry_3d.clear();
    m_sizes_to_textured_geometry_2d.clear();
    m_sizes_to_untextured_geometry_3d.clear();
    m_sizes_to_untextured_geometry_2d.clear();
    delete m_gl_sprite_geometry_storages;
}


DECLARE_COMMON_TYPE(sad::Renderer)
