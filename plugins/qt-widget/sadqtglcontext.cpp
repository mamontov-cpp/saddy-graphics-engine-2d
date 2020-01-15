#include "qt/sadqtglcontext.h"
#include "qt/sadqtrenderer.h"

#include <window.h>
#include <camera.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include "../../3rdparty/glm/glm/glm.hpp"
#include "../../3rdparty/glm/glm/ext.hpp"


sad::qt::GLContext::GLContext() : m_window(NULL), m_valid(false)
{
    
}

sad::qt::GLContext::~GLContext()
{

}


bool sad::qt::GLContext::valid() const
{
    return m_valid;
}


void sad::qt::GLContext::swapBuffers()
{
    
}

#ifndef DEFAULT_DEPTH_VALUE
#define DEFAULT_DEPTH_VALUE 0.8f //!< Value, which gives us a z=0.5 in mapping coordinates
#endif

sad::Point3D sad::qt::GLContext::mapToViewport(const sad::Point2D & p, bool ztest)
{
    if (!m_window)
        return p;
    if (m_window->valid() == false || !valid())
        return p;
    if (m_window->renderer() == NULL)
    {
        return p;
    }

    bool isOpenGL3 = this->isOpenGL3compatible();
    sad::qt::Renderer* r = static_cast<sad::qt::Renderer*>(m_window->renderer());
    GLint*     viewport = r->viewport();

    GLfloat winx = 0, winy = 0, winz = 0;
    GLdouble result[3];

    winx = (float)(p.x());
    winy = (float)(p.y());
    if (ztest)
        glReadPixels((int)winx, (int)winy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winz);
    else
        winz = DEFAULT_DEPTH_VALUE;
    if (!isOpenGL3)
    {
        GLdouble  modelview[16];
        GLdouble  projection[16];
        GLdouble  oldprojection[16];

        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);

        bool hasCamera = false;
        sad::Camera* cam = r->getDefaultCamera();
        if (cam)
        {
            hasCamera = true;
            cam->apply();
            memcpy(oldprojection, projection, 16 * sizeof(GLdouble));
            glGetDoublev(GL_PROJECTION_MATRIX, projection);
        }

        gluUnProject(winx, winy, winz, modelview, projection, viewport, result, result + 1, result + 2);

        result[0] -= this->renderer()->globalTranslationOffset().x();
        result[1] -= this->renderer()->globalTranslationOffset().y();

        if (hasCamera)
        {
            glMatrixMode(GL_PROJECTION);
            glLoadMatrixd(oldprojection);
        }
    }
    else
    {
        glm::mat4x4 model, projection;
        bool hasCamera = false;
        sad::Camera* cam = r->getDefaultCamera();
        if (cam)
        {
            hasCamera = true;
            cam->forceRecomputeMatrices();
            model = glm::make_mat4x4(cam->modelViewMatrix());
            projection = glm::make_mat4x4(cam->projectionMatrix());
        }
        else
        {
            float id[16] = {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };
            model = glm::make_mat4x4(id);
            projection = glm::make_mat4x4(id);
        }
        glm::vec3 tmp_vec = glm::unProject(glm::vec3(winx, winy, winz), model, projection, glm::make_vec4(viewport));
        result[0] = tmp_vec[0];
        result[1] = tmp_vec[1];
        result[2] = tmp_vec[2];
    }

    return sad::Point3D(result[0], result[1], result[2]);
}


sad::os::GLContextHandle *  sad::qt::GLContext::handle() const
{
    return NULL;
}

sad::Window * sad::qt::GLContext::window() const
{
    return m_window;
}

bool sad::qt::GLContext::isOpenGL3compatible() const
{
    return true;
}

bool sad::qt::GLContext::createFor(sad::Window * win)
{
    m_window = win;
    m_valid = true;
    return true;
}

void sad::qt::GLContext::destroy()
{
    
}

void  sad::qt::GLContext::makeCurrent()
{
    
}

sad::Renderer* sad::qt::GLContext::renderer() const
{
    if (m_window)
    {
        return m_window->renderer();
    }
    return NULL;
}
