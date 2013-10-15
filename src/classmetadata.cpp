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
	if (m_name == name)
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

void sad::ClassMetaData::addAncestor(const sad::String & name)
{
	bool created = false;
	m_ancestors.add(sad::ClassMetaDataContainer::ref()->get(name, created));
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


