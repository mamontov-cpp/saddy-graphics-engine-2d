/* \file sadqtopenglwidget.h
 
   Describes an OpenGLWidget, adapted to work with Saddy
 */
#pragma once
#include <QOpenGLWidget>
#include <QTimer>
#include <QMutex>

#include "sadqtrenderer.h"

#include <input/events.h>

namespace sad
{

namespace qt
{

class MouseCursor;
    
class OpenGLWidget: public QOpenGLWidget
{	
public:
    friend class sad::qt::MouseCursor;

    Q_OBJECT
public:
    /** Makes new widget with specified parent
        \param[in] parent a parent widget
        \param[in] f window flags
     */
    explicit OpenGLWidget(QWidget* parent = NULL, Qt::WindowFlags f = Qt::WindowFlags());
    /*! Destroys widget
     */
    ~OpenGLWidget();
    /*! Sets renderer for widget
        \param[in] renderer a renderer
     */
    void setRenderer(sad::qt::Renderer* renderer);
    /*! Returns renderer for widget
        \return renderer
     */
    sad::qt::Renderer* renderer() const;
    /*! Resizes a viewport, according to a new width and height
        \param[in] width width of viewport
        \param[in] height height of viewport
     */
    virtual void resizeGL(int width, int height);
    /*! Performs scene rendering
     */
    virtual void paintGL();
    /*! Handles mouse entering event for widget
        \param[in] ev event
     */
    virtual void enterEvent(QEvent* ev);
    /*! Handles mouse wheel event for widget
        \param[in] ev event
     */
    virtual void wheelEvent(QWheelEvent* ev);
    /*! Handles mouse double click event for widget
        \param[in] ev event
     */
    virtual void mouseDoubleClickEvent(QMouseEvent* ev);
    /*! Handles mouse press event for widget
        \param[in] ev event	 
     */
    virtual void mousePressEvent(QMouseEvent* ev);
    /*! Handles mouse move event for widget 
        \param[in] ev event
     */
    virtual void mouseMoveEvent(QMouseEvent* ev);
    /*! Handles mouse press event for widget
        \param[in] ev event
     */
    virtual void mouseReleaseEvent(QMouseEvent* ev);
    /*! Handles mouse leaving event for widget
        \param[in] ev event
    */
    virtual void leaveEvent(QEvent* ev);
    /*! Handles key press event for widget
        \param[in] ev event
     */
    virtual void keyPressEvent(QKeyEvent* ev);
    /*! Handles key release event for widget
        \param[in] ev event
    */
    virtual void keyReleaseEvent(QKeyEvent* ev);
    /*! Handles event for Widget
        \param[in] e event
     */
    virtual bool event(QEvent* e);
    /*! Catches events, sent to functions
        \param[in] obj object
        \param[in] ev event
     */
    virtual bool eventFilter(QObject* obj, QEvent* ev);
public slots:
    /*! Emitted, when applications starts quitting
     */
    virtual void applicationQuit();
    /*! Shows widget in fullscreen
     */
    virtual void goFullscreen();
    /*! Makes widget go windowed
     */
    virtual void goWindowed();
    /*! Resizes widget, so it fits the window
     */
    virtual void fitWidgetToWindow();
    /*! Sets renderer immediately
     * \param r renderer
     */
    virtual void immediateSetRenderer(void* r);
signals:
    void rendererChanged(void* r);
protected:
    /*! Tries to intercept activation event
        \param[in] ev event
     */
    void tryHandleActivateEvent(QEvent* ev) const;
    /* Tries to handle minimization for window
       \param[in] ev event
     */
    void tryHandleMinimization(QEvent* ev) const;
    /*! Maps point to viewport
        \param[in] p point a point
     */
    sad::Point3D toViewport(const QPoint& p) const;
    /*! Converts mouse event to sad::input::MouseEvent
        \param[in] ev event
        \param[out] sev sad::input::MouseEvent instance
     */
    void mouseEventToSadMouseEvent(QMouseEvent* ev, sad::input::MouseEvent* sev) const;
    /*! A window, that is attached to widget
     */
    QWidget* m_window;
    /*! Whether it was rendered first time
     */
    bool m_first;	
    /*! Whether renderer was reshaped
     */
    bool m_reshaped;
    /*! A renderer, that should be used for rendering in widget
     */
    sad::qt::Renderer* m_renderer;
    /*! A timer for dynamical updates
     */
    QTimer m_timer;
    /*! An old size of widget
     */
    sad::Size2I m_old_size;
    /*! An old for full screen
    */
    QRect m_old_geometry;
    /*! An old max size for window
    */
    QSize m_old_max_size;
    /*! Mutex for widget
     */
    QMutex m_mtx;
};


}


}
