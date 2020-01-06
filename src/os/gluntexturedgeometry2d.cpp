#include "os/gluntexturedgeometry2d.h"
#include "os/extensionfunctions.h"

#include "renderer.h"
#include "opengl.h"

#include <cassert>

// ===================================== PUBLIC METHODS =====================================

sad::os::GLUntexturedGeometry2D::GLUntexturedGeometry2D(sad::Renderer* renderer, unsigned int points)
: m_renderer(renderer), m_vertex_array(0), m_vertex_buffer(0), m_point_count(points), m_is_on_gpu(false)
{
    if (m_renderer == NULL)
    {
        m_renderer = sad::Renderer::ref();
    }
    m_f = m_renderer->opengl()->extensionFunctions();
    assert(points > 0);
}

sad::os::GLUntexturedGeometry2D::~GLUntexturedGeometry2D()
{

}

void sad::os::GLUntexturedGeometry2D::setVertices(const sad::Rect2D& vertices) const
{
    if (!m_is_on_gpu)
    {
        return;
    }
    sad::os::ExtensionFunctions* f = m_f;
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::setVertices: glBindVertexArray(m_vertex_array)");

    f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::setVertices: glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer)");

    f->glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(double), reinterpret_cast<const double*>(&vertices));
    tryLogGlError("sad::os::GLUntexturedGeometry2D::setVertices: glBufferSubData()");
}

void sad::os::GLUntexturedGeometry2D::setVertices(const sad::Point2D& p1, const sad::Point2D& p2) const
{
    if (!m_is_on_gpu)
    {
        return;
    }
    sad::os::ExtensionFunctions* f = m_f;
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::setVertices: glBindVertexArray(m_vertex_array)");

    f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::setVertices: glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer)");

    double buf[4] = { p1.x() , p1.y(), p2.x(), p2.y() };
    f->glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(double), buf);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::setVertices: glBufferSubData()");
}

void sad::os::GLUntexturedGeometry2D::setVertices(const double* vertexes) const
{
    if (!m_is_on_gpu)
    {
        return;
    }
    sad::os::ExtensionFunctions* f = m_f;
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::setVertices: glBindVertexArray(m_vertex_array)");

    f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::setVertices: glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer)");

    f->glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * m_point_count * sizeof(double), vertexes);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::setVertices: glBufferSubData()");
}

void sad::os::GLUntexturedGeometry2D::loadToGPU()
{
    if (!m_is_on_gpu)
    {
        sad::os::ExtensionFunctions* f = m_f;

        f->glGenVertexArrays(1, &m_vertex_array);
        f->glBindVertexArray(m_vertex_array);

        f->glEnableVertexAttribArray(0);
        tryLogGlError("sad::os::GLUntexturedGeometry2D::loadToGPU: glEnableVertexAttribArray(0)");

        // Create vertex buffer
        f->glGenBuffers(1, &m_vertex_buffer);
        f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
        double* buffer = new double[2 * m_point_count];
        std::fill_n(buffer, 2 * m_point_count, 0.0);
        f->glBufferData(GL_ARRAY_BUFFER, 2 * m_point_count * sizeof(double), buffer, GL_DYNAMIC_DRAW);
        f->glVertexAttribPointer(
            0,
            2,
            GL_DOUBLE,
            GL_FALSE,
            0,
            static_cast<void*>(0)
        );
        tryLogGlError("sad::os::GLUntexturedGeometry2D::loadToGPU: glVertexAttribPointer");
        delete[] buffer;

        f->glDisableVertexAttribArray(0);
        tryLogGlError("sad::os::GLTexturedGeometry2D::loadToGPU: glDisableVertexAttribArray(0)");

        f->glBindBuffer(GL_ARRAY_BUFFER, 0);
        f->glBindVertexArray(0);

        m_is_on_gpu = true;
    }
}

void sad::os::GLUntexturedGeometry2D::unload()
{
    if (m_is_on_gpu)
    {
        sad::os::ExtensionFunctions* f = m_f;
        f->glDeleteBuffers(1, &m_vertex_buffer);
        f->glDeleteVertexArrays(1, &m_vertex_array);

        m_is_on_gpu = false;
    }
}

void sad::os::GLUntexturedGeometry2D::drawArrays(GLenum mode, const double* vertexes)
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

void sad::os::GLUntexturedGeometry2D::drawArrays(GLenum mode, const sad::Rect2D& vertexes)
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

    sad::os::ExtensionFunctions* f = m_f;
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::drawArrays: glBindVertexArray(m_vertex_array)");

    f->glEnableVertexAttribArray(0);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::drawArrays: glEnableVertexAttribArray(0)");

    // Render arrays
    glDrawElements(mode, 4, GL_UNSIGNED_BYTE, __indices);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::drawArrays: glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, __indices)");

    f->glDisableVertexAttribArray(0);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::drawArrays: glDisableVertexAttribArray(0)");
}

void sad::os::GLUntexturedGeometry2D::drawIndexedQuad() const
{
    sad::os::ExtensionFunctions* f = m_f;
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::drawArrays: glBindVertexArray(m_vertex_array)");

    f->glEnableVertexAttribArray(0);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::drawArrays: glEnableVertexAttribArray(0)");

    // Render arrays
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, __indices);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::drawArrays: glDrawElements(mode, 4, GL_UNSIGNED_BYTE, __indices)");

    f->glDisableVertexAttribArray(0);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::drawArrays: glDisableVertexAttribArray(0)");
}

void sad::os::GLUntexturedGeometry2D::drawLine(const sad::Point2D& p1, const sad::Point2D& p2)
{
    if (!m_is_on_gpu)
    {
        this->loadToGPU();
    }
    if (!m_is_on_gpu)
    {
        return;
    }
    setVertices(p1, p2);
    this->drawArrays(GL_LINES);
}

void sad::os::GLUntexturedGeometry2D::drawRectLines(const sad::Rect2D& r)
{
    if (!m_is_on_gpu)
    {
        this->loadToGPU();
    }
    if (!m_is_on_gpu)
    {
        return;
    }
    setVertices(r);
    this->drawArrays(GL_LINE_LOOP);
}


// ===================================== PRIVATE METHODS =====================================

void sad::os::GLUntexturedGeometry2D::drawArrays(GLenum mode) const
{
    sad::os::ExtensionFunctions* f = m_f;
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::drawArrays: glBindVertexArray(m_vertex_array)");

    f->glEnableVertexAttribArray(0);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::drawArrays: glEnableVertexAttribArray(0)");

    // Render arrays
    glDrawArrays(mode, 0, m_point_count);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::drawArrays: glDrawArrays(mode, 0, m_point_count)");

    f->glDisableVertexAttribArray(0);
    tryLogGlError("sad::os::GLUntexturedGeometry2D::drawArrays: glDisableVertexAttribArray(0)");
}

void sad::os::GLUntexturedGeometry2D::tryLogGlError(const char* op) const
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