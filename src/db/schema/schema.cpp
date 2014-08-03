#include "db/schema/schema.h"

sad::db::schema::Schema::Schema(sad::db::schema::Schema* parent) 
{
	if (parent)
	{
		addParent(parent);
	}
}

sad::db::schema::Schema::~Schema()
{

}

bool sad::db::schema::Schema::add(const sad::String& s, sad::db::Property* prop)
{
	bool ok = false;
	if (this->getProperty(s) == NULL)
	{
		m_properties.insert(s, prop);
		ok = true;
	}
	return ok;
}

sad::db::Property* sad::db::schema::Schema::getProperty(const sad::String& s) const
{
	sad::db::Property * result = NULL;
	if (m_parent.size())
	{
		for(int i = 0; i < m_parent.size() && result == NULL; i++)
		{
			result = m_parent[i]->getProperty(s);
		}
	}

	if (result == NULL && m_properties.contains(s))
	{
		result = m_properties[s];
	}
	return result;
}

bool sad::db::schema::Schema::check(const picojson::value& v)
{
	if (m_parent.size()) 
	{
		for(size_t i = 0; i < m_parent.size(); i++)
		{
			if (m_parent[i]->check(v) == false)
			{
				return false;
			}
		}
	}
	bool success = true;
	for(sad::PtrHash<sad::String, sad::db::Property>::iterator it = m_properties.begin();
		it != m_properties.end();
		++it)
	{
		success = success && it.value()->check(it.key(), v);
	}
	return success;
}

bool sad::db::schema::Schema::load(sad::db::Object * o, const picojson::value& v)
{
	if (!o || v.is<picojson::object>() == false)
	{
		return false;
	}
	bool result = true;
	for(size_t i = 0; i < m_parent.size() && result; i++)
	{
		result = result && m_parent[i]->load(o, v);
	}
	if (!result)
	{
		return result;
	}
	for(sad::PtrHash<sad::String, sad::db::Property>::iterator it = m_properties.begin();
		(it != m_properties.end()) && result;
		++it)
	{
		const picojson::value * prop = picojson::get_property(v, it.key());
		if (prop)
		{
			sad::db::Variant tmp;
			it.value()->get(o, tmp);
			result = result && tmp.load(*prop);
			if (result)
			{
				result = result && it.value()->set(o, tmp);
			}
		}
		else
		{
			result = false;
		}
	}

	return result;
}

void sad::db::schema::Schema::save(sad::db::Object * linked, picojson::value & v)
{
	if (!linked)
	{
		return;
	}
	if (v.is<picojson::object>() == false)
	{
		v = picojson::object();
	}
	else
	{
		const picojson::object& vo = v.get<picojson::object>();
		if (vo.find("type") == vo.end())
		{
			v.insert("type", linked->serializableName());
		}
	}
	for(size_t i = 0; i < m_parent.size(); i++)
	{
		m_parent[i]->save(linked, v);
	}
	
	for(sad::PtrHash<sad::String, sad::db::Property>::iterator it = m_properties.begin();
		it != m_properties.end();
		++it)
	{
		sad::db::Variant tmp;
		it.value()->get(linked, tmp);
		v.insert(it.key(), tmp.save());
	}
	
}

const sad::Vector<sad::db::schema::Schema*>& sad::db::schema::Schema::parent() const
{
	return m_parent;
}

void sad::db::schema::Schema::addParent(sad::db::schema::Schema* parent)
{
	m_parent << parent;
}

