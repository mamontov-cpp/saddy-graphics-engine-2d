/*! \file   scene.h
    
    \brief  Here placed a scene files
*/
#pragma once
#include "db/dbobject.h"
#include "sadvector.h"
#include "sadhash.h"
#include "temporarilyimmutablecontainer.h"
#include "sadmutex.h"
#include "scenenode.h"

#include <assert.h>


namespace sad
{
class Camera;

/*! Scene is a special container, which renders itself, using a renderer
 */
class Scene: public sad::db::Object, public sad::TemporarilyImmutableContainer<sad::SceneNode>
{
public:
    /*! Creates an empty scene
     */
    Scene();                   
    /*! You can freely inherit and implement your own scene
     */
    virtual ~Scene(); 
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
    /*! Returns current camera
        \return camera
     */
    sad::Camera* getCamera() const;
    /*! Returns current camera for scene
        \return current camera for scene
     */
    sad::Camera& camera() const; 
    /*! Sets a renderer
        \param[in] renderer renderer part
     */
    void setRenderer(sad::Renderer* renderer);
    /*! Clears renderer, without resetting it for some nodes
     */
    void clearRenderer();
    /*! Returns a renderer from scene
        \return renderer from scene
     */
    inline sad::Renderer* renderer() const
    {
        return m_renderer;
    }
    /*! Sets a camera in scene, deleting old camera
        \param[in] camera  new camera
    */
    void setCamera(sad::Camera* camera);
    /*! Finds a layers for node
        \param[in] node this node
        \return -1 if not found, index otherwise
     */
    int findLayer(sad::SceneNode* node);
    /*! Sets a layer for node. If no node found in scene - nothing happens, if layer not found - it 
        pushes to end
        \param[in] node node data
        \param[in] layer layer number
     */
    void setLayer(sad::SceneNode* node,unsigned int layer);
    /*! Swaps to layers for nodes
        \param[in] node1 first node
        \param[in] node2 second node
     */
    void swapLayers(sad::SceneNode* node1, sad::SceneNode* node2);
    /*! Renders a scene, making it visible
     */
    virtual void render();
    /*! Returns amount of scene objects
        \return objects amount
     */
    inline size_t objectCount() const
    {
        return m_layers.count();
    }
    /*! Returns objects for scene
        \return objects list for a scene
     */
    inline const sad::Vector<sad::SceneNode*>& objects() const
    {
        return m_layers;
    }
    /*! Sets scene activity flag, which determines, whether it should be rendered
        \param[in] active an activity flag
     */
    inline void setActive(bool active)
    {
        m_active = active;
    }
    /*! Returns scene activity flag, which determines, whether it should be rendered
        \return whether scene is active
     */
    inline bool active()
    {
        return m_active;
    }
    /*! Returns cached scene layer parameter for scene
        \return scene layer
     */
    unsigned int cachedSceneLayer() const;
    /*! Returns scene layer for scene
        \return scene layer
     */
    unsigned int sceneLayer() const;
    /*! Sets new scene layer
        \param[in] layer a layer
     */
    void setSceneLayer(unsigned int layer);
    /*! A name, under which object could be serialized
        \return name, under which object will be serialized
     */
    virtual const sad::String& serializableName() const;

    /*! Adds new node
        \param[in] s a scene node
     */
    inline void addNode(sad::SceneNode* s)
    {
        this->add(s);
    }
    /*! Removes a node from a scene
        \param[in] s a node
     */
    inline void removeNode(sad::SceneNode* s)
    {
        this->remove(s);
    }
    /*! Clears node list
     */
    inline void clearNodes()
    {
        this->clear();
    }
protected:
    /*! Determines, whether scene is active and should be rendered
     */
    bool                      m_active;
    /*! A cached scene layer for scene
     */
    unsigned int m_cached_layer;
    /*! A layers, which stores a node 
     */
    sad::Vector<SceneNode*>   m_layers;   
    /*! A camera, to be applied before rendering
     */
    sad::Camera*        m_camera;       
    /*! Renderer, which scene belongs to
     */
    sad::Renderer*        m_renderer;       
    /*! Adds an object to scene
        \param[in] node 
     */
    virtual void addNow(sad::SceneNode* node);
    /*! Removes object from scene
        \param[in] node
     */
    virtual void removeNow(sad::SceneNode* node);
    /*! Clears a scene
     */
    virtual void clearNow();
};

}
