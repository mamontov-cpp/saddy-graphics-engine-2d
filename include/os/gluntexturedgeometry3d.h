/*! \file gluntexturedgeometry3d.h

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

/*! A geometry for rendering
 */
class GLUntexturedGeometry3D
{
public:
    /*! Makes new geometry
     *  \param[in] renderer a renderer
     *  \param[in] points a point count
     */
    GLUntexturedGeometry3D(sad::Renderer* renderer, unsigned int points);
    /*! Destroys geometry
     */
    ~GLUntexturedGeometry3D();
    /*! Sets vertices for vertex buffer
     *  \param[in] vertices rectangle
     */
    void setVertices(const sad::Rect< sad::Point3D >& vertices) const;
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
     *  \param[in] mode a mode
      * \param[in] vertexes vertexes
      */
    void drawArrays(GLenum mode, const sad::Rect<sad::Point3D>& vertexes);
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
};

}

}
