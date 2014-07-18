#include "db/schema/schema.h"

sad::db::schema::Schema::Schema(sad::db::schema::Schema* parent) : m_parent(parent)
{

}

sad::db::schema::Schema::~Schema()
{

}

bool sad::db::schema::Schema::add(const sad::String& s, sad::db::Property* prop)
{
	bool ok = false;
	if (this->get(s) == NULL)
	{
		m_properties.insert(s, prop);
		ok = true;
	}
	return ok;
}

sad::db::Property* sad::db::schema::Schema::get(const sad::String& s) const
{
	sad::db::Property * result = NULL;
	if (this->parent())
	{
		result = this->parent()->get(s);
	}

	if (result == NULL && m_properties.contains(s))
	{
		result = m_properties[s];
	}
	return result;
}

bool sad::db::schema::Schema::check(const picojson::value& v)
{
	if (this->parent())
	{
		if (this->parent()->check(v) == false)
		{
			return false;
		}
	}
	bool success = true;
	for(sad::PtrHash<sad::String, sad::db::Property>::iterator it = m_properties.begin();
		it != m_properties.end();
		it++)
	{
		success = success && it.value()->check(it.key(), v);
	}
	return success;
}

bool sad::db::schema::Schema::load(sad::db::Object * o, const picojson::value& v)
{
	return false;
}

picojson::value sad::db::schema::Schema::save(sad::db::Object * linked)
{
	return picojson::value();
}

sad::db::schema::Schema* sad::db::schema::Schema::parent() const
{
	return m_parent;
}

void sad::db::schema::Schema::setParent(sad::db::schema::Schema* parent)
{
	m_parent = parent;
}

