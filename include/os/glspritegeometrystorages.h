/*! \file glspritegeometrystorages.h
 *
 *   Defines a storages for geometry
 */
#pragma once
#include "glspritegeometrystorage.h"
#include "gltexturedgeometry2d.h"
#include "gltexturedgeometry3d.h"
#include "gluntexturedgeometry2d.h"
#include "gluntexturedgeometry3d.h"

namespace sad
{

namespace os
{

/*! A storages for sprite geometry
 */
class GLSpriteGeometryStorages
{
public:
    /*! Creates new storages
     */
    inline GLSpriteGeometryStorages() {}
    /*! Frees all related data
     */
    inline ~GLSpriteGeometryStorages() {}
    /*! Sets renderer and pre-allocates geometries, related to renderer
     *  \param[in] r renderer
     */
    inline void setRenderer(sad::Renderer* r)
    {
        m_t2d_storage.setRenderer(r);
        m_t3d_storage.setRenderer(r);
        m_u2d_storage.setRenderer(r);
        m_u3d_storage.setRenderer(r);
    }

    /*! Frees all objects
     */
    inline void free()
    {
        m_t2d_storage.free();
        m_t3d_storage.free();
        m_u2d_storage.free();
        m_u3d_storage.free();
    }

    /*! Unloads geometry from GPU
     */
    inline void unloadFromGPU()
    {
        m_t2d_storage.unloadFromGPU();
        m_t3d_storage.unloadFromGPU();
        m_u2d_storage.unloadFromGPU();
        m_u3d_storage.unloadFromGPU();
    }

    /*! Returns new geometry, allocating it or taking from stack
     *  \return value
     */
    inline sad::os::GLTexturedGeometry2D* takeTextured2D()
    {
        return m_t2d_storage.take();
    }
    /*! Returns new geometry, allocating it or taking from stack
     *  \return value
     */
    inline sad::os::GLTexturedGeometry3D* takeTextured3D()
    {
        return m_t3d_storage.take();
    }
    /*! Returns new geometry, allocating it or taking from stack
     *  \return value
     */
    inline sad::os::GLUntexturedGeometry2D* takeUntextured2D()
    {
        return m_u2d_storage.take();
    }
    /*! Returns new geometry, allocating it or taking from stack
     *  \return value
     */
    inline sad::os::GLUntexturedGeometry3D* takeUntextured3D()
    {
        return m_u3d_storage.take();
    }
    /*! Stores geometry into list
     *  \param[in] g geometry
     */
    inline void store(sad::os::GLTexturedGeometry2D* g)
    {
        m_t2d_storage.store(g);
    }
    /*! Stores geometry into list
     *  \param[in] g geometry
     */
    inline void store(sad::os::GLTexturedGeometry3D* g)
    {
        m_t3d_storage.store(g);
    }
    /*! Stores geometry into list
     *  \param[in] g geometry
     */
    inline void store(sad::os::GLUntexturedGeometry2D* g)
    {
        m_u2d_storage.store(g);
    }
    /*! Stores geometry into list
     *  \param[in] g geometry
     */
    inline void store(sad::os::GLUntexturedGeometry3D* g)
    {
        m_u3d_storage.store(g);
    }
private:
    /*! A storage for textured 2d geometry
     */
    sad::os::GLSpriteGeometryStorage<sad::os::GLTexturedGeometry2D, 4> m_t2d_storage;
    /*! A storage for textured 3d geometry
     */
    sad::os::GLSpriteGeometryStorage<sad::os::GLTexturedGeometry3D, 4> m_t3d_storage;
    /*! A storage for untextured 2d geometry
     */
    sad::os::GLSpriteGeometryStorage<sad::os::GLUntexturedGeometry2D, 4> m_u2d_storage;
    /*! A storage for untextured 3d geometry
     */
    sad::os::GLSpriteGeometryStorage<sad::os::GLUntexturedGeometry3D, 4> m_u3d_storage;
};


}

}