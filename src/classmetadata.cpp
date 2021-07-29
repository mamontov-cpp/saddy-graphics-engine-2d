#include <classmetadatacontainer.h>

void sad::ClassMetaData::setName(const sad::String & name)
{
    m_name = name;
}

const sad::String& sad::ClassMetaData::name() const
{
    return m_name;
}

bool sad::ClassMetaData::canBeCastedTo(const sad::String & name)
{
    if (name == "sad::db::Object")
    {
        return true;
    }
    if (m_name == name)
    {
        return true;
    }
    if (m_casts.contains(name))
    {
        return true;
    }
    bool result = false;
    for(size_t i = 0; i < m_ancestors.size(); i++) 
    {
        result = result || m_ancestors[i]->canBeCastedTo(name);
    }
    return result;
}


sad::Object * sad::ClassMetaData::castTo(sad::Object * o, const sad::String & name)
{
    if (m_name == name)
    {
        return o;
    }
    if (m_casts.contains(name))
    {
        return m_casts[name]->cast(o);
    }
    if (this->canBeCastedTo(name))
    {
        return o;
    }
    return nullptr;
}

void sad::ClassMetaData::addAncestor(const sad::String & name)
{
    bool created = false;
    sad::ClassMetaData * parent = sad::ClassMetaDataContainer::ref()->get(name, created);
    this->addAncestor(parent);
}

void sad::ClassMetaData::addAncestor(sad::ClassMetaData* ancestor)
{
    if (ancestor)
    {
        sad::ClassMetaData* parent = ancestor;
        m_ancestors.add(parent);
        for (CastFunctions::iterator it = parent->m_casts.begin(); it != parent->m_casts.end(); it++)
        {
            this->m_casts.insert(it.key(), it.value()->clone());
        }
    }
}


bool sad::ClassMetaData::canBeCastedFrom(const sad::String & name)
{
    bool created = false;
    ClassMetaData * metaDataToBeCastedTo = sad::ClassMetaDataContainer::ref()->get(name, created);
    bool result = metaDataToBeCastedTo->canBeCastedTo(m_name);
    return result;
}


unsigned int sad::ClassMetaData::privateIndex() const
{
    return m_private_index;
}


void sad::ClassMetaData::setPrivateIndex(unsigned int privateIndex)
{
    m_private_index = privateIndex;
}


