#include "sadqtopenglwidget.h"

#include <fpsinterpolation.h>
#include <pipeline/pipeline.h>
#include <mousecursor.h>

#include <scene.h>
#include <camera.h>

#include <GL/glu.h>

sad::qt::OpenGLWidget::OpenGLWidget(QWidget* parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f), m_first(true), m_reshaped(false)
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

	m_renderer = new sad::qt::Renderer();
	m_renderer->setWidget(this);
}

sad::qt::OpenGLWidget::~OpenGLWidget()
{
	if (m_renderer->initialized())
	{
		m_renderer->deinitRendererAfterLoop();
	}
	if (m_renderer)
	{
		delete m_renderer;
	}
}


void sad::qt::OpenGLWidget::setRenderer(sad::qt::Renderer* renderer)
{
	if (renderer)
	{
		if (renderer != m_renderer)
		{
			m_first = true;
			m_reshaped = false;
			delete m_renderer;
			m_renderer = renderer;
			m_renderer->setWidget(this);
		}
	}
}

sad::qt::Renderer* sad::qt::OpenGLWidget::renderer() const
{
	return m_renderer;
}

void sad::qt::OpenGLWidget::resizeGL(int width, int height)
{
	if (width == 0)
	{
		width = 1;
	}
	if (m_first)
	{
		if (m_renderer->initialized())
		{
			m_renderer->initRendererBeforeLoop();
			m_first = false;
		}
		else
		{
			m_first = true;
		}
	}
	if (m_renderer->initialized())
	{
		m_renderer->reshape(width, height);
		m_reshaped = true;
	}
	this->update();
}

bool cvg = false;
void sad::qt::OpenGLWidget::paintGL()
{
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
}