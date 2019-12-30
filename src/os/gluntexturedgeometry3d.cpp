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
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glBindVertexArray(m_vertex_array)");

    f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer)");

    float buf[12];
    int i = 0;
    buf[i++] = static_cast<float>(vertices[0].x());
    buf[i++] = static_cast<float>(vertices[0].y());
    buf[i++] = static_cast<float>(vertices[0].z());

    buf[i++] = static_cast<float>(vertices[3].x());
    buf[i++] = static_cast<float>(vertices[3].y());
    buf[i++] = static_cast<float>(vertices[3].z());

    buf[i++] = static_cast<float>(vertices[1].x());
    buf[i++] = static_cast<float>(vertices[1].y());
    buf[i++] = static_cast<float>(vertices[1].z());

    buf[i++] = static_cast<float>(vertices[2].x());
    buf[i++] = static_cast<float>(vertices[2].y());
    buf[i] = static_cast<float>(vertices[2].z());

    f->glBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(float), buf);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::setVertices: glBufferSubData()");
}

void sad::os::GLUntexturedGeometry3D::setVertices(const float* vertexes) const
{
    if (!m_is_on_gpu)
    {
        return;
    }
    sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glBindVertexArray(m_vertex_array)");

    f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer)");

    void* buf = f->glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE)");

    if (buf)
    {
        memcpy(buf, vertexes, 3 *m_point_count * sizeof(float));
        f->glUnmapBuffer(GL_ARRAY_BUFFER);
        tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glUnmapBuffer(GL_ARRAY_BUFFER)");
    }
}

void sad::os::GLUntexturedGeometry3D::loadToGPU()
{
    if (!m_is_on_gpu)
    {
        sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();

        f->glGenVertexArrays(1, &m_vertex_array);
        f->glBindVertexArray(m_vertex_array);

        // Create vertex buffer
        f->glGenBuffers(1, &m_vertex_buffer);
        f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
        float* buffer = new float[3 * m_point_count];
        std::fill_n(buffer, 3 * m_point_count, 0.0f);
        f->glBufferData(GL_ARRAY_BUFFER, 3 * m_point_count * sizeof(float), buffer, GL_DYNAMIC_DRAW);
        delete[] buffer;

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

void sad::os::GLUntexturedGeometry3D::drawArrays(GLenum mode, const float* vertexes)
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
    this->drawArrays(mode);
}

void sad::os::GLUntexturedGeometry3D::tryLogGlError(const char* op) const
{
    sad::Renderer* r = sad::Renderer::ref();
    if (m_renderer)
    {
        r = m_renderer;
    }

    GLenum err_code = glGetError();
    if (err_code != GL_NO_ERROR)
    {
        sad::String error_string = reinterpret_cast<const char*>(gluErrorString(err_code));
        bool handled = false;
        sad::String error_data = op;
        error_data += ": ";
        error_data += error_string;
        SL_LOCAL_WARNING(error_data, *r);
    }
}

// ===================================== PRIVATE METHODS =====================================

void sad::os::GLUntexturedGeometry3D::drawArrays(GLenum mode) const
{
    sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glBindVertexArray(m_vertex_array)");

    f->glEnableVertexAttribArray(0);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glEnableVertexAttribArray(0)");

    f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer)");
    f->glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        static_cast<void*>(0)
    );
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glVertexAttribPointer");

    // Render arrays
    glDrawArrays(mode, 0, m_point_count);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glDrawArrays(mode, 0, m_point_count)");

    f->glDisableVertexAttribArray(0);
    tryLogGlError("sad::os::GLUntexturedGeometry3D::drawArrays: glDisableVertexAttribArray(0)");
}