#include "renderer.h"
#include "texturemanager.h"

sad::Point3D sad::Renderer::mousePos()
{
	int x=0, y=0;
	int wx=0, wy=0;
	unsigned int mask=0;
	::Window rootw,childw;
	XQueryPointer(m_window.dpy, m_window.win, &rootw ,&childw, &x, &y, &wx, &wy, &mask);
	
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
	glFinish();
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

