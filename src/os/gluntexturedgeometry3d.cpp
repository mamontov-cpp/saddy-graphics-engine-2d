#include "os/gluntexturedgeometry3d.h"
#include "os/extensionfunctions.h"

#include "renderer.h"
#include "opengl.h"

#include <cassert>

// ===================================== PUBLIC METHODS =====================================

sad::os::GLUntexturedGeometry3D::GLUntexturedGeometry3D(sad::Renderer* renderer, unsigned int points)
: m_renderer(renderer), m_vertex_array(0), m_vertex_buffer(0), m_point_count(points), m_is_on_gpu(false)
{
    if (m_renderer == NULL)
    {
        m_renderer = sad::Renderer::ref();
    }
    assert(points > 0);
}

sad::os::GLUntexturedGeometry3D::~GLUntexturedGeometry3D()
{

}



void sad::os::GLUntexturedGeometry3D::setVertices(const sad::Rect< sad::Point3D >& vertices) const
{
    if (!m_is_on_gpu)
    {
        return;
    }
    sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::setVertices: glBindVertexArray(m_vertex_array)");

    f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::setVertices: glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer)");

    f->glBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(double), reinterpret_cast<const double*>(&vertices));
    tryLogGlError("sad::os::GLUntexturedGeometry3D::setVertices: glBufferSubData()");
}

void sad::os::GLUntexturedGeometry3D::setVertices(const double* vertexes) const
{
    if (!m_is_on_gpu)
    {
        return;
    }
    sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::setVertices: glBindVertexArray(m_vertex_array)");

    f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::setVertices: glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer)");

    f->glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * m_point_count * sizeof(double), vertexes);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::setVertices: glBufferSubData()");
}

void sad::os::GLUntexturedGeometry3D::loadToGPU()
{
    if (!m_is_on_gpu)
    {
        sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();

        f->glGenVertexArrays(1, &m_vertex_array);
        f->glBindVertexArray(m_vertex_array);

        f->glEnableVertexAttribArray(0);
        tryLogGlError("sad::os::GLUntexturedGeometry3D::loadToGPU: glEnableVertexAttribArray(0)");
        // Create vertex buffer
        f->glGenBuffers(1, &m_vertex_buffer);
        f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
        double* buffer = new double[3 * m_point_count];
        std::fill_n(buffer, 3 * m_point_count, 0.0);
        f->glBufferData(GL_ARRAY_BUFFER, 3 * m_point_count * sizeof(double), buffer, GL_DYNAMIC_DRAW);
        f->glVertexAttribPointer(
            0,
            3,
            GL_DOUBLE,
            GL_FALSE,
            0,
            static_cast<void*>(0)
        );
        tryLogGlError("sad::os::GLUntexturedGeometry3D::loadToGPU: glVertexAttribPointer");
        delete[] buffer;

        f->glDisableVertexAttribArray(0);
        tryLogGlError("sad::os::GLUntexturedGeometry3D::loadToGPU: glDisableVertexAttribArray(0)");

        f->glBindBuffer(GL_ARRAY_BUFFER, 0);
        f->glBindVertexArray(0);

        m_is_on_gpu = true;
    }
}

void sad::os::GLUntexturedGeometry3D::unload()
{
    if (m_is_on_gpu)
    {
        sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
        f->glDeleteBuffers(1, &m_vertex_buffer);
        f->glDeleteVertexArrays(1, &m_vertex_array);

        m_is_on_gpu = false;
    }
}

void sad::os::GLUntexturedGeometry3D::drawArrays(GLenum mode, const double* vertexes)
{
    if (!m_is_on_gpu)
    {
        this->loadToGPU();
    }
    if (!m_is_on_gpu)
    {
        return;
    }
    setVertices(vertexes);
    this->drawArrays(mode);
}

static GLubyte __indices[] = { 0, 3, 1, 2 };

void sad::os::GLUntexturedGeometry3D::drawArrays(GLenum mode, const sad::Rect<sad::Point3D>& vertexes)
{
    if (!m_is_on_gpu)
    {
        this->loadToGPU();
    }
    if (!m_is_on_gpu)
    {
        return;
    }
    setVertices(vertexes);

    sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glBindVertexArray(m_vertex_array)");

    f->glEnableVertexAttribArray(0);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glEnableVertexAttribArray(0)");

    // Render arrays
    glDrawElements(mode, 4, GL_UNSIGNED_BYTE, __indices);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glDrawElements(mode, 4, GL_UNSIGNED_BYTE, __indices)");

    f->glDisableVertexAttribArray(0);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glDisableVertexAttribArray(0)");
}

// ===================================== PRIVATE METHODS =====================================

void sad::os::GLUntexturedGeometry3D::drawArrays(GLenum mode) const
{
    sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glBindVertexArray(m_vertex_array)");

    f->glEnableVertexAttribArray(0);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glEnableVertexAttribArray(0)");

    // Render arrays
    glDrawArrays(mode, 0, m_point_count);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glDrawArrays(mode, 0, m_point_count)");

    f->glDisableVertexAttribArray(0);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glDisableVertexAttribArray(0)");
}

void sad::os::GLUntexturedGeometry3D::tryLogGlError(const char* op) const
{
    GLenum err_code = glGetError();

    if (err_code != GL_NO_ERROR)
    {
        sad::Renderer* r = sad::Renderer::ref();
        if (m_renderer)
        {
            r = m_renderer;
        }

        sad::String error_string = reinterpret_cast<const char*>(gluErrorString(err_code));
        sad::String error_data = op;
        error_data += ": ";
        error_data += error_string;
        SL_LOCAL_WARNING(error_data, *r);
    }
}