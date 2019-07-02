/*! \file glgeometry.h

    A geometry as set of points, required for rendering and texturing
 */
#pragma once
#include "glheaders.h"

namespace sad
{
class Renderer;

/*! A geometry for rendering
 */
class GLGeometry
{
public:
    /*! Makes new geometry
     *  \param[in] renderer a renderer
     *  \param[in] points a point count
     */
    GLGeometry(sad::Renderer* renderer, unsigned int points);
    /*! Destroys geometry
     */
    ~GLGeometry();
    /*! Sets vertices for vertex buffer
     *  \param[in] vertexes
     */
    void setVertices(const float* vertexes);
    /*! Sets texture coordinates for texture buffer
     *  \param[in] texture coordinates texture coordinates
     */
    void setTextureCoordinates(const float* textureCoordinates);
    /*! Tries to upload geometry to GPU
     */
    void loadToGPU();
    /*! Unloads resources from GPU
     */
    void unload();
private:
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
    /*! A point count for rendering points
     */
    unsigned int m_point_count;
    /*! Whether geometry is already uploaded to GPU
     */
    bool m_is_on_gpu;
};


}
