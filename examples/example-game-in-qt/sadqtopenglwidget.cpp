#include "sadqtopenglwidget.h"

#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>


sad::qt::OpenGLWidget::OpenGLWidget(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f), m_first(true), m_time(0)
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
    
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
	m_timer.setInterval(0);
	m_timer.start();
}

sad::qt::OpenGLWidget::~OpenGLWidget()
{
	
}



void sad::qt::OpenGLWidget::resizeGL(int width, int height)
{
	if (width == 0)
	{
		width = 1;
	}
	if (m_first)
	{
		glShadeModel(GL_SMOOTH);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POINT_SMOOTH);

		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_COLOR_MATERIAL);
	}
	// Reset viewport for window
	glViewport(0, 0, width, height);

	// Clear projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//  Set perspective projection
	GLfloat aspectratio = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
	gluPerspective(
		45.0f,
		aspectratio,
		0.1f,
		100.0f
	);

	// Clear modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (m_first)
	{
		glFinish();
	}

	m_first = false;
	this->update();
}


void sad::qt::OpenGLWidget::paintGL()
{
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// Apply camera
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 640, 0, 480);
	glPopAttrib();

	// Render triangle
	glDisable(GL_TEXTURE_2D);
	GLint   clr[4] = {};
	glGetIntegerv(GL_CURRENT_COLOR, clr);
	glColor4ub(255, 255, 0, 255);

	glBegin(GL_TRIANGLES);

	glVertex2f(0, 0);
	glVertex2f(320, 240);
	glVertex2f(320 + 320 * cos(m_time), 240 + 120 * cos(m_time));

	glEnd();

	glColor4iv(clr);
	glEnable(GL_TEXTURE_2D);

	m_time += 1;
}