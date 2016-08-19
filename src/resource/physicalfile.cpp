#include "resource/physicalfile.h"

#include "resource/tree.h"

#include "renderer.h"

#include <util/fs.h>

#include <algorithm>
#include <fstream>

sad::resource::PhysicalFile::PhysicalFile(const sad::String & name) 
: m_name(name), m_tree(NULL)
{
            
}


sad::resource::PhysicalFile::~PhysicalFile()
{

}


bool sad::resource::PhysicalFile::isAnonymous() const
{
    return m_name.length() != 0;	
}

const sad::String & sad::resource::PhysicalFile::name() const
{
    return m_name;	
}

void sad::resource::PhysicalFile::setName(const sad::String & name)
{
    m_name = name;
}

sad::Vector<sad::resource::Error*> sad::resource::PhysicalFile::load(sad::resource::Folder * parent)
{
    sad::Vector<sad::resource::Error*>  result;
    result << new sad::resource::FileLoadingNotImplemented(m_name);
    return result;
}

sad::Vector<sad::resource::Error*> sad::resource::PhysicalFile::reload()
{
    sad::Vector<sad::resource::Error*> errors;

    sad::resource::ResourceEntryList list;
    for(size_t i = 0; i < m_resources.size() && errors.size() == 0; i++)
    {
        sad::resource::Resource * res = m_tree->factory()->create(
            m_resources[i]->factoryName()
        );
        if (res)
        {
            res->setFactoryName(m_resources[i]->factoryName());
            sad::Maybe<sad::String> path = m_tree->root()->find(m_resources[i]); 
            if (path.exists())
            {
                bool ok = res->tryLoad(
                    *this, 
                    m_tree->renderer(), 
                    m_resources[i]->options(), 
                    m_resources[i]->shouldStoreLinks()
                );
                if (ok)
                {				
                    list << sad::resource::ResourceEntry(path.value(), res);
                }
                else
                {
                    errors << new sad::resource::ResourceLoadError(path.value());
                }
            }
            else
            {
                delete res;
                errors << new sad::resource::AnonymousResource(m_resources[i]->factoryName());
            }
        }
        else
        {
            errors << new sad::resource::UnregisteredResourceType(m_resources[i]->factoryName());
        }
    }

    if (errors.size() == 0)
    {
        for(size_t i = 0; i < m_resources.size(); i++)
        {
            m_tree->root()->replaceResource(list[i].p1(), list[i].p2());
            m_resources[i] = list[i].p2();
            m_resources[i]->setName(list[i].p1());
            list[i].p2()->setPhysicalFile(this);
        }
    }
    else
    {
        sad::resource::free(list);
    }
    return errors;
}

void sad::resource::PhysicalFile::add(sad::resource::Resource * r)
{
    if (r && std::find(m_resources.begin(), m_resources.end(), r) == m_resources.end())
    {
        m_resources << r;
    }	
}

void sad::resource::PhysicalFile::remove(sad::resource::Resource * r)
{
    m_resources.removeAll(r);
}

void sad::resource::PhysicalFile::setTree(sad::resource::Tree * tree)
{
    m_tree = tree;
}

sad::resource::Tree * sad::resource::PhysicalFile::tree() const
{
    return m_tree;
}

const sad::Vector<sad::resource::Resource*> & sad::resource::PhysicalFile::resources() const
{
    return m_resources;	
}

void sad::resource::PhysicalFile::replace(
    sad::resource::Resource * from, 
    sad::resource::Resource * to
)
{
    for(size_t i = 0; i < m_resources.size(); i++)
    {
        if (m_resources[i] == from)
        {
            m_resources[i] = to;
        }
    }
}

bool sad::resource::PhysicalFile::supportsLoadingFromTar7z() const
{
    return false;
}


sad::Maybe<sad::String> sad::resource::PhysicalFile::tryReadToString()
{
    sad::Maybe<sad::String> result;
    std::ifstream stream(m_name.c_str());
    if (stream.good())
    {
        std::string alldata(
            (std::istreambuf_iterator<char>(stream)), 
            std::istreambuf_iterator<char>()
        );
        result.setValue(alldata);
    }
    else
    {
        if (util::isAbsolutePath(m_name) == false)
        {
            sad::String path = util::concatPaths(m_tree->renderer()->executablePath(), m_name);
            stream.clear();
            stream.open(path.c_str());
            if (stream.good())
            {
                std::string alldata(
                    (std::istreambuf_iterator<char>(stream)), 
                     std::istreambuf_iterator<char>()
                    );
                result.setValue(alldata);
            }
        }
    }
    return result;
}

void sad::resource::PhysicalFile::replaceResources(
        const sad::resource::ResourceEntryList & resourcelist
)
{
    m_resources.clear();
    for(size_t i = 0 ; i < resourcelist.size(); i++)
    {
        m_resources << resourcelist[i].p2();
        resourcelist[i].p2()->setPhysicalFile(this);
    }
}

void sad::resource::PhysicalFile::createOldResourceList(
    sad::resource::ResourceEntryList & resources
)
{
    for(size_t i = 0; i < m_resources.size(); i++)
    {
        sad::Maybe<sad::String> name = this->tree()->root()->find(m_resources[i]);
        if (name.exists())
        {
            resources << sad::resource::ResourceEntry(name.value(), m_resources[i]);
        }
    }
}

void sad::resource::PhysicalFile::diffResourcesLists(
        const sad::resource::ResourceEntryList & oldlist,
        const sad::resource::ResourceEntryList & newlist,
        sad::resource::ResourceEntryList & tobeadded,
        sad::resource::ResourceEntryList & tobereplaced,
        sad::resource::ResourceEntryList & toberemoved
)
{
    sad::resource::ResourceEntryList noldlist = oldlist;
    for(size_t i = 0 ; i < newlist.size(); i++)
    {
        bool exists = false;
        for(size_t j = 0 ; j < noldlist.size() && !exists; j++)
        {
            if (newlist[i].p1() == noldlist[j].p1())
            {
                tobereplaced << newlist[i];
                exists = true;
                noldlist.removeAt(j);
            }
        }
        if (exists == false)
        {
            tobeadded << newlist[i];
        }
    }
    for(size_t i = 0; i < noldlist.size(); i++)
    {
        toberemoved << noldlist[i];
    }
}

void sad::resource::PhysicalFile::convertReferencedOptionsToBeRemovedToErrors(
    const sad::resource::ResourceEntryList & toberemoved,
    sad::Vector<sad::resource::Error *> & errors
)
{
    for(size_t i = 0 ; i < toberemoved.size(); i++)
    {
        if (toberemoved[i].p2()->referenced())
        {
            errors << new sad::resource::CannotDeleteReferencedResource(toberemoved[i].p1());
        }
    }
}

