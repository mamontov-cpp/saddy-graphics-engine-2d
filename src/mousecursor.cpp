#include "mousecursor.h"
#include "renderer.h"
#include "window.h"
#include "glcontext.h"
#include "sprite2dadapter.h"
#include "os/windowhandles.h"
#include "input/controls.h"

sad::MouseCursorImage::~MouseCursorImage()
{
	
}

sad::MouseCursorSprite::MouseCursorSprite(Sprite2DAdapter * a): m_a(a)
{
	
}

void sad::MouseCursorSprite::setPos(const sad::Point2D & p)
{
	m_a->setPos(p);
}

void sad::MouseCursorSprite::render()
{
	m_a->render();
}

sad::MouseCursorSprite::~MouseCursorSprite()
{
	delete m_a;
}

sad::MouseCursor::MouseCursor()
: m_usecustomcursor(false),
m_hidden(false),
m_hidecustomcursor(false),
m_cursor(NULL),
m_renderer(NULL),
m_enter_handler(NULL),
m_leave_handler(NULL),
m_move_handler(NULL)
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

void sad::MouseCursor::show()
{
	if (m_usecustomcursor)
	{
		m_hidecustomcursor = false;
	}
	else
	{
		showDefaultCursor();
	}
}

void sad::MouseCursor::hide()
{
	if (m_usecustomcursor)
	{
		m_hidecustomcursor = true;
	}
	else
	{
		hideDefaultCursor();
	}
}

sad::MouseCursorImage * sad::MouseCursor::cursorImage() const
{
	return m_cursor;
}


void sad::MouseCursor::setImage(sad::MouseCursorImage * image)
{
	if (image == NULL)
		return;

	delete m_cursor;
	m_cursor = image;
	insertHandlersIfNeeded();
	if (m_usecustomcursor == false)
	{
		if (this->position().exists())
			hideDefaultCursor();
		// Force notification, that cursor is hidden
		m_hidden = true;
	} 
	m_usecustomcursor = true;
}

void sad::MouseCursor::setImage(Sprite2DAdapter * a)
{
	setImage(new sad::MouseCursorSprite(a));	
}


void sad::MouseCursor::clearCursorImage()
{
	delete m_cursor;
	m_cursor = NULL;
	if (m_usecustomcursor)
	{
		showDefaultCursor();
		m_hidden = false;
	}
	m_usecustomcursor = false;
}

void sad::MouseCursor::insertHandlersIfNeeded()
{
	if (m_usecustomcursor)
	{
		m_enter_handler = m_renderer->controls()->add(
			*sad::input::ET_MouseEnter, 
			this, &sad::MouseCursor::showCustomCursorIfNeedTo
		);
		m_leave_handler = m_renderer->controls()->add(
			*sad::input::ET_MouseLeave, 
			this, &sad::MouseCursor::hideCustomCursorIfNeedTo
		);
		m_move_handler = m_renderer->controls()->add(
			*sad::input::ET_MouseMove,  
			this, &sad::MouseCursor::moveCustomCursor
		);
	}
}


void sad::MouseCursor::removeHandlersIfNeeded()
{
	if (m_usecustomcursor)
	{
		m_renderer->controls()->remove(m_enter_handler);
		m_renderer->controls()->remove(m_leave_handler);
		m_renderer->controls()->remove(m_move_handler);

		m_enter_handler = NULL;
		m_leave_handler = NULL;
		m_move_handler  = NULL;
	} 
}

void sad::MouseCursor::showDefaultCursor()
{
	if (m_hidden)
	{
		m_hidden = false;

#ifdef WIN32
		ShowCursor(TRUE);
#endif

#ifdef X11
		XUndefineCursor(
			m_renderer->window()->handles()->Dpy, 
			m_renderer->window()->handles()->Win 
		);
#endif
	}
}

void sad::MouseCursor::hideDefaultCursor()
{
	if (!m_hidden)
	{
		m_hidden = true;

#ifdef WIN32
		ShowCursor(FALSE);
#endif

#ifdef X11
	// Fairly copypasted from
	// http://www.gamedev.net/topic/285005-anyone-knows-how-to-hideshow-mouse-pointer-under-linux-using-opengl/
	Pixmap blank;
	XColor dummy;
	char data[1] = {0};
	Cursor cursor;

	blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
	cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
	XFreePixmap (dpy, blank);
	XDefineCursor(
		m_renderer->window()->handles()->Dpy, 
		m_renderer->window()->handles()->Win, 
		cursor
	);
#endif	
	}
}


void sad::MouseCursor::renderCursorIfNeedTo()
{
	if (m_usecustomcursor && m_hidden && !m_hidecustomcursor && m_cursor != NULL)
	{
		m_cursor->render();
	}
}

void sad::MouseCursor::showCustomCursorIfNeedTo(const sad::input::MouseEnterEvent & e)
{
	if (m_usecustomcursor && m_cursor != NULL)
	{
		hideDefaultCursor();
		m_cursor->setPos(e.Point3D);
	}
}


void sad::MouseCursor::hideCustomCursorIfNeedTo()
{
	if (m_usecustomcursor && m_cursor != NULL)
	{
		showDefaultCursor();
	}	
}

void sad::MouseCursor::moveCustomCursor(const sad::input::MouseMoveEvent & e)
{
	if (m_usecustomcursor && m_cursor != NULL)
	{
		m_cursor->setPos(e.Point3D);
	}	
}

