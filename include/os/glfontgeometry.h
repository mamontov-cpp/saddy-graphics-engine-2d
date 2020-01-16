/*! \file glfontgeometry.h

    A geometry for rendering fonts
 */
#pragma once
#include "glheaders.h"
#include "../sadcolor.h"
#include "../bindable.h"
#include "../sadvector.h"


namespace sad
{
class Renderer;

namespace os
{

class ExtensionFunctions;

/*! A geometry for rendering
    */
class GLFontGeometry
{
public:
    /*! Makes new geometry
        *  \param[in] renderer a renderer
        *  \param[in] points a point count
        */
    GLFontGeometry(sad::Renderer* renderer, unsigned int points);
    /*! Destroys geometry
        */
    ~GLFontGeometry();
    /*! Sets vertices for vertex buffer
     *  \param[in] points rectangle
     */
    void setVertices(const sad::Vector<double>& points) const;
    /*! Sets texture coordinates for texture buffer
     *  \param[in] textureCoordinates texture coordinates
     */
    void setTextureCoordinates(const sad::Vector<double>& textureCoordinates) const;
    /*! Tries to upload geometry to GPU
     */
    void loadToGPU();
    /*! Unloads resources from GPU
     */
    void unload();
    /*! Draws objects
     */
    void draw();
    /*! Resizes point count
     */
    void resize(unsigned int point_count);
    /*! Sets renderer for geometry
        \param[in] r renderer
     */
    void setRenderer(sad::Renderer* r);
    /*! Returns renderer
        \return renderer
     */
    sad::Renderer* renderer() const;
    /*! Sets bindable geometry
        \param[in] bindable
     */
    void setBindable(sad::Bindable* bindable);
    /*! Returns bindable
        \return bindable
     */
    sad::Bindable* bindable() const;
    /*! Sets if we own color
        \param[in] own_color
     */
    inline void setOwnColor(bool own_color)
    {
        m_own_color = own_color;
    }
    /*! Returns whether we own color
        \return own color
     */
    inline bool ownColor() const
    {
        return m_own_color;
    }
    /*! Sets color for geometry
        \param[in] clr color
     */
    inline void setColor(const sad::AColor& clr)
    {
        m_color = clr;
    }
    /*! Returns color
        \return color
     */
    inline sad::AColor& color()
    {
        return m_color;
    }
private:
    /*! Tries to log OpenGL error if occurred
     *  \param[in] op log info
     */
    inline void tryLogGlError(const char* op) const;
    /*! A renderer for geometry
     */
    sad::Renderer* m_renderer;
    /*! A vertex array for rendering
     */
    GLuint m_vertex_array;
    /*! A vertex buffer
     */
    GLuint m_vertex_buffer;
    /*! A texture buffer
     */
    GLuint m_texture_buffer;
    /*! Whether we own color, or should render global font color
     */
    bool m_own_color;
    /*! Color for rendering
     */
    sad::AColor m_color;
    /*! A point count for rendering points
     */
    unsigned int m_point_count;
    /*! A used points amount
     */
    unsigned int m_used_points;
    /*! Whether geometry is already uploaded to GPU
     */
    bool m_is_on_gpu;
    /*! Extension functions
     */
    sad::os::ExtensionFunctions* m_f;
    /*! Texture
     */
    sad::Bindable* m_bindable;
};

}

}
