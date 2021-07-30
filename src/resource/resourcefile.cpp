#include "resource/resourcefile.h"

#include "resource/tree.h"

#include "renderer.h"

#include <util/fs.h>

#include <algorithm>
#include <fstream>

#include "db/dbtypename.h"

#define TAR7Z_SADDY

#include <3rdparty/tar7z/include/archive.h>


// ================================== sad::resource::ResourceFileIdentifier ==================================

void sad::resource::ResourceFileIdentifier::parse(const sad::String& string, sad::resource::ResourceFileIdentifier& ri)
{
    const int tar7zlength = 6;
    if (string.startsWith("tar7z:", tar7zlength))
    {
        ri.Type = sad::resource::ResourceFileType::RFT_TAR7Z_INNER_FILE;
        ri.Valid  = false;
        const char* endstring = string.c_str() + string.size();
        const char* numbuf = string.c_str() + tar7zlength; 
        const char* endbuf = strchr(numbuf, ':');
        if (endbuf)
        {
            endbuf--;
            unsigned int archive_name_size;
            if (sad::String::parseUInt(numbuf, endbuf, &archive_name_size))
            {
                endbuf += 4;
                if ((endbuf <= endstring) && ((endbuf + archive_name_size) <= endstring))
                {
                    sad::String& archive_name = ri.ArchiveName;
                    archive_name.replace(archive_name.begin(), archive_name.end(), endbuf, archive_name_size);
                    endbuf += archive_name_size;
                    ++endbuf;
                    if (endbuf < endstring)
                    {
                        ri.Valid = true;
                        ri.FileName = endbuf;
                    }
                }
            }
        }
    }
    else
    {
        ri.Valid = true;
        ri.Type = sad::resource::ResourceFileType::RFT_FILE;
        ri.FileName = string;
    }
}

// ================================== sad::resource::ResourceFile ==================================


sad::resource::ResourceFile::ResourceFile(const sad::String & name) 
: m_name(name), m_tree(nullptr)
{
    if (m_name.size())
    {
        sad::resource::ResourceFileIdentifier::parse(m_name, m_type);         
    }
}


sad::resource::ResourceFile::~ResourceFile()
{

}


bool sad::resource::ResourceFile::isAnonymous() const
{
    return m_name.length() != 0;    
}

const sad::String & sad::resource::ResourceFile::name() const
{
    return m_name;  
}

void sad::resource::ResourceFile::setName(const sad::String & name)
{
    m_name = name;
    sad::resource::ResourceFileIdentifier::parse(m_name, m_type);
}

sad::Vector<sad::resource::Error*> sad::resource::ResourceFile::load(sad::resource::Folder * parent)
{
    sad::Vector<sad::resource::Error*>  result;
    result << new sad::resource::FileLoadingNotImplemented(m_name);
    return result;
}

sad::Vector<sad::resource::Error*> sad::resource::ResourceFile::reload()
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
                picojson::value opts = m_resources[i]->options();
                opts.insert("force_archive_reload", picojson::value(true));
                bool ok = res->tryLoad(
                    *this, 
                    m_tree->renderer(), 
                    opts, 
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

sad::Maybe<sad::String> sad::resource::ResourceFile::tryLoad(sad::resource::Tree* tree)
{
    sad::resource::Folder* folder = tree->root();
    sad::Vector<sad::resource::Error*> data = this->load(folder);
    return sad::resource::errorsToString(data);    
}

sad::Maybe<sad::String> sad::resource::ResourceFile::tryReload()
{
    sad::Vector<sad::resource::Error*> data = this->reload();
    return sad::resource::errorsToString(data);    
}

void sad::resource::ResourceFile::add(sad::resource::Resource * r)
{
    if (r && std::find(m_resources.begin(), m_resources.end(), r) == m_resources.end())
    {
        m_resources << r;
    }   
}

void sad::resource::ResourceFile::remove(sad::resource::Resource * r)
{
    m_resources.removeAll(r);
}

void sad::resource::ResourceFile::setTree(sad::resource::Tree * tree)
{
    m_tree = tree;
}

sad::resource::Tree * sad::resource::ResourceFile::tree() const
{
    return m_tree;
}

const sad::Vector<sad::resource::Resource*> & sad::resource::ResourceFile::resources() const
{
    return m_resources; 
}

void sad::resource::ResourceFile::replace(
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

const sad::resource::ResourceFileIdentifier& sad::resource::ResourceFile::rfi() const
{
    return m_type;
}

bool sad::resource::ResourceFile::supportsLoadingFromTar7z() const
{
    return false;
}


sad::Maybe<sad::String> sad::resource::ResourceFile::tryReadToString(bool force_reload) const
{
    sad::Maybe<sad::String> result;
    if (m_type.Valid)
    {
        if (m_type.Type == sad::resource::ResourceFileType::RFT_FILE)
        {
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
        }

        if (m_type.Type == sad::resource::ResourceFileType::RFT_TAR7Z_INNER_FILE)
        {
            tar7z::Entry* e = this->tree()->archiveEntry(m_type.ArchiveName, m_type.FileName, force_reload);
            if (e)
            {
                result.setValue(sad::String(e->contents(), e->Size));
            }
        }
    }
    return result;
}

void sad::resource::ResourceFile::replaceResources(
        const sad::resource::ResourceEntryList & resource_list
)
{
    m_resources.clear();
    for(size_t i = 0 ; i < resource_list.size(); i++)
    {
        m_resources << resource_list[i].p2();
        resource_list[i].p2()->setPhysicalFile(this);
    }
}

void sad::resource::ResourceFile::createOldResourceList(
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

void sad::resource::ResourceFile::diffResourcesLists(
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

void sad::resource::ResourceFile::convertReferencedOptionsToBeRemovedToErrors(
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

DECLARE_COMMON_TYPE(sad::resource::ResourceFile);
