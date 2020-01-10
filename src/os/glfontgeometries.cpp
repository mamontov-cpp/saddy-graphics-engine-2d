#include "os/glfontgeometries.h"

#include <cassert>

sad::os::GLFontGeometries::GLFontGeometries() : m_rendered_geometries(0)
{

}

sad::os::GLFontGeometries::~GLFontGeometries()
{

}

void sad::os::GLFontGeometries::draw()
{
    for (size_t i = 0; i < m_rendered_geometries; ++i)
    {
        m_font_geometries[i].draw();
    }
}


void  sad::os::GLFontGeometries::unload()
{
    for (size_t i = 0; i < m_font_geometries.size(); ++i)
    {
        m_font_geometries[i].unload();
    }
}

void sad::os::GLFontGeometries::setRenderer(sad::Renderer* r)
{
    for (size_t i = 0; i < m_font_geometries.size(); ++i)
    {
        m_font_geometries[i].setRenderer(r);
    }
}

sad::Renderer* sad::os::GLFontGeometries::renderer() const
{
    if (m_font_geometries.size() == 0)
    {
        return NULL;
    }
    return m_font_geometries[0].renderer();
}

sad::Bindable* sad::os::GLFontGeometries::bindable() const
{
    if (m_font_geometries.size() == 0)
    {
        return NULL;
    }
    return m_font_geometries[0].bindable();
}

bool  sad::os::GLFontGeometries::empty() const
{
    return m_rendered_geometries != 0;
}

void sad::os::GLFontGeometries::startRebuilding()
{
    m_rendered_geometries = 0;
}

void sad::os::GLFontGeometries::append(sad::Renderer* r, sad::Bindable* b, const sad::Vector<double>& points, const sad::Vector<double>& tc, const sad::Vector<float>& colors)
{
    assert(points.size() / 2 == tc.size() / 3);
    assert(points.size() / 2 == colors.size() / 4);

    if (m_rendered_geometries < m_font_geometries.size())
    {
        sad::os::GLFontGeometry& g = m_font_geometries[m_rendered_geometries];
        g.resize(points.size() / 2);
        g.setRenderer(r);
        g.setBindable(b);
        g.loadToGPU();
        g.setVertices(points);
        g.setTextureCoordinates(tc);
        g.setColors(colors);
        ++m_rendered_geometries;
    }
    else 
    {
        m_font_geometries.add(sad::os::GLFontGeometry(r, points.size() / 2));
        sad::os::GLFontGeometry& g = m_font_geometries[m_rendered_geometries];
        g.setBindable(b);
        g.loadToGPU();
        g.setVertices(points);
        g.setTextureCoordinates(tc);
        g.setColors(colors);
        ++m_rendered_geometries;
    }
}