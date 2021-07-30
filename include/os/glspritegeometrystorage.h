/*! \file glspritegeometrystorage.h
 *
 *   A storage for sprite geometry
 */
#pragma once
#include "../sadhash.h"
#include "../sadvector.h"

namespace sad
{

class Renderer;

namespace os
{
/*! Default amount of pre-allocated geometries
 */
#define SAD_OS_GEOMETRY_STORAGE_SIZE (100)

/*! Declares a sprite storage, with geometry class and point count
 */
template<
    typename _Geometry,
    int _PointsCount
>
class GLSpriteGeometryStorage
{
public:
    /*! Initializes default, broken storage. To fix it, call  setRenderer
     */
    inline GLSpriteGeometryStorage() : m_renderer(nullptr)
    {
        
    }
    /*! Frees data from storage
     */
    ~GLSpriteGeometryStorage()
    {
        free();
    }
    /*! Sets renderer and pre-allocates geometries, related to renderer
     *  \param[in] r renderer
     */
    void setRenderer(sad::Renderer* r)
    {
        m_renderer = r;
        for (int i = 0; i < SAD_OS_GEOMETRY_STORAGE_SIZE; i++)
        {
            m_available_geometry_stack << new _Geometry(r, _PointsCount);
        }
    }
    /*! Frees all objects
     */
    void free()
    {
        for (typename sad::Hash<_Geometry*, _Geometry*>::iterator it = m_taken_geometries.begin(); it != m_taken_geometries.end(); ++it)
        {
            delete it.value();
        }
        m_taken_geometries.clear();
        for (size_t i = 0; i < m_available_geometry_stack.size(); ++i)
        {
            delete m_available_geometry_stack[i];
        }
        m_available_geometry_stack.clear();
    }
    /*! Unloads geometry from GPU
     */
    void unloadFromGPU()
    {
        for (typename sad::Hash<_Geometry*, _Geometry*>::iterator  it = m_taken_geometries.begin(); it != m_taken_geometries.end(); ++it)
        {
            it.value()->unload();
        }
        for (size_t i = 0; i < m_available_geometry_stack.size(); ++i)
        {
            m_available_geometry_stack[i]->unload();
        }
    }
    /*! Returns new geometry, allocating it or taking from stack
     *  \return value
     */
    _Geometry* take()
    {
        _Geometry* result;
        if (!m_available_geometry_stack.empty())
        {
            result = m_available_geometry_stack.back();
            m_available_geometry_stack.pop_back();
        }
        else
        {
            result = new _Geometry(m_renderer, _PointsCount);
        }
        m_taken_geometries.insert(result, result);
        return result;
    }
    /*! Stores geometry into list
     *  \param[in] g geometry
     */
    void store(_Geometry* g)
    {
        m_taken_geometries.remove(g);
        m_available_geometry_stack.push_back(g);
    }
private:
    /*! Geometries, that are currently in use by their owners
     */
    sad::Hash<_Geometry*, _Geometry*> m_taken_geometries;
    /*! Available geometries stack
     */
    sad::Vector<_Geometry*> m_available_geometry_stack;
    /*! A renderer class
     */
    sad::Renderer* m_renderer;
};

}

}
