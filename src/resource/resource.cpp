#include "resource/resource.h"
#include "resource/abstractlink.h"
#include "resource/resourcefile.h"


#include "renderer.h"

#include <algorithm>
#include "opticksupport.h"

// ================================== sad::resource::Resource ==================================

DECLARE_SOBJ(sad::resource::Resource);


sad::resource::Resource::Resource() : m_folder(nullptr), m_file(nullptr), m_store_links(false)
{
    PROFILER_EVENT;
    
}

bool sad::resource::Resource::tryLoad(
        const sad::resource::ResourceFile & file,
        sad::Renderer * r,
        const picojson::value& options,
        bool store_links
)
{
    PROFILER_EVENT;
    m_options = options;
    if (!r)
    {
        r = sad::Renderer::ref();
    }
    if (file.name().length() == 0)
    {
        return false;
    }
    bool result = load(file, r, options);
    if (result)
    {
        if (store_links)
        {
            enableStoringLinks();
        }
        else
        {
            disableStoringLinks();
        }
    }
    return result;
}

void sad::resource::Resource::unloadFromGPU()
{
    PROFILER_EVENT;

}

sad::resource::Resource::~Resource()
{
    PROFILER_EVENT;
    for(size_t i = 0; i < m_links.size(); i++)
    {
        m_links[i]->detach();
    }
}

bool sad::resource::Resource::referenced() const
{
    PROFILER_EVENT;
    return m_links.count() != 0;
}

void sad::resource::Resource::replaceWith(resource::Resource* a)
{
    PROFILER_EVENT;
    a->m_links << this->m_links;
    size_t i = 0;
    sad::Vector<sad::resource::AbstractLink*> links = m_links;
    // Notify links, that we are gone
    for(i = 0; i < links.size(); i++)
    {
        links[i]->attach(a);
    }
    assert(i == links.size());
    // Clear self links
    m_links.clear();
}


void sad::resource::Resource::setParentFolder(resource::Folder* folder)
{
    PROFILER_EVENT;
    m_folder = folder;
}

sad::resource::Folder* sad::resource::Resource::parentFolder()
{
    PROFILER_EVENT;
    return m_folder;
}

const sad::String& sad::resource::Resource::name()
{
    PROFILER_EVENT;
    return m_name;	
}

void sad::resource::Resource::setName(const sad::String & name)
{
    PROFILER_EVENT;
    m_name = name;
}

void sad::resource::Resource::addLink(sad::resource::AbstractLink* link)
{	
    if (m_store_links && link)
    {
        if (std::find(m_links.begin(), m_links.end(), link) == m_links.end())
        {
            m_links << link;
        }
    }
}

void sad::resource::Resource::removeLink(sad::resource::AbstractLink* link)
{
    PROFILER_EVENT;
    if (m_store_links && link)
    {
        m_links.removeFirst(link);
    }		
}


void sad::resource::Resource::enableStoringLinks()
{
    PROFILER_EVENT;
    m_store_links = true;	
}

void sad::resource::Resource::disableStoringLinks()
{
    PROFILER_EVENT;
    m_store_links = false;
    m_links.clear();
}

bool sad::resource::Resource::shouldStoreLinks() const
{
    PROFILER_EVENT;
    return m_store_links;	
}

void sad::resource::Resource::setPhysicalFile(sad::resource::ResourceFile * file)
{
    PROFILER_EVENT;
    m_file = file;	
}

sad::resource::ResourceFile * sad::resource::Resource::file() const
{
    PROFILER_EVENT;
    return m_file;	
}

void sad::resource::Resource::setFactoryName(const sad::String & name)
{
    PROFILER_EVENT;
    m_factory_name = name;
}

const sad::String & sad::resource::Resource::factoryName() const
{
    PROFILER_EVENT;
    return m_factory_name;
}

const picojson::value & sad::resource::Resource::options() const
{
    PROFILER_EVENT;
    return m_options;	
}

bool sad::resource::Resource::supportsLoadingFromTar7z() const
{
    PROFILER_EVENT;
    return false;
}
