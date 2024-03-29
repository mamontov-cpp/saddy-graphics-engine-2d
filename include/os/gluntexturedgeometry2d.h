/*! \file gluntexturedgeometry2d.h

    A geometry as set of points, required for rendering and texturing
 */
#pragma once
#include "glheaders.h"
#include "geometry2d.h"

namespace sad
{
class Renderer;

namespace os
{

class ExtensionFunctions;

/*! A geometry for rendering
 */
class GLUntexturedGeometry2D
{
public:
    /*! Makes new geometry
     *  \param[in] renderer a renderer
     *  \param[in] points a point count
     */
    GLUntexturedGeometry2D(sad::Renderer* renderer, unsigned int points);
    /*! Destroys geometry
     */
    ~GLUntexturedGeometry2D();
    /*! Sets vertices for vertex buffer
     *  \param[in] vertices rectangle
     */
    void setVertices(const sad::Rect2D& vertices) const;
    /*! Sets vertices for vertex buffer
     *  \param[in] p1 first point
     *  \param[in] p2 second point
     */
    void setVertices(const sad::Point2D& p1, const sad::Point2D& p2) const;
    /*! Sets vertices for vertex buffer
     *  \param[in] vertices rectangle
     */
    void setVertices(const double* vertices) const;
    /*! Tries to upload geometry to GPU
     */
    void loadToGPU();
    /*! Unloads resources from GPU
     */
    void unload();
    /*! Draws array list
     * \param[in] mode a mode
     * \param[in] vertexes vertexes
     */
    void drawArrays(GLenum mode, const double* vertexes);
    /*! Draws array list
     * \param[in] mode a mode
      * \param[in] vertexes vertexes
      */
    void drawArrays(GLenum mode, const sad::Rect2D& vertexes);
    /*! Draws indexed quad via GL_TRIANGLE_STRIP and quad
     */
    void drawIndexedQuad() const;
    /*! Draws line for geometry
     *  \param[in] p1 first point
     *  \param[in] p2 second point
     */
    void drawLine(const sad::Point2D& p1, const sad::Point2D& p2);
    /*! Draws rectangle with lines
     *  \param[in] r rectangle
     */
    void drawRectLines(const sad::Rect2D& r);
private:
    /*! Draws array list
     * \param[in] mode a mode
     */
    inline void drawArrays(GLenum mode) const;
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
