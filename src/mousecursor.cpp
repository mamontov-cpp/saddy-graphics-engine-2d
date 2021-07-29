#include "mousecursor.h"
#include "renderer.h"
#include "window.h"
#include "glcontext.h"
#include "sprite2d.h"
#include "camera.h"
#include "os/windowhandles.h"
#include "os/ubo.h"
#include "input/controls.h"

#include "db/dbtypename.h"

sad::MouseCursorImage::~MouseCursorImage()
{

}

sad::MouseCursorSprite::MouseCursorSprite(sad::Sprite2D * a): m_a(a)
{
    m_a->addRef();
}

void sad::MouseCursorSprite::setPos(const sad::Point2D & p)
{
    m_a->moveTo(p);
}

void sad::MouseCursorSprite::render(sad::Renderer* r)
{
    if (r)
    {
        if (!r->scenes().empty())
        {
            sad::Scene* s = r->scenes()[0];
            if (m_a->scene() != s)
            {
                m_a->setScene(s);
            }
            if (r->context()->isOpenGL3compatible() && s)
            {
                if (r->cameraObjectBuffer()->userData() != s->getCamera())
                {
                    s->getCamera()->apply();
                }
            }
            m_a->render();
        }
    }
}

void sad::MouseCursorSprite::setShaderFunction(sad::ShaderFunction* shader_function)
{
    m_a->setShaderFunction(shader_function);
}

sad::ShaderFunction* sad::MouseCursorSprite::shaderFunction() const
{
    return m_a->shaderFunction();
}

sad::MouseCursorSprite::~MouseCursorSprite()
{
    if (m_a)
    {
        m_a->delRef();
    }
}

sad::MouseCursor::MouseCursor()
: m_use_custom_cursor(false),
m_hide_custom_cursor(false),
m_hidden(false),
m_cursor(nullptr),
m_renderer(nullptr),
m_enter_handler(nullptr),
m_leave_handler(nullptr),
m_move_handler(nullptr)
{

}

sad::MouseCursor::~MouseCursor()
{	
    delete m_cursor;
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
    if (m_renderer != nullptr)
    {
        if (m_renderer->hasValidContext())
        {
            // ReSharper disable once CppInitializedValueIsAlwaysRewritten
            bool successfullyqueried = false;
            sad::Point2D point;
    
#ifdef WIN32
            POINT p;
            GetCursorPos(&p);
            successfullyqueried = (ScreenToClient(
                m_renderer->window()->handles()->WND,
                &p) != FALSE
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
    if (m_renderer != nullptr)
    {
        if (m_renderer->hasValidContext())
        {
#ifdef WIN32
            POINT wp;
            wp.x = (LONG)p.x();
            wp.y = (LONG)p.y();
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
    if (m_use_custom_cursor)
    {
        m_hide_custom_cursor = false;
    }
    else
    {
        showDefaultCursor();
    }
}

void sad::MouseCursor::hide()
{
    if (m_use_custom_cursor)
    {
        m_hide_custom_cursor = true;
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
    if (image == nullptr)
        return;

    delete m_cursor;
    m_cursor = image;
    insertHandlersIfNeeded();
    if (m_use_custom_cursor == false)
    {
        if (this->position().exists())
            hideDefaultCursor();
    } 
    m_use_custom_cursor = true;
}

void sad::MouseCursor::setImage(Sprite2D * a)
{
    setImage(new sad::MouseCursorSprite(a));
}


void sad::MouseCursor::clearCursorImage()
{
    delete m_cursor;
    m_cursor = nullptr;
    removeHandlersIfNeeded();
    if (m_use_custom_cursor)
    {
        showDefaultCursor();
        m_hidden = false;
    }
    m_use_custom_cursor = false;
}

void sad::MouseCursor::insertHandlersIfNeeded()
{
    if (m_use_custom_cursor)
    {
        m_enter_handler = m_renderer->controls()->add(
            *sad::input::EventType::ET_MouseEnter, 
            this, &sad::MouseCursor::showCustomCursorIfNeedTo
        );
        m_leave_handler = m_renderer->controls()->add(
            *sad::input::EventType::ET_MouseLeave, 
            this, &sad::MouseCursor::hideCustomCursorIfNeedTo
        );
        m_move_handler = m_renderer->controls()->add(
            *sad::input::EventType::ET_MouseMove,  
            this, &sad::MouseCursor::moveCustomCursor
        );
    }
}


void sad::MouseCursor::removeHandlersIfNeeded()
{
    if (m_use_custom_cursor)
    {
        m_renderer->controls()->remove(m_enter_handler);
        m_renderer->controls()->remove(m_leave_handler);
        m_renderer->controls()->remove(m_move_handler);

        m_enter_handler = nullptr;
        m_leave_handler = nullptr;
        m_move_handler  = nullptr;
    } 
}

void sad::MouseCursor::applyCursorTransformations()
{
    
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

    blank = XCreateBitmapFromData (
        m_renderer->window()->handles()->Dpy, 
        m_renderer->window()->handles()->Win, 
        data, 
        1, 
        1
    );
    cursor = XCreatePixmapCursor(
        m_renderer->window()->handles()->Dpy,
        blank, 
        blank, 
        &dummy, 
        &dummy, 
        0, 
        0
    );
    XFreePixmap (m_renderer->window()->handles()->Dpy, blank);
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
    if (m_use_custom_cursor && m_hidden && !m_hide_custom_cursor && m_cursor != nullptr)
    {
        this->applyCursorTransformations();
        m_cursor->render(m_renderer);
    }
}

void sad::MouseCursor::showCustomCursorIfNeedTo(const sad::input::MouseEnterEvent & e)
{
    if (m_use_custom_cursor && m_cursor != nullptr)
    {
        hideDefaultCursor();
        m_cursor->setPos(e.Point3D);
    }
}


void sad::MouseCursor::hideCustomCursorIfNeedTo()
{
    if (m_use_custom_cursor && m_cursor != nullptr)
    {
        showDefaultCursor();
    }	
}

void sad::MouseCursor::moveCustomCursor(const sad::input::MouseMoveEvent & e)
{
    if (m_use_custom_cursor && m_cursor != nullptr)
    {
        m_cursor->setPos(e.Point3D);
    }	
}

DECLARE_COMMON_TYPE(sad::MouseCursor)

