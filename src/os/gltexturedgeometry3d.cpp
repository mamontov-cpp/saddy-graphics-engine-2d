#include "os/gltexturedgeometry3d.h"
#include "os/extensionfunctions.h"

#include "renderer.h"
#include "opengl.h"

#include <cassert>

// ===================================== PUBLIC METHODS =====================================

sad::os::GLTexturedGeometry3D::GLTexturedGeometry3D(sad::Renderer* renderer, unsigned int points)
: m_renderer(renderer), m_vertex_array(0), m_vertex_buffer(0), m_texture_buffer(0), m_point_count(points), m_is_on_gpu(false)
{
    if (m_renderer == NULL)
    {
        m_renderer = sad::Renderer::ref();
    }
    assert(points > 0);
}

sad::os::GLTexturedGeometry3D::~GLTexturedGeometry3D()
{

}



void sad::os::GLTexturedGeometry3D::setVertices(const sad::Rect< sad::Point3D >& vertices) const
{
    if (!m_is_on_gpu)
    {
        return;
    }
    sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLTexturedGeometry3D::setVertices: glBindVertexArray(m_vertex_array)");

    f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    tryLogGlError("sad::os::GLTexturedGeometry3D::setVertices: glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer)");

    f->glBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(double), reinterpret_cast<const double*>(&vertices));
    tryLogGlError("sad::os::GLTexturedGeometry3D::setVertices: glBufferSubData()");
}

void sad::os::GLTexturedGeometry3D::setVertices(const double* vertexes) const
{
    if (!m_is_on_gpu)
    {
        return;
    }
    sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLTexturedGeometry3D::setVertices: glBindVertexArray(m_vertex_array)");

    f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    tryLogGlError("sad::os::GLTexturedGeometry3D::setVertices: glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer)");

    f->glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * m_point_count * sizeof(double), vertexes);
    tryLogGlError("sad::os::GLTexturedGeometry3D::setVertices: glBufferSubData()");
}




void sad::os::GLTexturedGeometry3D::setTextureCoordinates(const double* textureCoordinates) const
{
    if (!m_is_on_gpu)
    {
        return;
    }
    sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLTexturedGeometry3D::setTextureCoordinates: glBindVertexArray(m_vertex_array)");

    f->glBindBuffer(GL_ARRAY_BUFFER, m_texture_buffer);
    tryLogGlError("sad::os::GLTexturedGeometry3D::setTextureCoordinates: glBindBuffer(GL_ARRAY_BUFFER, m_texture_buffer)");

    f->glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * m_point_count * sizeof(double), textureCoordinates);
    tryLogGlError("sad::os::GLTexturedGeometry3D::setTextureCoordinates: glBufferSubData()");
}

void sad::os::GLTexturedGeometry3D::setTextureCoordinates(const sad::Rect2D& textureCoordinates) const
{
    if (!m_is_on_gpu)
    {
        return;
    }
    sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLTexturedGeometry3D::setTextureCoordinates: glBindVertexArray(m_vertex_array)");

    f->glBindBuffer(GL_ARRAY_BUFFER, m_texture_buffer);
    tryLogGlError("sad::os::GLTexturedGeometry3D::setTextureCoordinates: glBindBuffer(GL_ARRAY_BUFFER, m_texture_buffer)");

    f->glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(double), reinterpret_cast<const double*>(&textureCoordinates));
    tryLogGlError("sad::os::GLTexturedGeometry3D::setTextureCoordinates: glBufferSubData()");
}

void sad::os::GLTexturedGeometry3D::loadToGPU()
{
    if (!m_is_on_gpu)
    {
        sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();

        f->glGenVertexArrays(1, &m_vertex_array);
        f->glBindVertexArray(m_vertex_array);

        // Create vertex buffer
        f->glEnableVertexAttribArray(0);
        tryLogGlError("sad::os::GLTexturedGeometry3D::loadToGPU: glEnableVertexAttribArray(0)");
        f->glGenBuffers(1, &m_vertex_buffer);
        f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
        double* buffer = new double[3 * m_point_count];
        std::fill_n(buffer, 3 * m_point_count, 0.0f);
        f->glBufferData(GL_ARRAY_BUFFER, 3 * m_point_count * sizeof(double), buffer, GL_DYNAMIC_DRAW);
        f->glVertexAttribPointer(
            0,
            3,
            GL_DOUBLE,
            GL_FALSE,
            0,
            static_cast<void*>(0)
        );
        tryLogGlError("sad::os::GLTexturedGeometry3D::loadToGPU: glVertexAttribPointer");
        delete[] buffer;

        // Create texture coordinates buffer
        f->glEnableVertexAttribArray(1);
        tryLogGlError("sad::os::GLTexturedGeometry3D::loadToGPU: glEnableVertexAttribArray(1)");
        f->glGenBuffers(1, &m_texture_buffer);
        f->glBindBuffer(GL_ARRAY_BUFFER, m_texture_buffer);
        buffer = new double[2 * m_point_count];
        std::fill_n(buffer, 2 * m_point_count, 0.0);
        f->glBufferData(GL_ARRAY_BUFFER, 2 * m_point_count * sizeof(double), buffer, GL_DYNAMIC_DRAW);
        f->glVertexAttribPointer(
            1,
            2,
            GL_DOUBLE,
            GL_FALSE,
            0,
            static_cast<void*>(0)
        );
        tryLogGlError("sad::os::GLTexturedGeometry3D::loadToGPU: glVertexAttribPointer");
        delete[] buffer;

        f->glDisableVertexAttribArray(1);
        tryLogGlError("sad::os::GLTexturedGeometry3D::loadToGPU: glDisableVertexAttribArray(1)");

        f->glDisableVertexAttribArray(0);
        tryLogGlError("sad::os::GLTexturedGeometry3D::loadToGPU: glDisableVertexAttribArray(0)");

        f->glBindBuffer(GL_ARRAY_BUFFER, 0);
        f->glBindVertexArray(0);

        m_is_on_gpu = true;
    }
}

void sad::os::GLTexturedGeometry3D::unload()
{
    if (m_is_on_gpu)
    {
        sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
        f->glDeleteBuffers(1, &m_vertex_buffer);
        f->glDeleteBuffers(1, &m_texture_buffer);
        f->glDeleteVertexArrays(1, &m_vertex_array);

        m_is_on_gpu = false;
    }
}

void sad::os::GLTexturedGeometry3D::drawArrays(GLenum mode, const double* vertexes, const double* tc)
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
    if (tc)
    {
        setTextureCoordinates(tc);
    }
    this->drawArrays(mode);
}

static GLubyte __indices[] = { 0, 3, 1, 2 };

void sad::os::GLTexturedGeometry3D::drawArrays(GLenum mode, const sad::Rect<sad::Point3D>& vertexes, const sad::Rect2D& tc)
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
    setTextureCoordinates(tc);

    sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLTexturedGeometry3D::drawArrays: glBindVertexArray(m_vertex_array)");

    f->glEnableVertexAttribArray(0);
    tryLogGlError("sad::os::GLTexturedGeometry3D::drawArrays: glEnableVertexAttribArray(0)");

    f->glEnableVertexAttribArray(1);
    tryLogGlError("sad::os::GLTexturedGeometry3D::drawArrays: glEnableVertexAttribArray(1)");

    // Render arrays
    glDrawElements(mode, 4, GL_UNSIGNED_BYTE, __indices);
    tryLogGlError("sad::os::GLTexturedGeometry3D::drawArrays: glDrawElements(mode, 4, GL_UNSIGNED_BYTE, __indices)");

    f->glDisableVertexAttribArray(1);
    tryLogGlError("sad::os::GLTexturedGeometry3D::drawArrays: glDisableVertexAttribArray(1)");

    f->glDisableVertexAttribArray(0);
    tryLogGlError("sad::os::GLTexturedGeometry3D::drawArrays: glDisableVertexAttribArray(0)");
}

void sad::os::GLTexturedGeometry3D::tryLogGlError(const char* op) const
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

// ===================================== PRIVATE METHODS =====================================

void sad::os::GLTexturedGeometry3D::drawArrays(GLenum mode) const
{
    sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLTexturedGeometry3D::drawArrays: glBindVertexArray(m_vertex_array)");

    f->glEnableVertexAttribArray(0);
    tryLogGlError("sad::os::GLTexturedGeometry3D::drawArrays: glEnableVertexAttribArray(0)");

    f->glEnableVertexAttribArray(1);
    tryLogGlError("sad::os::GLTexturedGeometry3D::drawArrays: glEnableVertexAttribArray(1)");

    // Render arrays
    glDrawArrays(mode, 0, m_point_count);
    tryLogGlError("sad::os::GLTexturedGeometry3D::drawArrays: glDrawArrays(mode, 0, m_point_count)");

    f->glDisableVertexAttribArray(1);
    tryLogGlError("sad::os::GLTexturedGeometry3D::drawArrays: glDisableVertexAttribArray(1)");

    f->glDisableVertexAttribArray(0);
    tryLogGlError("sad::os::GLTexturedGeometry3D::drawArrays: glDisableVertexAttribArray(0)");
}