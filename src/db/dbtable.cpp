#include "db/dbtable.h"
#include "db/dbdatabase.h"
#include "db/dbobjectfactory.h"


sad::db::Table::Table() : m_max_minor_id(1)
{
	
}

sad::db::Table::~Table() 
{
	for(sad::Hash<sad::db::Object*, bool>::iterator it = m_owns_objects.begin(); 
		it != m_owns_objects.end();
		++it)
	{
		if (it.value())
		{
			delete it.key();
		}
	}
}

void sad::db::Table::add(sad::db::Object* a, bool own)
{
	assert(a);
	if (a->MajorId > 0)
	{
		sad::db::Object * old;
		if (a->MinorId > 0)
		{
			old = queryByID(a->MajorId, a->MinorId);
			if (old)
			{
				remove(old);
			}
		}
		old = queryByMajorId(a->MajorId);
		if (old)
		{
			remove(old);
		}
	}
	// Create major id if needed
	if (a->MajorId == 0)
	{
		unsigned int majorid = 1;
		if (database())
		{
			majorid = database()->uniqueMajorId(this);
		}
		a->MajorId = majorid;
	}

	if (a->MinorId == 0)
	{
		a->MinorId = this->m_max_minor_id++;
	}

	m_owns_objects.insert(a, own);

	if (a->Name.size() != 0)
	{
		if (m_object_by_name.contains(a->Name) == false)
		{
			m_object_by_name.insert(a->Name, sad::Vector<sad::db::Object*>());
			m_object_by_name[a->Name].push_back(a);
		}
		else
		{
			sad::Vector<sad::db::Object*> & list = m_object_by_name[a->Name];
			if (std::find(list.begin(), list.end(), a) == list.end())
			{
				list.push_back(a);
			}
		}
	}

	m_objects_by_minorid.insert(a->MinorId, a);
	m_objects_by_majorid.insert(a->MajorId, a);	
	a->setTable(this);
}

void sad::db::Table::remove(sad::db::Object* a)
{
	if (a)
	{
		if (a->Name.size() != 0)
		{
			if (m_object_by_name.contains(a->Name) == false)
			{
				sad::Vector<sad::db::Object*> & list = m_object_by_name[a->Name];
				sad::Vector<sad::db::Object*>::iterator pos =  std::find(list.begin(), list.end(), a);
				list.erase(pos);
			}
		}

		if (a->MajorId > 0 && m_objects_by_majorid.contains(a->MajorId))
		{
			m_objects_by_majorid.remove(a->MajorId);
		}

		if (a->MinorId > 0 && m_objects_by_minorid.contains(a->MinorId))
		{
			m_objects_by_minorid.remove(a->MinorId);
		}

		if (m_owns_objects.contains(a))
		{
			if (m_owns_objects[a])
			{
				delete a;
			}
			else
			{
				a->setTable(NULL);
			}
			m_owns_objects.remove(a);
		}
	}
}

sad::db::Object* sad::db::Table::queryByID(unsigned long long major_id, unsigned long long minor_id)
{
	sad::db::Object* result = NULL;
	if (m_objects_by_majorid.contains(major_id) && m_objects_by_minorid.contains(minor_id))
	{
		sad::db::Object*  f1 = m_objects_by_majorid[major_id];
		sad::db::Object * f2 = m_objects_by_minorid[minor_id];
		if (f1 == f2)
		{
			result = f1;
		}
	}
	return result;	
}

sad::Vector<sad::db::Object*> sad::db::Table::queryByName(const sad::String& name)
{
	sad::Vector<sad::db::Object*> result;
	if (m_object_by_name.contains(name))
	{
		result = m_object_by_name[name];
	}
	return result;	
}

sad::db::Object* sad::db::Table::queryByMajorId(unsigned long long major_id)
{
	sad::db::Object* result = NULL;
	if (m_objects_by_majorid.contains(major_id))
	{
		result = m_objects_by_majorid[major_id];
	}
	return result;	
}

bool sad::db::Table::load(const picojson::value & v, sad::db::ObjectFactory * factory, bool own)
{
	bool result = false;
	if (v.is<picojson::array>())
	{
		picojson::array entries = v.get<picojson::array>();
		sad::Vector<sad::db::Object*> buffer;
		bool ok = true;
		// Load items to buffer
		for(size_t i = 0; i < entries.size() && ok; i++)
		{
			sad::db::Object * tmp = factory->createFromEntry(entries[i]);
			if (!tmp)
			{
				ok = false;
			}
			else
			{
				ok = ok && tmp->load(entries[i]);
				if (!ok)
				{
					delete  tmp;
				}
			}
			if (ok)
			{
				buffer << tmp;
			}
		}
		// Insert buffer to table, otherwise delete buffer
		if (ok)
		{
			for(size_t i = 0; i < buffer.size(); i++)
			{
				add(buffer[i], own);
			}
		}
		else
		{
			for(size_t i = 0; i < buffer.size(); i++)
			{
				delete buffer[i];
			}
		}
		result = ok;
	}
	return result;
}

void sad::db::Table::save(picojson::value & v)
{
	if (v.is<picojson::array>() == false)
	{
		v = picojson::value(picojson::array_type, false);
	}
	sad::Vector<sad::db::Object*> result;
	this->objects(result);
	for(size_t i = 0; i < result.size(); i++)
	{
		picojson::value tmp(picojson::object_type, false);
		result[i]->save(tmp);
		v.push_back(tmp);
	}
}


sad::db::Database* sad::db::Table::database() const
{
	return m_database;
}

void sad::db::Table::setDatabase(sad::db::Database* d)
{
	m_database = d;
}


void sad::db::Table::objects(sad::Vector<sad::db::Object*> & o)
{
	for(sad::Hash<sad::db::Object*, bool>::iterator it = m_owns_objects.begin();
		it != m_owns_objects.end();
		++it)
	{
		o << it.key();
	}
}

