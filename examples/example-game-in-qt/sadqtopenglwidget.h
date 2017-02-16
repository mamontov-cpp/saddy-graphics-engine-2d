/* \file sadopenglqtwidget.h
 
   Describes an OpenGLWidget, adapted to work with Saddy
 */
#pragma once
#include <QOpenGLWidget>
#include <QTimer>

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

	/** Resizes a viewport, according to a new width and height
	    \param[in] width width of viewport
	    \param[in] height height of viewport
	 */
	virtual void resizeGL(int width, int height);
	/** Performs scene rendering
	 */
	void paintGL();
protected:
	/*! Whether it was rendered first time
	 */
	bool m_first;
	/* A time value for rotating
	 */
	double m_time;
	/*! A timer for dynamical updates
	 */
	QTimer m_timer;
};


}


}
