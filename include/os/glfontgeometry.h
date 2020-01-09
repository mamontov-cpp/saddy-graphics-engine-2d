/*! \file glfontgeometry.h

    A geometry for rendering fonts
 */
#pragma once
#include "glheaders.h"
#include "geometry2d.h"
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
     *  \param[in] vertices rectangle
     */
    void setVertices(const sad::Vector<double>& points) const;
    /*! Sets texture coordinates for texture buffer
     *  \param[in] textureCoordinates texture coordinates
     */
    void setTextureCoordinates(const sad::Vector<double>& textureCoordinates) const;
    /*! Sets colors for color buffer
     *  \param[in] colors for color buffer
     */
    void setColors(const sad::Vector <float> & colors) const;
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
    void resize(unsigned int point_coint);
    /*! Sets renderer for geometry
        \param[in] r renderer
     */
    void setRenderer(sad::Renderer* r);
    /*! Returns renderer
        \return renderer
     */
    sad::Renderer* renderer() const;
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
    /*! A color buffer
     */
    GLuint m_color_buffer;
    /*! A point count for rendering points
     */
    unsigned int m_point_count;
    /*! Whether geometry is already uploaded to GPU
     */
    bool m_is_on_gpu;
    /*! Extension functions
     */
    sad::os::ExtensionFunctions* m_f;
};

}

}
