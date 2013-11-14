#include "renderer.h"
#include "texturemanager.h"
#include "glcontext.h"
#include "window.h"
#include "os/windowhandles.h"
#include "../../include/log/log.h"
#include "../../include/3rdparty/format/format.h"


#ifdef WIN32

sad::Point3D sad::Renderer::mousePos()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(this->window()->handles()->WND,&p);
	sad::Point2D point = this->window()->toClient(
		sad::Point2D(p.x, p.y)
	);
	return this->context()->mapToViewport(point, m_glsettings.ztest());
}

void sad::Renderer::update()
{
 if (m_setimmediately || m_reset)
 {
	 m_timer.start();
	 m_reset = false;
 }
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 scene()->render();
 context()->swapBuffers();
 ++m_frames;
 m_timer.stop();
 double elapsed = m_timer.elapsed();
 if (m_setimmediately || elapsed > 500.0)
 {
	 double newfps = 1000.0 * m_frames / elapsed; 
	 setFPS( newfps );
	 m_frames = 0;
	 m_reset = true;
	 m_setimmediately = false;
 }
 this->trySwapScenes();
}


#endif
