/*! \file   renderer.cpp
\author FreakyBlast

\brief  Definition of Renderer.

In this file OpenGL function has been used obviously.
*/
#include "renderer.h"
#include "texturemanager.h"
#include "input.h"
#include "fontmanager.h"
#include "texturemanager.h"
#include "scene.h"
#ifdef WIN32
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#endif

#ifndef GL_GENERATE_MIPMAP_HINT
	#define GL_GENERATE_MIPMAP_HINT           0x8192
#endif





sad::Renderer::~Renderer(void)
{
	if (m_currentscene)
		delete m_currentscene;
	delete m_input_manager;
	delete m_font_manager;
	delete m_texture_manager;
}

bool sad::Renderer::init(const sad::Settings& _settings)
{
 SL_LOCAL_SCOPE("sad::Renderer::init", (*this));
 m_glsettings.setWidthScreen(_settings.width());
 m_glsettings.setHeightScreen(_settings.height());
 m_glsettings.setIsFullscreen(_settings.isFullscreen());
 m_glsettings.setBPP(_settings.bpp());
 m_glsettings.setDepth(_settings.depth());
 m_glsettings.setFoV(_settings.fov());
 m_glsettings.setZNear(_settings.znear());
 m_glsettings.setZFar(_settings.zfar());
 m_glsettings.setZTest(_settings.ztest());
 m_glsettings.setZTestValue(_settings.ztestvalue());
 m_window.fullscreen = m_glsettings.isFullscreen();
 m_created=createWindow();
 m_window.width=_settings.width();
 m_window.height=_settings.height();
 if (!m_created) { SL_LOCAL_FATAL("Renderer init: can't create window\n",*this);}
 return true;
}


void sad::Renderer::reshape(int width, int height)
{
  SL_SCOPE(fmt::Format("sad::Renderer::reshape({0}, {1})") << width << height);

  if (width==0) width=1;
  
  glViewport (0, 0, width, height);				// Переустанавливаем ViewPort (область видимости)
  glMatrixMode (GL_PROJECTION);										// Выбираем матрицу проекции
  glLoadIdentity ();													// Сбрасываем её на единичную
  gluPerspective (m_glsettings.fov(), 
			    (GLfloat)(width)/(GLfloat)(height),		      	// Calculate The Aspect Ratio Of The Window
			     m_glsettings.znear(), 
		             m_glsettings.zfar());		
  glMatrixMode (GL_MODELVIEW);										// Выбираем видовую матрицу
  glLoadIdentity ();													// Сбрасываем её на единичную
  this->controls()->  postResize(sad::ResizeEvent( 
								 m_window.height,m_window.width,height,width
	                             ));
  m_window.width=width;
  m_window.height=height;
}

sad::Renderer * sad::Renderer::m_instance = NULL;

void sad::Renderer::destroyInstance()
{
	delete  sad::Renderer::m_instance;
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



void sad::Renderer::run()
{
 SL_LOCAL_SCOPE("sad::Renderer::run()", *this);
 if (m_currentscene)
 {
	 this->m_currentscene->setRenderer(this);
 }
 //If already created
 if (m_created)
 {
	 mainLoop();
	 return;
 }

 if (createWindow())
 {
	    m_created=true;
		SL_LOCAL_MESSAGE("Started rendering", *this);
	    mainLoop();
 }
 else
	 SL_LOCAL_FATAL("Can't create window", *this);
 
}	





//Getting a black background with all params
bool sad::Renderer::initGLRendering()
{
    SL_LOCAL_SCOPE("sad::Renderer::initGLRendering()", *this);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f,0.0f,0.0f,0.0f); //Fill a black background
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);

	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	
	const char * version=(const char *)glGetString(GL_VERSION);
	if (version!=NULL)
	{
		SL_LOCAL_MESSAGE(sad::String("running OpenGL ")+sad::String(version), *this);
		if (version[0]>'1' || version[2] >='4')
			glHint(GL_GENERATE_MIPMAP_HINT,GL_NICEST);
	}

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	
	reshape(m_glsettings.width(),m_glsettings.height());
	
	glFlush();
	return true;
}


void sad::Renderer::mapToOGL(int x,int y,float & px,float & py,float & pz)
{
	GLint     viewport[4];
	GLdouble  modelview[16];
	GLdouble  projection[16];

	GLfloat winx=0,winy=0,winz=0;
	GLdouble result[3];
	
	glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
    glGetDoublev(GL_PROJECTION_MATRIX,projection);
	glGetIntegerv(GL_VIEWPORT,viewport);

	winx=(float)x;
#ifdef WIN32  // On win32 we explicitly handle coordinates
	winy=(float)(y);
#else
	winy=(float)(viewport[3] - y);
#endif
	if (this->m_glsettings.ztest())
	    glReadPixels(x,(int)winy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winz);
	else
		winz=this->m_glsettings.ztestvalue();


	gluUnProject(winx,winy,winz,modelview,projection,viewport,result,result+1,result+2);
	
	px=(float)(result[0]);
	py=(float)(result[1]);
	pz=(float)(result[2]);
}

sad::FontManager * sad::Renderer::fonts()
{
	return m_font_manager;
}

sad::TextureManager * sad::Renderer::textures()
{
	return m_texture_manager;
}

sad::Renderer::Renderer()
{
	m_windowtitle="SadExample";
	m_input_manager = new sad::Input();
	m_created=false;
    m_running = false;
	m_font_manager = new sad::FontManager();
	m_texture_manager = new sad::TextureManager();
	m_fps = 75;
	m_chscene = 0;
	m_currentscene = new sad::Scene();
	m_currentscene->setRenderer(this);
	initWindowParameters();
}

sad::log::Log * sad::Renderer::log()
{
	return &m_log;
}

sad::Input  * sad::Renderer::controls()
{
	return m_input_manager;
}

void sad::Renderer::setCurrentScene(sad::Scene* scene) 
{
	scene->setRenderer(this);
	if (m_currentscene) 
		delete m_currentscene;
	m_currentscene=scene; 
}

void sad::Renderer::pushScene(Scene * scene)
{
	m_chscene=scene;
	scene->setRenderer(this);
}


void sad::Renderer::setFPS(double fps) 
{
	// Reject sudden raises or drops for frameate
	if (fps > m_fps * 7)
		return;
	m_fps =  fps * 0.6 + m_fps * 0.4;
}