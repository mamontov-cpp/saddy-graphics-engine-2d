#include "qt/sadqtmousecursor.h"
#include "qt/sadqtrenderer.h"
#include "qt/sadqtopenglwidget.h"

#include <QCursor>

#include <camera.h>

sad::qt::MouseCursor::MouseCursor()
{
    
}


sad::qt::MouseCursor::~MouseCursor()
{

}

sad::MaybePoint3D  sad::qt::MouseCursor::position() const
{
    sad::MaybePoint3D result;
    if (m_renderer)
    {
        sad::qt::Renderer* r = static_cast<sad::qt::Renderer*>(m_renderer);
        sad::qt::OpenGLWidget* w = r->widget();
        if (w)
        {
            if (w->underMouse())
            {
                QPoint p = QCursor::pos();
                p = w->mapFromGlobal(p);
                result.setValue(w->toViewport(p));
                return result;
            }
        }
    }
    return sad::MaybePoint3D();
}

void sad::qt::MouseCursor::setPosition(const sad::Point2D & p)
{
    if (m_renderer)
    {
        
        sad::qt::Renderer* r = static_cast<sad::qt::Renderer*>(m_renderer);
        sad::qt::OpenGLWidget* w = r->widget();
        QPoint  pnt(p.x(), w->height() - p.y());
        QCursor::setPos(w->mapToGlobal(pnt));
    }
}


void sad::qt::MouseCursor::showDefaultCursor()
{
    if (m_renderer)
    {
        sad::qt::Renderer* r = static_cast<sad::qt::Renderer*>(m_renderer);
        if (r->widget())
        {
            r->widget()->setCursor(Qt::ArrowCursor);
        }
    }
    m_hidden = false;
}

void sad::qt::MouseCursor::hideDefaultCursor()
{
    if (m_renderer)
    {
        sad::qt::Renderer* r = static_cast<sad::qt::Renderer*>(m_renderer);
        if (r->widget())
        {
            r->widget()->setCursor(Qt::BlankCursor);
        }
    }
    m_hidden = true;
}

