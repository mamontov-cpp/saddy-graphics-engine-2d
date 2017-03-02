/* \file sadqtopenglwidget.h
 
   Describes an OpenGLWidget, adapted to work with Saddy
 */
#pragma once
#include <QOpenGLWidget>
#include <QTimer>
#include "sadqtrenderer.h"

namespace sad
{

namespace qt
{
	
class OpenGLWidget: public QOpenGLWidget
{	
public:
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
	/** Resizes a viewport, according to a new width and height
	    \param[in] width width of viewport
	    \param[in] height height of viewport
	 */
	virtual void resizeGL(int width, int height);
	/** Performs scene rendering
	 */
	virtual void paintGL();
public slots:
	/*! Emitted, when applications starts quitting
	 */
	virtual void applicationQuit();
protected:
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
};


}


}
