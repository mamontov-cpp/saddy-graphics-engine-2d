/*! \file glfontgeometries.h

    Describes stored geometries, needed to render
 */
#pragma once
#include "../sadvector.h"
#include "glfontgeometry.h"

namespace sad
{

namespace os
{

/*! Stored geometries list
 */
class GLFontGeometries
{
public:
    /*! Geometries
     */
    GLFontGeometries();
    /*! Destructs data
     */
    ~GLFontGeometries();
    /*! Draw geometries
     */
    void draw();
    /*! Unloads resources from GPU
     */
    void unload();
    /*! Sets renderer for geometry
        \param[in] r renderer
     */
    void setRenderer(sad::Renderer* r);
    /*! Returns renderer
        \return renderer
     */
    sad::Renderer* renderer() const;
    /*! Returns bindable
        \return bindable
     */
    sad::Bindable* bindable() const;
    /*! Returns true if nothing will be drawn
        \return true if empty
     */
    bool empty() const;
    /*! Called, when label starts rebuilding all buffers
     */
    void startRebuilding();
    /*! Appends new geometry
        \param[in] r renderer
        \param[in] b bindable object
        \param[in] point points
        \param[in] tc texture coordinates
        \param[in] colors colors data
     */
    void append(sad::Renderer* r, sad::Bindable* b, const sad::Vector<double>& points, const sad::Vector<double>& tc, const sad::Vector<float>& colors);
private:
    /*! List of font geometries
     */
    sad::Vector<sad::os::GLFontGeometry> m_font_geometries;
    /*! Rendered geometries count
     */
    size_t m_rendered_geometries;
};

}

}
