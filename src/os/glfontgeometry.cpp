#include "os/glfontgeometry.h"
#include "os/extensionfunctions.h"

#include "renderer.h"
#include "opengl.h"

#include <cassert>

// ===================================== PUBLIC METHODS =====================================

sad::os::GLFontGeometry::GLFontGeometry(sad::Renderer* renderer, unsigned int points)
: m_renderer(renderer), m_vertex_array(0), m_vertex_buffer(0), m_texture_buffer(0), m_own_color(false), m_point_count(points), m_used_points(points), m_is_on_gpu(false), m_bindable(nullptr)
{
    if (m_renderer == nullptr)
    {
        m_renderer = sad::Renderer::ref();
    }
    m_f = m_renderer->opengl()->extensionFunctions();
}

sad::os::GLFontGeometry::~GLFontGeometry()
{

}

void sad::os::GLFontGeometry::setVertices(const sad::Vector<double>& points) const
{
    if (!m_is_on_gpu)
    {
        return;
    }
    sad::os::ExtensionFunctions* f = m_f;
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLFontGeometry::setVertices: glBindVertexArray(m_vertex_array)");

    f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    tryLogGlError("sad::os::GLFontGeometry::setVertices: glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer)");

    f->glBufferSubData(GL_ARRAY_BUFFER, 0, points.size() * sizeof(double), reinterpret_cast<const double*>(&(points[0])));
    tryLogGlError("sad::os::GLFontGeometry::setVertices: glBufferSubData()");
}

void sad::os::GLFontGeometry::setTextureCoordinates(const sad::Vector<double>& textureCoordinates) const
{
    if (!m_is_on_gpu)
    {
        return;
    }
    sad::os::ExtensionFunctions* f = m_f;
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLFontGeometry::setTextureCoordinates: glBindVertexArray(m_vertex_array)");

    f->glBindBuffer(GL_ARRAY_BUFFER, m_texture_buffer);
    tryLogGlError("sad::os::GLFontGeometry::setTextureCoordinates: glBindBuffer(GL_ARRAY_BUFFER, m_texture_buffer)");

    f->glBufferSubData(GL_ARRAY_BUFFER, 0, textureCoordinates.size() * sizeof(double), reinterpret_cast<const double*>(&(textureCoordinates[0])));
    tryLogGlError("sad::os::GLFontGeometry::setTextureCoordinates: glBufferSubData()");
}

void sad::os::GLFontGeometry::unload()
{
    if (m_is_on_gpu)
    {
        sad::os::ExtensionFunctions* f = m_f;
	if (!f) 
	{
		return;
	}
        f->glDeleteBuffers(1, &m_vertex_buffer);
        f->glDeleteBuffers(1, &m_texture_buffer);
        f->glDeleteVertexArrays(1, &m_vertex_array);

        m_is_on_gpu = false;
    }
}

void sad::os::GLFontGeometry::loadToGPU()
{
    if (m_point_count == 0)
    {
        return;
    }
    if (!m_is_on_gpu)
    {
        sad::os::ExtensionFunctions* f = m_f;

        f->glGenVertexArrays(1, &m_vertex_array);
        f->glBindVertexArray(m_vertex_array);

        // Create vertex buffer
        f->glGenBuffers(1, &m_vertex_buffer);
        f->glEnableVertexAttribArray(0);
        tryLogGlError("sad::os::GLFontGeometry::loadToGPU: glEnableVertexAttribArray(0)");
        f->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
        double* buffer = new double[2 * m_point_count];
        std::fill_n(buffer, 2 * m_point_count, 0.0f);
        f->glBufferData(GL_ARRAY_BUFFER, 2 * m_point_count * sizeof(double), buffer, GL_DYNAMIC_DRAW);
        f->glVertexAttribPointer(
            0,
            2,
            GL_DOUBLE,
            GL_FALSE,
            0,
            static_cast<void*>(0)
        );
        tryLogGlError("sad::os::GLFontGeometry::loadToGPU: glVertexAttribPointer");
        delete[] buffer;

        // Create texture coordinates buffer
        f->glGenBuffers(1, &m_texture_buffer);
        f->glEnableVertexAttribArray(1);
        tryLogGlError("sad::os::GLFontGeometry::loadToGPU: glEnableVertexAttribArray(1)");
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
        tryLogGlError("sad::os::GLFontGeometry::loadToGPU: glVertexAttribPointer");
        delete[] buffer;

        f->glDisableVertexAttribArray(1);
        tryLogGlError("sad::os::GLFontGeometry::loadToGPU: glDisableVertexAttribArray(1)");

        f->glDisableVertexAttribArray(0);
        tryLogGlError("sad::os::GLFontGeometry::loadToGPU: glDisableVertexAttribArray(0)");

        f->glBindBuffer(GL_ARRAY_BUFFER, 0);
        f->glBindVertexArray(0);

        m_is_on_gpu = true;
    }
}

void sad::os::GLFontGeometry::draw()
{
    if (m_used_points == 0)
    {
        return;
    }
    if (!m_is_on_gpu)
    {
        this->loadToGPU();
    }
    if (!m_is_on_gpu)
    {
        return;
    }
    
    if (m_bindable)
    {
        m_bindable->bind();
    }

    sad::os::ExtensionFunctions* f = m_f;
    f->glBindVertexArray(m_vertex_array);
    tryLogGlError("sad::os::GLFontGeometry::draw: glBindVertexArray(m_vertex_array)");

    f->glEnableVertexAttribArray(0);
    tryLogGlError("sad::os::GLFontGeometry::draw: glEnableVertexAttribArray(0)");

    f->glEnableVertexAttribArray(1);
    tryLogGlError("sad::os::GLFontGeometry::draw: glEnableVertexAttribArray(1)");

    // Render arrays
    glDrawArrays(GL_TRIANGLES, 0, m_used_points);
    tryLogGlError("sad::os::GLFontGeometry::draw: glDrawArrays(GL_TRIANGLES, 0, m_point_count)");

    f->glDisableVertexAttribArray(1);
    tryLogGlError("sad::os::GLFontGeometry::draw: glDisableVertexAttribArray(1)");

    f->glDisableVertexAttribArray(0);
    tryLogGlError("sad::os::GLFontGeometry::draw: glDisableVertexAttribArray(0)");

}

void sad::os::GLFontGeometry::resize(unsigned int point_count)
{
    if (point_count <= m_point_count)
    {
        m_used_points = point_count;
    }
    else
    {
        if (m_is_on_gpu)
        {
            unload();
            m_is_on_gpu = false;
        }
        m_point_count = point_count;
        m_used_points = point_count;
    }
}

void sad::os::GLFontGeometry::setRenderer(sad::Renderer* r)
{
    m_renderer = r;
}

sad::Renderer* sad::os::GLFontGeometry::renderer() const
{
    return m_renderer;
}

void sad::os::GLFontGeometry::setBindable(sad::Bindable* bindable)
{
    m_bindable = bindable;
}

sad::Bindable* sad::os::GLFontGeometry::bindable() const
{
    return m_bindable;
}

// ===================================== PRIVATE METHODS =====================================

void sad::os::GLFontGeometry::tryLogGlError(const char* op) const
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