#include "db/dbvariant.h"

sad::db::Variant::Variant() : 
m_object(NULL), 
m_is_sad_object(false), 
m_typename(""), 
m_copy(NULL),
m_delete(NULL),
m_save(NULL),
m_load(NULL)
{

}

sad::db::Variant::Variant(const sad::db::Variant & v)
{
	assign(v);
}

sad::db::Variant & sad::db::Variant::operator=(const sad::db::Variant  & v)
{
	release();
	assign(v);
	return *this;
}

sad::db::Variant::~Variant()
{
	release();
}


picojson::value sad::db::Variant::save() const
{
	picojson::value v;
	if (m_object)
	{
		v = m_save(m_object);
	}
	return v;
}

bool sad::db::Variant::load(const picojson::value & v)
{
	if (m_object)
	{
		return m_load(m_object, v);
	}
	return false;
}


void sad::db::Variant::release()
{
	if (m_object)
	{
		m_delete(m_object);
	}
}


void sad::db::Variant::assign(const sad::db::Variant & v)
{
	if (v.m_object)
	{
		m_object = (v.m_copy)(v.m_object);
		m_is_sad_object = v.m_is_sad_object;
		m_typename = v.m_typename;
		m_copy = v.m_copy;
		m_delete = v.m_delete;
		m_save = v.m_save;
		m_load = v.m_load;
	}
	else
	{
		m_object = NULL;
		m_is_sad_object = false;
		m_typename = "";
		m_copy = NULL;
		m_delete = NULL;
		m_save = NULL;
		m_load = NULL;
	}
}
