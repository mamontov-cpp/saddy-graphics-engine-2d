#include "orthographiccamera.h"
#include "renderer.h"
#include "glcontext.h"
#include "log/log.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include "opticksupport.h"

DECLARE_SOBJ_INHERITANCE(sad::OrthographicCamera, sad::Camera)

sad::OrthographicCamera::OrthographicCamera()
: m_fetched(false), m_width(1), m_height(1)
{
    PROFILER_EVENT;

}

sad::OrthographicCamera::OrthographicCamera(int width, int height)
: m_fetched(true), m_width(width), m_height(height)
{
    PROFILER_EVENT;
    fillProjectionMatrix();
}

void sad::OrthographicCamera::apply()
{
    PROFILER_EVENT;
    if (!m_fetched)
    {
        m_fetched = true;
        m_width = m_scene->renderer()->settings().width();
        m_height = m_scene->renderer()->settings().height();

        fillProjectionMatrix();
    }

    sad::Renderer* renderer = sad::Renderer::ref();
    if (m_scene)
    {
        sad::Renderer* local_renderer = m_scene->renderer();
        if (local_renderer)
        {
            renderer = local_renderer;
        }
    }
    if (!(renderer->context()->isOpenGL3compatible()))
    {
        glPushAttrib(GL_TRANSFORM_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, m_width, 0, m_height, -1, 1);
        glPopAttrib();
    }

    this->sad::Camera::apply();
}

sad::OrthographicCamera::~OrthographicCamera()
{
    PROFILER_EVENT;
}


void sad::OrthographicCamera::fillProjectionMatrix()
{
    PROFILER_EVENT;
    float left = 0, right = static_cast<float>(m_width), bottom = 0, top = static_cast<float>(m_height), znear = -1, zfar = 1;

    float tx = -1, ty = -1, tz = (zfar + znear) / (zfar - znear);

    m_projection_matrix[0] = 2.0f / (right - left);
    m_projection_matrix[4] = 0;
    m_projection_matrix[8] = 0;
    m_projection_matrix[12] = tx;

    m_projection_matrix[1] = 0;
    m_projection_matrix[5] = 2.0f / (top - bottom);
    m_projection_matrix[9] = 0;
    m_projection_matrix[13] = ty;

    m_projection_matrix[2] = 0;
    m_projection_matrix[6] = 0;
    m_projection_matrix[10] = -2.0f / (zfar - znear);
    m_projection_matrix[14] = tz;

    m_projection_matrix[3] = 0;
    m_projection_matrix[7] = 0;
    m_projection_matrix[11] = 0;
    m_projection_matrix[15] = 1;
}
