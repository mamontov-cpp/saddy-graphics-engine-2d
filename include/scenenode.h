/*! \file   scenenode.h
    

    Defines a node of a scene, which is basic for any renderable part
 */
#pragma once
#include "object.h"
#include "sadvector.h"
#include "sadrect.h"
#include "shaderfunction.h"

namespace sad
{
class Scene;
class Renderer;

/*! Defines an object, which is basic for any renderable part of scene
 */
class SceneNode: public sad::Object
{
SAD_OBJECT
public:
    /*! Defines a basic renderable part of scene
     */
    SceneNode();
    /*! Implement this to make object render a part of scene
     */
    virtual void render() = 0;
    /*! Fills vector of regions with data, that could be used for identifying bounds of item.
        As default, no regions are produced.
        \param[out] r a vector of regions
     */
    virtual void regions(sad::Vector<sad::Rect2D> & r);
    /*! Returns regions for a scene node
        \return region list
     */
    inline sad::Vector<sad::Rect2D>  getRegions()
    {
        sad::Vector<sad::Rect2D> r;
        regions(r);
        return r;
    }
    /*! Returns first region or empty rect if nothing
        \return first region
     */
    inline sad::Rect2D region0()
    {
        sad::Vector<sad::Rect2D> r;
        regions(r);
        return (r.size() == 0) ? sad::Rect2D() : r[0];
    }
    /*! Called, when renderer is changed for a scene
     */
    virtual void rendererChanged();
    /*! Must be implemented in order to get node destroyed
     */
    virtual ~SceneNode();
    /*! Resets object's non-serialized state, when restoring snapshot to 
        ensure idempotency of restoring objects
     */
    virtual void reset();
    /*! A basic schema for object
        \return a schema 
     */
    static sad::db::schema::Schema* basicSchema();
    /*! Returns schema for an object
        \return schema
     */
    virtual sad::db::schema::Schema* schema() const;
    /*! Sets scene 
        \param[in] scene a scene, which will render a node
     */
    virtual void setScene(sad::Scene * scene);
    /*! Returns a scene, which will render a node
        \return scene, which will render a node
     */
    virtual sad::Scene * scene() const;
    /*! Returns a renderer, which scene is belongs to
        \returns a renderer
     */
    sad::Renderer * renderer() const;
    /*! Sets scene activity flag, which determines, whether it should be rendered
        \param[in] active an activity flag
     */
    inline void setActive(bool active)
    {
        this->Active = active;
    }
    /*! Returns scene node activity flag, which determines, whether it should be rendered
        \return whether scene node is active
     */
    inline bool active()
    {
        return this->Active;
    }
    /*! Sets scene visibilty flag, which determines, whether it should be rendered
        \param[in] visible a visibility flag
     */
    inline void setVisible(bool visible)
    {
        m_visible = visible;
    }
    /*! Returns scene node visibility flag, which determines, whether it should be rendered
        \return whether scene node is visible
     */
    inline bool visible()
    {
        return m_visible;
    }
    /*! Sets cached layer value. If scene is set for node, forces node to set it's layer. 
        Note, that value will not be preserved, when a layer set after that.
        \param[in] layer a layer valye
     */
    void setCachedLayer(unsigned int layer);
    /*! Returns cached value for scene node, if scene is not set, otherwise returns node's layer.
        \return cached layer value
     */
    unsigned int cachedLayer() const;
    /*! Sets cached scene id for scene node
        \param[in] sceneid a cached scene id
     */
    void setCachedSceneId(unsigned long long sceneid);
    /*! Returns scene id, if scene is set, otherwiser returns cached scene id
        \return scene id
     */
    unsigned long long sceneId() const;
    /*! Moves object by specified vector
        \param[in] p point
     */
    virtual void moveBy(const sad::Point2D& p);
    /*! Tests if scene node can be rotated
        \return if node can be rotated
     */
    virtual bool canBeRotated() const;
    /*! Tries to rotate item by specified angle
        \param[in] delta a delta for node
     */
    virtual void rotate(double delta);
    /*! Sets shader function
     *  \param[in] fun a function
     */
    void setShaderFunction(sad::ShaderFunction* fun);
    /*! Returns shader function
     *  \return shader function
     */
    sad::ShaderFunction* shaderFunction() const;
protected:
    /*! Determines, whether scene node is visible and should be rendered. It's same as m_active but can be used for different purposes,
        when object is active, but hidden by somewhere else in chain of responsibility of application.
     */ 
    bool         m_visible;
    /*! A scene, which node is attached to
     */
    sad::Scene * m_scene;
    /*! A special layer cache to be set for a scene node
     */
    unsigned int m_cached_layer;
    /*! A cached scene minor id
     */
    unsigned long long m_cached_scene_id;
    /*! A shader function for scene node
     */
    sad::ShaderFunction* m_shader_function;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::SceneNode)
