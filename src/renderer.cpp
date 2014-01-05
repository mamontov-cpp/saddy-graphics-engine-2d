#include "renderer.h"

#include "texturemanager.h"
#include "fontmanager.h"
#include "texturemanager.h"
#include "scene.h"
#include "window.h"
#include "glcontext.h"
#include "mainloop.h"
#include "mousecursor.h"
#include "opengl.h"
#include "fpsinterpolation.h"
#include "input/controls.h"
#include "pipeline/pipeline.h"
#include "os/windowhandles.h"

sad::Renderer * sad::Renderer::m_instance = NULL;

sad::Renderer::Renderer()
: 
m_log(new sad::log::Log()),
m_font_manager(new sad::FontManager()),
m_texture_manager(new sad::TextureManager()),
m_scene(new sad::Scene()),
m_new_scene(NULL),
m_window(new sad::Window()),
m_context(new sad::GLContext()),
m_cursor(new sad::MouseCursor()),
m_opengl(new sad::OpenGL()),
m_main_loop(new sad::MainLoop()),
m_fps_interpolation(new sad::FPSInterpolation()),
m_controls(new sad::input::Controls()),
m_pipeline(new sad::pipeline::Pipeline()),
m_added_system_pipeline_tasks(false)
{
	m_scene->setRenderer(this);
	m_window->setRenderer(this);
	m_cursor->setRenderer(this);
	m_opengl->setRenderer(this);
	m_main_loop->setRenderer(this);
	m_texture_manager->setRenderer(this);
	
	// Add stopping a main loop to quite events of controls to make window close
	// when user closs a window
	m_controls->add(*(sad::input::ET_Quit), m_main_loop, &sad::MainLoop::stop);

	// Init pipeline to make sure, that user can add actions after rendering step, before 
	// renderer started
	this->initPipeline();
}

sad::Renderer::~Renderer(void)
{
	if (m_scene)
		delete m_scene;
	delete m_cursor;
	delete m_font_manager;
	delete m_texture_manager;
	delete m_pipeline;
	delete m_controls;
	delete m_window;
	delete m_context;
	delete m_opengl;
	delete m_main_loop;
	delete m_fps_interpolation; 
	delete m_log;
}

void sad::Renderer::setScene(Scene * scene)
{
	if (this->running())
	{
		m_new_scene = scene;
	} 
	else
	{
		delete m_scene;
		m_scene = scene;
	}
	scene->setRenderer(this);
}

sad::Scene* sad::Renderer::scene() const
{
	return m_scene;
}


void sad::Renderer::init(const sad::Settings& _settings)
{
	SL_INTERNAL_SCOPE("sad::Renderer::init", (*this));
	m_glsettings = _settings;
	m_window->setCreationSize(m_glsettings.width(), m_glsettings.height());
}

bool sad::Renderer::run()
{
	SL_INTERNAL_SCOPE("sad::Renderer::run()", *this);
 

	// Try to create window if needed
	bool success = false;
	if (m_window->valid() == false)
	{
		success = m_window->create();
		if (!success)
		{
			SL_LOCAL_FATAL("Cannot create window\n",*this);
		}
	} 
	else
	{
		success = true;
	}

	// Try to create context if needed
	if (m_context->valid() == false && success)
	{
		success =  m_context->createFor(m_window);
		if (!success)
		{
			SL_LOCAL_FATAL("Failed to create OpenGL context\n",*this);
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
		initPipeline();
		//cursor()->insertHandlersIfNeeded();
		mainLoop()->run();
		//cursor()->removeHandlersIfNeeded();
		cleanPipeline();
		m_context->destroy();
		m_window->destroy();
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

sad::Renderer* sad::Renderer::ref()
{
	if (sad::Renderer::m_instance == NULL)
	{
		sad::Renderer::m_instance = new sad::Renderer();
		atexit(sad::Renderer::destroyInstance);
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


sad::FontManager * sad::Renderer::fonts()
{
	return m_font_manager;
}

sad::TextureManager * sad::Renderer::textures()
{
	return m_texture_manager;
}

sad::log::Log * sad::Renderer::log()
{
	return m_log;
}

sad::Window * sad::Renderer::window()
{
	return m_window;
}

sad::GLContext * sad::Renderer::context()
{
	return m_context;
}

const sad::Settings & sad::Renderer::settings() const
{
	return m_glsettings;
}

sad::MouseCursor* sad::Renderer::cursor() const
{
	return m_cursor;
}

void sad::Renderer::setCursor(sad::MouseCursor * cursor)
{
	delete m_cursor;
	m_cursor = cursor;
}

sad::OpenGL * sad::Renderer::opengl() const
{
	return m_opengl;
}

sad::MainLoop * sad::Renderer::mainLoop() const
{
	return m_main_loop;
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

void sad::Renderer::emergencyShutdown()
{
	// Unload all textures, because after shutdown context will be lost
	// and glDeleteTextures could lead to segfault
	this->textures()->unload();
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
		result = this->context()->mapToViewport(windowpoint, m_glsettings.ztest());
	}
	return result;
}

void sad::Renderer::destroyInstance()
{
	delete  sad::Renderer::m_instance;
}


void sad::Renderer::reshape(int width, int height)
{
	SL_INTERNAL_SCOPE(fmt::Format("sad::Renderer::reshape({0}, {1})") << width << height, *this);

	if (width == 0) {
		width = 1;
	}
	SL_LOCAL_INTERNAL(
		fmt::Format("Before glViewport error code {0}")
		<< glGetError(), 
		*this
	);
	// Reset viewport for window
	glViewport (0, 0, width, height);
	SL_LOCAL_INTERNAL(
		fmt::Format("glViewport error code {0}")
		<< glGetError(), 
		*this
	);
	
	// Clear projection matrix
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
  
	//  Set perspective projection
	GLfloat aspectratio = (GLfloat)(width)/(GLfloat)(height);
	gluPerspective(
		m_glsettings.fov(), 
		aspectratio,
		m_glsettings.znear(), 
		m_glsettings.zfar()
	);		
	
	// Clear modelview matrix
	glMatrixMode (GL_MODELVIEW);										
	glLoadIdentity ();	
}

#ifndef GL_GENERATE_MIPMAP_HINT
	#define GL_GENERATE_MIPMAP_HINT           0x8192
#endif


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
	
	const char * version=(const char *)glGetString(GL_VERSION);
	if (version!=NULL)
	{
		SL_LOCAL_INTERNAL(sad::String("running OpenGL ")+sad::String(version), *this);
		if (version[0]>'1' || version[2] >= '4')
			glHint(GL_GENERATE_MIPMAP_HINT,GL_NICEST);
	}

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	
	reshape(m_glsettings.width(),m_glsettings.height());
	
	glFinish();
	return true;
}



void sad::Renderer::trySwapScenes()
{
	if (m_new_scene)
	{
		delete m_scene;
		m_scene = m_new_scene;
		m_scene->setRenderer(this);
	}
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
		this->pipeline()
			->systemAppendProcess(this, &sad::Renderer::trySwapScenes)
			->mark("sad::Renderer::trySwapScenes");
		m_added_system_pipeline_tasks =  true;
	}
	//We should append rendering task to pipeline to make scene renderable
	if (this->pipeline()->contains("sad::Scene::render") == false)
	{
		this->pipeline()->appendProcess(this, &sad::Renderer::scene, &sad::Scene::render)->mark("sad::Scene::render");
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

void sad::Renderer::finishRendering()
{
	glFinish();
	context()->swapBuffers();
}

