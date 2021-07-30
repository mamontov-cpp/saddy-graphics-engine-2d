/*! \file glfontgeometries.h

    Describes stored geometries, needed to render
 */
#pragma once
#include "../sadvector.h"
#include "../sadpoint.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "../fontshaderfunction.h"
#include "glfontgeometry.h"

namespace sad
{

class Label;

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
     *  \param[in] node a node to use
     *  \param[in] color color data
     *  \param[in] center a point center
     *  \param[in] angle an angle
     *  \param[in] fun render function
     */
    void draw(sad::Label* node, const sad::AColor& color, const sad::Point2D& center, double angle, sad::FontShaderFunction* fun);
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
        \param[in] points points
        \param[in] tc texture coordinates
        \param[in] own_color whether we own color
        \param[in] color a color stuff
     */
    void append(
        sad::Renderer* r,
        sad::Bindable* b, 
        const sad::Vector<double>& points, 
        const sad::Vector<double>& tc, 
        bool own_color,
        const sad::AColor& color
    );
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
