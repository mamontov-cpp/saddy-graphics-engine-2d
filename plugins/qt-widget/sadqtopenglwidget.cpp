#include "qt/sadqtopenglwidget.h"
#include "qt/sadqtkeytosadkey.h"

#include <mousecursor.h>
#include <scene.h>
#include <fpsinterpolation.h>

#include <input/events.h>

#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>

sad::qt::OpenGLWidget::OpenGLWidget(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f), m_window(nullptr), m_first(true), m_reshaped(false), m_mtx(QMutex::Recursive)
{
    QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
    fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
    fmt.setRenderableType(QSurfaceFormat::OpenGL);
    fmt.setSwapInterval(1);
    fmt.setRedBufferSize(8);
    fmt.setGreenBufferSize(8);
    fmt.setBlueBufferSize(8);
    fmt.setAlphaBufferSize(8);
    fmt.setStencilBufferSize(8);
    fmt.setDepthBufferSize(24);
    fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);

    this->setFormat(fmt);
    this->setUpdateBehavior(QOpenGLWidget::NoPartialUpdate);
    
    connect(QApplication::instance(), SIGNAL(lastWindowClosed()), this, SLOT(applicationQuit()));
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(this, SIGNAL(rendererChanged(void*)), this, SLOT(immediateSetRenderer(void*)));
    m_timer.setInterval(0);
    m_timer.start();

    m_renderer = new sad::qt::Renderer();
    m_renderer->setWidget(this);

    this->setMouseTracking(true);
    this->setFocusPolicy(Qt::StrongFocus);
}

sad::qt::OpenGLWidget::~OpenGLWidget()
{
    if (m_renderer)
    {
        if (m_renderer->initialized())
        {
            m_renderer->deinitRendererAfterLoop();
        }
        delete m_renderer;
    }
}


void sad::qt::OpenGLWidget::setRenderer(sad::qt::Renderer* renderer)
{
    emit rendererChanged(renderer);
}

void sad::qt::OpenGLWidget::immediateSetRenderer(void* r)
{
    sad::qt::Renderer* renderer = static_cast<sad::qt::Renderer*>(r);
    m_mtx.lock();
    if (renderer)
    {
        if (renderer != m_renderer)
        {
            m_first = true;
            m_reshaped = false;
            if (m_renderer)
            {
                if (m_renderer->initialized())
                {
                    //m_renderer->submitEvent(new sad::input::QuitEvent(), true);
                }
            }
            delete m_renderer;
            m_renderer = renderer;
            m_renderer->setWidget(this);
        }
    }
    m_mtx.unlock();
}

sad::qt::Renderer* sad::qt::OpenGLWidget::renderer() const
{
    return m_renderer;
}

void sad::qt::OpenGLWidget::resizeGL(int width, int height)
{
    m_mtx.lock();
    if (width == 0)
    {
        width = 1;
    }
    if (m_first)
    {
        if (m_renderer->initialized())
        {
            m_renderer->initRendererBeforeLoop();
            if (this->isActiveWindow())
            {
                m_renderer->submitEvent(new sad::input::ActivateEvent());
            }
            m_first = false;
        }
    }
    if (m_renderer->initialized())
    {
        m_renderer->reshape(width, height);
        sad::input::ResizeEvent* ev = new sad::input::ResizeEvent();
        ev->OldSize = m_old_size;
        ev->NewSize = sad::Size2I(width, height);
        m_renderer->submitEvent(ev);
        m_reshaped = true;
    }
    this->update();
    m_old_size = sad::Size2I(width, height);
    m_mtx.unlock();
}

void sad::qt::OpenGLWidget::paintGL()
{
    if (this->window() != m_window)
    {
        if (m_window)
        {
            m_window->removeEventFilter(this);
        }
        m_window = this->window();
        m_window->installEventFilter(this);
    }
    m_mtx.lock();
    if (m_renderer)
    {
        if (m_reshaped == false && m_renderer->initialized())
        {
            this->resizeGL(this->width(), this->height());
        }
    }
    
    if (m_renderer)
    {
        if (m_renderer->initialized())
        {			
            m_renderer->runOnce();
        }
    }
    m_mtx.unlock();
}

void sad::qt::OpenGLWidget::enterEvent(QEvent* ev)
{
    m_mtx.lock();
    if (this->m_renderer)
    {
        if (m_renderer->initialized())
        {
            QPoint p = QCursor::pos();
            p = this->mapFromGlobal(p);
            sad::input::MouseEnterEvent* sev = new sad::input::MouseEnterEvent();
            sev->Point3D = this->toViewport(p);
            m_renderer->submitEvent(sev);
        }
    }
    m_mtx.unlock();
    this->QOpenGLWidget::enterEvent(ev);
}

void sad::qt::OpenGLWidget::wheelEvent(QWheelEvent* ev)
{
    m_mtx.lock();
    if (this->m_renderer)
    {
        if (m_renderer->initialized())
        {
            sad::input::MouseWheelEvent* sev = new sad::input::MouseWheelEvent();
            sev->Point3D = this->toViewport(ev->pos());
            sev->Delta = ev->delta();
            m_renderer->submitEvent(sev);
        }
    }
    m_mtx.unlock();
    this->QOpenGLWidget::wheelEvent(ev);
}

void sad::qt::OpenGLWidget::mouseDoubleClickEvent(QMouseEvent* ev)
{
    m_mtx.lock();
    if (this->m_renderer)
    {
        if (m_renderer->initialized())
        {
            sad::input::MouseDoubleClickEvent* sev = new sad::input::MouseDoubleClickEvent();
            this->mouseEventToSadMouseEvent(ev, sev);
            m_renderer->submitEvent(sev);
        }
    }
    m_mtx.unlock();
    this->QOpenGLWidget::mouseDoubleClickEvent(ev);
}

void sad::qt::OpenGLWidget::mousePressEvent(QMouseEvent* ev)
{
    m_mtx.lock();
    if (this->m_renderer)
    {
        if (m_renderer->initialized())
        {
            sad::input::MousePressEvent* sev = new sad::input::MousePressEvent();
            this->mouseEventToSadMouseEvent(ev, sev);
            m_renderer->submitEvent(sev);
        }
    }
    m_mtx.unlock();
    this->QOpenGLWidget::mousePressEvent(ev);
}


void sad::qt::OpenGLWidget::mouseMoveEvent(QMouseEvent* ev)
{
    m_mtx.lock();
    if (this->m_renderer)
    {
        if (m_renderer->initialized())
        {
            sad::input::MouseMoveEvent* sev = new sad::input::MouseMoveEvent();
            sev->Point3D = this->toViewport(ev->pos());
            m_renderer->submitEvent(sev);
        }
    }
    m_mtx.unlock();
    this->QOpenGLWidget::mouseMoveEvent(ev);
}

void sad::qt::OpenGLWidget::mouseReleaseEvent(QMouseEvent* ev)
{
    m_mtx.lock();
    if (this->m_renderer)
    {
        if (m_renderer->initialized())
        {
            sad::input::MouseReleaseEvent* sev = new sad::input::MouseReleaseEvent();
            this->mouseEventToSadMouseEvent(ev, sev);
            m_renderer->submitEvent(sev);
        }
    }
    m_mtx.unlock();
    this->QOpenGLWidget::mouseReleaseEvent(ev);
}


void sad::qt::OpenGLWidget::leaveEvent(QEvent* ev)
{
    m_mtx.lock();
    if (m_renderer)
    {
        if (m_renderer->initialized())
        {
            m_renderer->submitEvent(new sad::input::MouseLeaveEvent());
        }
    }
    m_mtx.unlock();
    this->QOpenGLWidget::leaveEvent(ev);
}

void sad::qt::OpenGLWidget::keyPressEvent(QKeyEvent* ev)
{
    m_mtx.lock();
    if (m_renderer)
    {
        if (m_renderer->initialized())
        {
            sad::input::KeyPressEvent* sev = new sad::input::KeyPressEvent();
            sad::qt::qtKeyEventToSadKeyEvent(ev, sev);
            m_renderer->submitEvent(sev);
        }
    }
    m_mtx.unlock();
    this->QOpenGLWidget::keyPressEvent(ev);
}

void sad::qt::OpenGLWidget::keyReleaseEvent(QKeyEvent* ev)
{
    m_mtx.lock();
    if (m_renderer)
    {
        if (m_renderer->initialized())
        {
            sad::input::KeyReleaseEvent* sev = new sad::input::KeyReleaseEvent();
            sad::qt::qtKeyEventToSadKeyEvent(ev, sev);
            m_renderer->submitEvent(sev);
        }
    }
    m_mtx.unlock();
    this->QOpenGLWidget::keyReleaseEvent(ev);
}

bool sad::qt::OpenGLWidget::event(QEvent* e)
{
    this->tryHandleActivateEvent(e);
    this->tryHandleMinimization(e);
    return this->QOpenGLWidget::event(e);
}

bool sad::qt::OpenGLWidget::eventFilter(QObject* obj, QEvent* ev)
{
    if (obj != this)
    {
        this->tryHandleActivateEvent(ev);
        this->tryHandleMinimization(ev);
    }
    return QObject::eventFilter(obj, ev);
}

// ======================= PUBLIC SLOT METHODS =======================

void sad::qt::OpenGLWidget::applicationQuit()
{
    m_mtx.lock();
    if (m_renderer)
    {
        if (m_renderer->initialized())
        {
            m_renderer->submitEvent(new sad::input::QuitEvent(), true);
        }
    }
    m_mtx.unlock();
}

void sad::qt::OpenGLWidget::goFullscreen()
{
    QWidget* window = this->window();
    m_old_max_size = window->maximumSize();
    window->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

    m_old_geometry = this->geometry();
    this->raise();
    window->window()->showFullScreen();

#ifdef LINUX
    // Give a Window Manager time to respond to fullscreen call.
    QTimer::singleShot(500, this, SLOT(fitWidgetToWindow()));
#else
    this->fitWidgetToWindow();
#endif
}

void sad::qt::OpenGLWidget::goWindowed()
{
    QWidget* window = this->window();
    window->window()->showNormal();
    window->setMaximumSize(m_old_max_size);
    this->setGeometry(m_old_geometry);
}

void sad::qt::OpenGLWidget::fitWidgetToWindow()
{
    QWidget* window = this->window();
    this->setGeometry(0, 0, window->width(), window->height());
}

// ======================= PROTECTED METHODS =======================

void sad::qt::OpenGLWidget::tryHandleActivateEvent(QEvent* ev) const
{
    if (ev->type() == QEvent::ActivationChange)
    {
        const_cast<sad::qt::OpenGLWidget*>(this)->m_mtx.lock();
        if (m_renderer)
        {
            if (m_renderer->initialized())
            {
                if (this->isActiveWindow())
                {
                    m_renderer->submitEvent(new sad::input::ActivateEvent());
                }
                else
                {
                    m_renderer->submitEvent(new sad::input::DeactivateEvent());
                }
            }
        }
        const_cast<sad::qt::OpenGLWidget*>(this)->m_mtx.unlock();
    }
}

void sad::qt::OpenGLWidget::tryHandleMinimization(QEvent* ev) const
{
    if (ev->type() == QEvent::WindowStateChange)
    {
        if (this->window()->isMinimized() && (this->m_renderer != nullptr))
        {
            const_cast<sad::qt::OpenGLWidget*>(this)->m_mtx.lock();
            if (m_renderer->initialized())
            {
                this->m_renderer->fpsInterpolation()->reset();
            }
            const_cast<sad::qt::OpenGLWidget*>(this)->m_mtx.unlock();
        }
    }
}

sad::Point3D sad::qt::OpenGLWidget::toViewport(const QPoint& p) const
{
    double y = this->height() - p.y();
    sad::Point3D mx = m_renderer->mapToViewport(sad::Point2D(p.x(), y));
    return mx;
}

void sad::qt::OpenGLWidget::mouseEventToSadMouseEvent(QMouseEvent* ev, sad::input::MouseEvent* sev) const
{
    sev->Point3D = this->toViewport(ev->pos());
    switch(ev->button())
    {
	case Qt::LeftButton: sev->Button = sad::MouseButton::MouseLeft; break;
    case Qt::RightButton: sev->Button = sad::MouseButton::MouseRight; break;
    case Qt::MiddleButton: sev->Button = sad::MouseButton::MouseMiddle; break;
    default: sev->Button = sad::MouseButton::MouseLeft;
    }
}
