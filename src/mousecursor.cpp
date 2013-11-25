#include "mousecursor.h"
#include "renderer.h"
#include "window.h"
#include "glcontext.h"
#include "os/windowhandles.h"

sad::MouseCursor::MouseCursor()
: m_showcustomcursor(false),
m_cursor(NULL),
m_renderer(NULL)
{

}

sad::MouseCursor::~MouseCursor()
{

}

void sad::MouseCursor::setRenderer(sad::Renderer * r)
{
	m_renderer = r;
}

sad::Renderer *  sad::MouseCursor::renderer() const
{
	return m_renderer;
}

sad::MaybePoint3D  sad::MouseCursor::position() const
{
	sad::MaybePoint3D result;
	if (m_renderer != NULL)
	{
		if (m_renderer->hasValidContext())
		{
			bool successfullyqueried = false;
			sad::Point2D point;
	
#ifdef WIN32
			POINT p;
			GetCursorPos(&p);
			successfullyqueried = (ScreenToClient(
				m_renderer->window()->handles()->WND,
				&p) == TRUE
			);
			point = sad::Point2D(p.x, p.y);
#endif

#ifdef X11
			int x=0, y=0;
			int wx=0, wy=0;
			unsigned int mask=0;
			::Window rootw,childw;
			successfullyqueried = (bool)XQueryPointer(
				m_renderer->window()->handles()->Dpy, 
				m_renderer->window()->handles()->Win, 
				&rootw,
				&childw, 
				&x, 
				&y, 
				&wx, 
				&wy, 
				&mask
			);
			if (successfullyqueried)
			{
				sad::Rect2I rect = m_renderer->window()->rect();
				// Check if x and y is on window
				successfullyqueried = (x >= 0) && (x <= rect.width())
								   && (y >= 0) && (y <= rect.width());
			}
			point = sad::Point2D(x, y);
#endif

			if (successfullyqueried)
			{
				point = m_renderer->window()->toClient(point);
				bool ztest = m_renderer->settings().ztest();
				result.setValue(
					m_renderer->context()->mapToViewport(point, ztest)
				);
			}
		}
	}
	return result;
}


void sad::MouseCursor::setPosition(const sad::Point2D & p)
{
	if (m_renderer != NULL)
	{
		if (m_renderer->hasValidContext())
		{
#ifdef WIN32
			POINT wp;
			wp.x = p.x();
			wp.y = p.y();
			ClientToScreen(m_renderer->window()->handles()->WND, &wp);
			SetCursorPos(wp.x, wp.y);
#endif

#ifdef X11
			XWarpPointer(
				m_renderer->window()->handles()->Dpy, 
				None, 
				m_renderer->window()->handles()->Win, 
				0, 
				0, 
				0, 
				0, 
				(int)(p.x()), 
				(int)(p.y())
			);
#endif
		}
	}
}
