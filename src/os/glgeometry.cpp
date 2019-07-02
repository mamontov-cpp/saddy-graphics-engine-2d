#include "os/glgeometry.h"
#include "os/extensionfunctions.h"

#include "renderer.h"
#include "opengl.h"

#include <cassert>


sad::os::GLGeometry::GLGeometry(sad::Renderer* renderer, unsigned int points)
: m_renderer(renderer), m_vertex_array(0), m_vertex_buffer(0), m_texture_buffer(0), m_point_count(points), m_is_on_gpu(false)
{
    if (m_renderer == NULL)
    {
        m_renderer = sad::Renderer::ref();
    }
    assert(points > 0);
}

sad::os::GLGeometry::~GLGeometry()
{

}


void sad::os::GLGeometry::setVertices(const float* vertexes)
{
    if (!m_is_on_gpu)
    {
        return;
    }
    sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
    f->glBindVertexArray(m_vertex_array);
    f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    void* buf = f->glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if (buf)
    {
        memcpy(buf, vertexes, 3 *m_point_count * sizeof(float));
        f->glUnmapBuffer(GL_ARRAY_BUFFER);
    }
}




void sad::os::GLGeometry::setTextureCoordinates(const float* textureCoordinates)
{
    if (!m_is_on_gpu)
    {
        return;
    }
    sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
    f->glBindVertexArray(m_vertex_array);
    f->glBindBuffer(GL_ARRAY_BUFFER, m_texture_buffer);
    void* buf = f->glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if (buf)
    {
        memcpy(buf, textureCoordinates, 2 * m_point_count * sizeof(float));
        f->glUnmapBuffer(GL_ARRAY_BUFFER);
    }
}

void sad::os::GLGeometry::loadToGPU()
{
    if (!m_is_on_gpu)
    {
        sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();

        f->glGenVertexArrays(1, &m_vertex_array);
        f->glBindVertexArray(m_vertex_array);

        // Create vertex buffer
        f->glGenBuffers(1, &m_vertex_buffer);
        f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
        float* buffer = new float[3 * m_point_count * sizeof(float)];
        std::fill_n(buffer, 3 * m_point_count, 0.0f);
        f->glBufferData(GL_ARRAY_BUFFER, 3 * m_point_count * sizeof(float), buffer, GL_DYNAMIC_DRAW);
        delete buffer;

        // Create texture coordinates buffer
        f->glGenBuffers(1, &m_texture_buffer);
        f->glBindBuffer(GL_ARRAY_BUFFER, m_texture_buffer);
        buffer = new float[2 * m_point_count * sizeof(float)];
        std::fill_n(buffer, 2 * m_point_count, 0.0f);
        f->glBufferData(GL_ARRAY_BUFFER, 2 * m_point_count * sizeof(float), buffer, GL_DYNAMIC_DRAW);
        delete buffer;

        m_is_on_gpu = true;
    }
}

void sad::os::GLGeometry::unload()
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

void sad::os::GLGeometry::drawArrays(GLenum mode)
{
    if (!m_is_on_gpu)
    {
        this->loadToGPU();
    }
    if (!m_is_on_gpu)
    {
        return;
    }
    sad::os::ExtensionFunctions* f = m_renderer->opengl()->extensionFunctions();
    f->glBindVertexArray(m_vertex_array);

    f->glEnableVertexAttribArray(0);
    f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    f->glVertexAttribPointer(
       0,
       3,
       GL_FLOAT,
       GL_FALSE,
       0,
       (void*)0
    );

    f->glEnableVertexAttribArray(1);
    f->glBindBuffer(GL_ARRAY_BUFFER, m_texture_buffer);
    f->glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    // Render arrays
    glDrawArrays(mode, 0, m_point_count);

    f->glDisableVertexAttribArray(1);
    f->glDisableVertexAttribArray(0);
}


