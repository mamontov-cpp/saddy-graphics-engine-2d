#include "shaderednode.h"

DECLARE_SOBJ_INHERITANCE(sad::ShaderedNode, sad::SceneNode);

sad::ShaderedNode::ShaderedNode() : m_node(NULL), m_shader(NULL)
{
    
}

sad::ShaderedNode::~ShaderedNode()
{
    if (m_node)
    {
        m_node->delRef();
        m_node = NULL;
    }
    if (m_shader)
    {
        m_shader->delRef();
        m_shader = NULL;
    }
}

void sad::ShaderedNode::setNode(sad::SceneNode* node)
{
    if (m_node)
    {
        m_node->delRef();
    }
    m_node = node;
    if (m_node)
    {
        m_node->addRef();
    }
}

sad::SceneNode* sad::ShaderedNode::node() const
{
    return m_node;
}

void sad::ShaderedNode::setShader(sad::Shader* shader)
{
    if (m_shader)
    {
        m_shader->delRef();
    }
    m_shader = shader;
    if (m_shader)
    {
        m_shader->addRef();
    }
}

sad::Shader* sad::ShaderedNode::shader() const
{
    return m_shader;
}

void sad::ShaderedNode::render()
{
    if (m_node)
    {
        if (m_shader)
        {
            m_shader->use();
        }
        m_node->render();
        if (m_shader)
        {
            m_shader->disable();
        }
    }
}

void sad::ShaderedNode::regions(sad::Vector<sad::Rect2D>& r)
{
    if (m_node)
    {
        m_node->regions(r);
    }
    else
    {
        r << sad::Rect2D();
    }
}

void sad::ShaderedNode::rendererChanged()
{
    if (m_node)
    {
        m_node->rendererChanged();
    }
    this->sad::SceneNode::rendererChanged();
}

void sad::ShaderedNode::moveBy(const sad::Point2D& p)
{
    if (m_node)
    {
        m_node->moveBy(p);
    }
}

bool sad::ShaderedNode::canBeRotated() const
{
    if (m_node)
    {
        return m_node->canBeRotated();
    }
    return false;
}

void sad::ShaderedNode::rotate(double delta)
{
    if (m_node)
    {
        m_node->rotate(delta);
    }
}

void sad::ShaderedNode::setScene(sad::Scene* scene)
{
    if (m_node)
    {
        m_node->setScene(scene);
    }
    this->sad::SceneNode::setScene(scene);
}


sad::Scene* sad::ShaderedNode::scene() const
{
    if (m_node)
    {
        return m_node->scene();
    }
    return this->sad::SceneNode::scene();
}

void sad::ShaderedNode::reset()
{
    if (m_node)
    {
        m_node->reset();
    }
    this->sad::SceneNode::reset();
}