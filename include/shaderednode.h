/*! \file shaderednode.h
 *   
 *  A node, with attached shader, used to apply shader before rendering
 */
#pragma once
#include "scenenode.h"
#include "shader.h"

namespace sad
{
    
class ShaderedNode: public sad::SceneNode
{
SAD_OBJECT
public:
    /*! A new shadered node
     */
    ShaderedNode();
    /*! Destroys a shadered node
     */
    ~ShaderedNode();
    /*! Sets inner node for shadered node
     *  \param node a node
     */
    void setNode(sad::SceneNode* node);
    /*! Returns  inner node
     *  \return node
     */
    sad::SceneNode* node() const;
    /*! Sets shader for node
     *  \param[in] shader a shader
     */
    void setShader(sad::Shader* shader);
    /*! Returns shader
     *  \return shader
     */
    sad::Shader* shader() const;
    /*! Implement this to make object render a part of scene
     */
    virtual void render();
    /*! Fills vector of regions with data, that could be used for identifying bounds of item.
        As default, no regions are produced.
        \param[out] r a vector of regions
     */
    virtual void regions(sad::Vector<sad::Rect2D>& r);
    /*! Called, when renderer is changed for a scene
     */
    virtual void rendererChanged();
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
    /*! Sets scene
        \param[in] scene a scene, which will render a node
     */
    virtual void setScene(sad::Scene* scene);
    /*! Returns a scene, which will render a node
        \return scene, which will render a node
     */
    virtual sad::Scene* scene() const;
    /*! Resets object's non-serialized state, when restoring snapshot to
        ensure idempotency of restoring objects
     */
    virtual void reset();
private:
    /*! A node, used for rendering
     */
    sad::SceneNode* m_node;
    /*! A shader, attached to node
     */
    sad::Shader* m_shader;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::ShaderedNode);