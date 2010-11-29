/*! \file   renderer.cpp
\author FreakyBlast

\brief  Definition of Renderer.

In this file OpenGL function has been used obviously.
*/


#include "renderer.h"
#include "texturemanager.h"
#ifdef WIN32
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#endif


sad::Renderer::~Renderer(void)
{
	if (m_currentscene)
		delete m_currentscene;
}

bool sad::Renderer::init(const sad::Settings& _settings)
{
 m_glsettings.setWidthScreen(_settings.width());
 m_glsettings.setHeightScreen(_settings.height());
 m_glsettings.setIsFullscreen(_settings.isFullscreen());
 m_glsettings.setBPP(_settings.bpp());
 m_glsettings.setDepth(_settings.depth());
 m_glsettings.setFoV(_settings.fov());
 m_glsettings.setZNear(_settings.znear());
 m_glsettings.setZFar(_settings.zfar());
 m_created=createWindow();
 if (!m_created) { hst::log::inst()->owrite(hst::string("Renderer init: can't create window\n"));}
 return true;
}


void sad::Renderer::reshape(int width, int height)
{
 glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));				// Переустанавливаем ViewPort (область видимости)
 glMatrixMode (GL_PROJECTION);										// Выбираем матрицу проекции
 glLoadIdentity ();													// Сбрасываем её на единичную
 gluPerspective (m_glsettings.fov(), 
			    (GLfloat)(width)/(GLfloat)(height),		      	// Calculate The Aspect Ratio Of The Window
			     m_glsettings.znear(), 
				 m_glsettings.zfar());		
 glMatrixMode (GL_MODELVIEW);										// Выбираем видовую матрицу
 glLoadIdentity ();													// Сбрасываем её на единичную
}


sad::Renderer& sad::Renderer::instance()
{
 static Renderer aloneRenderer;
 return aloneRenderer;
}



void sad::Renderer::run()
{
 //If already created
 if (m_created)
 {
	 mainLoop();
	 return;
 }

 if (createWindow())
 {
	    m_created=true;
		hst::log::inst()->owrite(hst::string("Renderer: started rendering cycles\n"));
	    mainLoop();
 }
 else
	 hst::log::inst()->owrite(hst::string("Can't create window\n"));
 
 hst::log::inst()->save("log.txt");
}	


void sad::Renderer::toggleFullscreen()								// Toggle Fullscreen/Windowed
{
  if (m_running)
  {
   this->releaseWindow();
   this->m_window.fullscreen=!this->m_window.fullscreen;
   this->createWindow();
   sad::TextureManager::buildAll();
  }
  else
  {
	  this->m_window.fullscreen=!this->m_window.fullscreen;
  }
}


//Getting a black background with all params
bool sad::Renderer::initGLRendering()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f,0.0f,0.0f,0.0f); //Fill a black background
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
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
	winy=(float)(viewport[3])-(float)(y);

	glReadPixels(x,(int)winy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winz);

	gluUnProject(winx,winy,winz,modelview,projection,viewport,result,result+1,result+2);

	px=(float)(result[0]);
	py=(float)(result[1]);
	pz=(float)(result[2]);
}