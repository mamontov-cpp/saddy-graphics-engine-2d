#include <classmetadatacontainer.h>

void sad::ClassMetaData::setName(const hst::string & name)
{
	m_name = name;
}

const hst::string& sad::ClassMetaData::name() const
{
	return m_name;
}

bool sad::ClassMetaData::canBeCastedTo(const hst::string & name)
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

void sad::ClassMetaData::addAncestor(const hst::string & name)
{
	bool created = false;
	m_ancestors.add(sad::ClassMetaDataContainer::ref()->get(name, created));
}


bool sad::ClassMetaData::canBeCastedFrom(const hst::string & name)
{
	bool created = false;
	ClassMetaData * metaDataToBeCastedTo = sad::ClassMetaDataContainer::ref()->get(name, created);
	bool result = metaDataToBeCastedTo->canBeCastedTo(m_name);
	return result;
}
