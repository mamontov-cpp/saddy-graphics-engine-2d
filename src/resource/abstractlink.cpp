#include "resource/abstractlink.h"
#include "resource/resource.h"
#include "resource/tree.h"
#include "resource/folder.h"

sad::resource::AbstractLink::AbstractLink(const sad::String & resource_type)
: 
m_notify_resource(false), 
m_resource(NULL),  
m_tree(NULL),
m_renderer(NULL),
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
    m_resource = NULL;
}

sad::resource::Resource* sad::resource::AbstractLink::resource() const
{
    if (m_resource == NULL)
    {
        sad::resource::Folder * folder = NULL;
        sad::resource::Tree * tree = this->tree();
        if (tree)
        {
            folder = tree->root();
        }
        sad::resource::Resource * res = NULL;
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
        m_resource = NULL;
    }
}

const sad::String&  sad::resource::AbstractLink::path() const
{
    return m_path;	
}

const sad::String & sad::resource::AbstractLink::treeName() const
{
    return m_treename;
}

void  sad::resource::AbstractLink::setTree(resource::Tree * tree)
{
    bool changed = m_tree == NULL || m_resource == NULL || tree != m_tree || m_render_dependent;
    m_render_dependent = false;
    m_tree = tree;
    if (changed)
    {
        if (m_resource && m_notify_resource)
        {
            m_resource->removeLink(this);
        }
        m_resource = NULL;
    }
}

sad::resource::Tree* sad::resource::AbstractLink::tree() const
{
    if (!m_tree && m_render_dependent && m_renderer)
    {
        const_cast<sad::resource::Tree *&>(this->m_tree) = m_renderer->tree(m_treename);
    }
    return m_tree;	
}

void sad::resource::AbstractLink::setTree(sad::Renderer * r, const sad::String& treename)
{
    bool changed = m_render_dependent == false || m_tree == NULL || r != m_renderer || treename != m_treename;
    m_render_dependent = true;
    m_renderer = r;
    m_treename = treename;
    if (changed)
    {
        if (m_resource && m_notify_resource)
        {
            m_resource->removeLink(this);
        }
        m_resource = NULL;
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
    return NULL;
}

void sad::resource::AbstractLink::setRenderer(sad::Renderer * r)
{
    this->setTree(r, m_treename);
}
