#include "resource/folder.h"
#include "resource/resource.h"
#include "opticksupport.h"

void sad::resource::free(const sad::resource::ResourceEntryList & list)
{
    PROFILER_EVENT;
    for(size_t i = 0; i < list.size(); i++)
    {
        delete list[i].p2();
    }
}

sad::resource::Folder::Folder() : m_parent(nullptr)
{
    PROFILER_EVENT;

}

sad::resource::Folder::~Folder()
{
    PROFILER_EVENT;
    for(sad::Hash<sad::String, sad::resource::Resource*>::iterator it = m_resources.begin();
        it != m_resources.end();
        ++it)
    {
        it.value()->delRef();
    }
    m_resources.clear();
}

bool sad::resource::Folder::hasFolders() const
{
    PROFILER_EVENT;
    return m_subfolders.count() != 0;
}

bool sad::resource::Folder::hasResources() const
{
    PROFILER_EVENT;
    return m_resources.count() != 0;
}

bool sad::resource::Folder::addFolder(const sad::String& path, sad::resource::Folder* folder)
{
    PROFILER_EVENT;
    sad::String foldername;
    sad::resource::Folder * parent = navigateParentFolder(path, true, foldername);
    if (parent == nullptr)
    {
        return false;
    }
    if (parent->m_subfolders.contains(foldername))
    {
        delete parent->m_subfolders[foldername]; //-V515
    }
    parent->m_subfolders.insert(foldername, folder);
    folder->setParent(parent);
    return true;
}


bool sad::resource::Folder::addResources(const sad::resource::ResourceEntryList & list, bool ref)
{
    PROFILER_EVENT;
    bool result = true;
    for(size_t i = 0; i < list.size(); i++)
    {
        result = result &&	this->addResource(list[i].p1(), list[i].p2(), ref);
    }
    return result;
}

void sad::resource::Folder::replaceResources(const sad::resource::ResourceEntryList & list)
{
    PROFILER_EVENT;
    for(size_t i = 0; i < list.size(); i++)
    {
        this->replaceResource(list[i].p1(), list[i].p2());
    }
}

void sad::resource::Folder::removeResources(
    const sad::resource::ResourceEntryList & list, 
    bool free
)
{
    PROFILER_EVENT;
    for(size_t i = 0; i < list.size(); i++)
    {
        this->removeResource(list[i].p1(), free);
    }	
}

sad::Vector<sad::String> sad::resource::Folder::duplicatesBetween(
    const sad::resource::ResourceEntryList & list
)
{
    PROFILER_EVENT;
    sad::Vector<sad::String> result;
    for(size_t i = 0; i < list.size(); i++)
    {
        if (this->resource(list[i].p1()) != nullptr)
        {
            result << list[i].p1();
        }
    }
    return result;
}

bool sad::resource::Folder::addResource(const sad::String & path, sad::resource::Resource* r, bool ref)
{
    PROFILER_EVENT;
    sad::String resourcename;
    sad::resource::Folder * parent = navigateParentFolder(path, true, resourcename);
    if (parent == nullptr)
    {
        return false;
    }
    if (parent->m_resources.contains(resourcename))
    {
        parent->m_resources[resourcename]->delRef();
    }
    if (ref)
    {
        r->addRef();
    }
    parent->m_resources.insert(resourcename, r);
    r->setParentFolder(parent);
    return true;	
}

void sad::resource::Folder::removeFolder(const sad::String& path, bool free)
{
    PROFILER_EVENT;
    sad::String foldername;
    sad::resource::Folder * parent = navigateParentFolder(path, false, foldername);
    if (parent == nullptr)
    {
        return;
    }
    if (parent->m_subfolders.contains(foldername))
    {
        parent->setParent(nullptr);
        if (free) 
        {
            delete parent->m_subfolders[foldername]; //-V515
        }
        parent->m_subfolders.remove(foldername);
    }
}

void sad::resource::Folder::removeResource(const sad::String& path, bool free)
{
    PROFILER_EVENT;
    sad::String resourcename;
    sad::resource::Folder * parent = navigateParentFolder(path, false, resourcename);
    if (parent == nullptr)
    {
        return;
    }
    if (parent->m_resources.contains(resourcename))
    {
        parent->setParent(nullptr);
        if (free) 
        {
            parent->m_resources[resourcename]->delRef();
        }
        parent->m_resources.remove(resourcename);
    }
            
}

sad::resource::Folder* sad::resource::Folder::folder(const sad::String& name)
{
    PROFILER_EVENT;
    sad::String foldername;
    resource::Folder * parent = this->navigateParentFolder(name, false, foldername);
    resource::Folder * result = nullptr;
    if (parent)
    {
        if (parent->m_subfolders.contains(foldername))
        {
            result = parent->m_subfolders[foldername];
        }
    }
    return result;
}

sad::resource::Resource* sad::resource::Folder::resource(const sad::String& name)
{
    PROFILER_EVENT;
    sad::String foldername;
    resource::Folder * parent = this->navigateParentFolder(name, false, foldername);
    resource::Resource * result = nullptr;
    if (parent)
    {
        if (parent->m_resources.contains(foldername))
        {
            result = parent->m_resources[foldername];
        }
    }
    return result;	
}

void sad::resource::Folder::replaceResource(const sad::String& path, resource::Resource* r)
{
    PROFILER_EVENT;
    sad::resource::Resource * old = resource(path);
    if (old)
    {
        old->replaceWith(r);
        this->removeResource(path, true);
    }
    this->addResource(path, r);
}

sad::resource::FolderIterator sad::resource::Folder::folderListBegin()
{
    PROFILER_EVENT;
    return m_subfolders.begin();
}

sad::resource::FolderIterator sad::resource::Folder::folderListEnd()
{
    PROFILER_EVENT;
    return m_subfolders.end();
}

sad::resource::ResourceIterator sad::resource::Folder::resourceListBegin()
{
    PROFILER_EVENT;
    return m_resources.begin();
}

sad::resource::ResourceIterator sad::resource::Folder::resourceListEnd()
{
    PROFILER_EVENT;
    return m_resources.end();
}

unsigned int  sad::resource::Folder::foldersCount() const
{
    PROFILER_EVENT;
    return m_subfolders.count();
}

unsigned int  sad::resource::Folder::resourceCount() const
{
    PROFILER_EVENT;
    return m_resources.count();
}

void sad::resource::Folder::setParent(sad::resource::Folder * folder)
{
    PROFILER_EVENT;
    m_parent = folder;
}

sad::resource::Folder * sad::resource::Folder::parent() const
{
    PROFILER_EVENT;
    return m_parent;
}

sad::Maybe<sad::String> sad::resource::Folder::find(sad::resource::Resource * r)
{
    PROFILER_EVENT;
    sad::Maybe<sad::String> result;
    for(sad::resource::ResourceIterator it = this->resourceListBegin(); 
        it != this->resourceListEnd(); 
        ++it)
    {
        if (it.value() == r)
        {
            result.setValue(it.key());
            return result;
        }
    }
    for(sad::resource::FolderIterator it = this->folderListBegin(); 
        it != this->folderListEnd(); 
        ++it)
    {
        result = it.value()->find(r);
        if (result.exists())
        {
            result.setValue(it.key() + "/" + result.value());
            return result;
        }
    }
    return result;
}

sad::resource::ResourceEntryList sad::resource::Folder::copyAndClear()
{
    PROFILER_EVENT;
    sad::resource::ResourceEntryList result;
    for(sad::resource::ResourceIterator it = this->resourceListBegin(); 
        it != this->resourceListEnd(); 
        ++it)
    {
        result << sad::resource::ResourceEntry(it.key(), it.value());
    }
    m_resources.clear();
    for(sad::resource::FolderIterator it = this->folderListBegin(); 
        it != this->folderListEnd(); 
        ++it)
    {
        sad::resource::ResourceEntryList tmp = it.value()->copyAndClear();
        sad::String prefix = it.key() + "/"; 
        for(size_t i = 0; i < tmp.size(); i++)
        {
            result <<  sad::resource::ResourceEntry(prefix + tmp[i].p1(), tmp[i].p2());
        }
    }
    return result;
}

void sad::resource::Folder::unloadResourcesFromGPU()
{
    PROFILER_EVENT;
    for(sad::resource::ResourceIterator it = this->resourceListBegin(); it != this->resourceListEnd(); ++it)
    {
        it.value()->unloadFromGPU();
    }

    for(sad::resource::FolderIterator it = this->folderListBegin(); it != this->folderListEnd(); ++it)
    {
        it.value()->unloadResourcesFromGPU();
    }
}

sad::resource::Folder * sad::resource::Folder::navigateParentFolder(
        const sad::String & path, 
        bool create,
        sad::String & name
)
{
    PROFILER_EVENT;
    if (path.size() == 0 )
        return nullptr;
    if (path.getOccurrences("/") > 1024)
        return nullptr;
    sad::Vector<sad::String> splitpath = path.split("/");
    if (splitpath.size() == 0)
        return nullptr;
    sad::resource::Folder * parent = this;
    for(int i = 0; i < (int)(splitpath.size()) - 1; i++)
    {
        if (parent->m_subfolders.contains(splitpath[i]) == false)
        {
            if (create)
            {
                parent->m_subfolders.insert(splitpath[i], new sad::resource::Folder());
            }
            else
            {
                return nullptr;
            }
        }
        parent = parent->m_subfolders[splitpath[i]];
    }
    name = splitpath[splitpath.count() - 1];
    return parent;
}
