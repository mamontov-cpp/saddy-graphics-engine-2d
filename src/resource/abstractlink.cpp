#include "resource/abstractlink.h"
#include "resource/resource.h"
#include "resource/tree.h"
#include "resource/folder.h"

sad::resource::AbstractLink::AbstractLink(const sad::String & resource_type)
: 
m_notify_resource(false), 
m_resource(nullptr),  
m_tree(nullptr),
m_renderer(nullptr),
m_render_dependent(false),
m_resource_type(resource_type)
{
    
}

sad::resource::AbstractLink::~AbstractLink()
{
    if (m_resource && m_notify_resource)
    {
        m_resource->removeLink(this);
    }
}

void sad::resource::AbstractLink::attach(resource::Resource* r)
{
    if (m_resource)
    {
        bool notify_resource = m_resource->shouldStoreLinks();
        if (notify_resource)
        {
            m_resource->removeLink(this);
        }
    }
    m_resource = r;
    if (m_resource)
    {
        m_notify_resource = m_resource->shouldStoreLinks();
        if (m_notify_resource)
        {
            m_resource->addLink(this);		
        }
    }
}

void sad::resource::AbstractLink::detach()
{
    m_resource = nullptr;
}

sad::resource::Resource* sad::resource::AbstractLink::resource() const
{
    if (m_resource == nullptr)
    {
        sad::resource::Folder * folder = nullptr;
        sad::resource::Tree * tree = this->tree();
        if (tree)
        {
            folder = tree->root();
        }
        sad::resource::Resource * res = nullptr;
        if (folder)
        {
            res = folder->resource(m_path);
        }
        if (res)
        {
            if (res->metaData()->canBeCastedTo(m_resource_type))
            {
                // We cannot use checked cast because type stored as string, so we store 
                // only resource type as string
                sad::Object * tmp = res->metaData()->castTo(res, m_resource_type);
                res = static_cast<sad::resource::Resource*>(tmp);
                const_cast<sad::resource::AbstractLink*>(this)->attach(res);
            }
        }
    }
    return m_resource;
}

void sad::resource::AbstractLink::setPath(sad::String path)
{
    bool changed = m_path != path;
    m_path = path;
    if (changed)
    {
        if (m_resource && m_notify_resource)
        {
            m_resource->removeLink(this);
        }
        m_resource = nullptr;
    }
}

const sad::String&  sad::resource::AbstractLink::path() const
{
    return m_path;	
}

const sad::String & sad::resource::AbstractLink::treeName() const
{
    return m_tree_name;
}

void  sad::resource::AbstractLink::setTree(resource::Tree * tree)
{
    bool changed = m_tree == nullptr || m_resource == nullptr || tree != m_tree || m_render_dependent;
    m_render_dependent = false;
    m_tree = tree;
    if (changed)
    {
        if (m_resource && m_notify_resource)
        {
            m_resource->removeLink(this);
        }
        m_resource = nullptr;
    }
}

sad::resource::Tree* sad::resource::AbstractLink::tree() const
{
    if (!m_tree && m_render_dependent && m_renderer)
    {
        const_cast<sad::resource::Tree *&>(this->m_tree) = m_renderer->tree(m_tree_name);
    }
    return m_tree;	
}

void sad::resource::AbstractLink::setTree(sad::Renderer * r, const sad::String& tree_name)
{
    bool changed = m_render_dependent == false || m_tree == nullptr || r != m_renderer || tree_name != m_tree_name;
    if (m_render_dependent && (m_tree != nullptr) && (r == m_renderer))
    {
        if (r != nullptr)
        {
            changed = changed || (m_tree != r->tree(tree_name));
            if (changed)
            {
                m_tree = nullptr;
            }
        }
    }
    m_render_dependent = true;
    m_renderer = r;
    m_tree_name = tree_name;
    if (changed)
    {
        if (m_resource && m_notify_resource)
        {
            m_resource->removeLink(this);
        }
        m_resource = nullptr;
    }
}


sad::Renderer * sad::resource::AbstractLink::renderer() const
{
    if (m_render_dependent)
    {
        return m_renderer;
    }
    else
    {
        if (m_tree)
        {
            return m_tree->renderer();
        }
    }
    return nullptr;
}

void sad::resource::AbstractLink::setRenderer(sad::Renderer * r)
{
    this->setTree(r, m_tree_name);
}
