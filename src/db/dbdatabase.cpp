#include "db/dbdatabase.h"

#include "util/fs.h"

#include "renderer.h"

#include <fstream>

sad::db::Database::Database() : m_max_major_id(0), m_renderer(NULL)
{
	m_factory = new sad::db::ObjectFactory();
	m_prop_factory = new sad::db::StoredPropertyFactory();
}

sad::db::Database::~Database()
{
	delete m_factory;
	delete m_prop_factory;
	for(sad::Hash<sad::String, sad::db::Table*>::iterator it = m_names_to_tables.begin();
		it != m_names_to_tables.end();
		++it
	   )
	{
		delete it.value();
	}
}

void sad::db::Database::save(sad::String & output)
{
	picojson::value resultvalue(picojson::object_type, false);

	picojson::value propertiesresult(picojson::object_type, false);
	for(sad::PtrHash<sad::String, sad::db::Property>::iterator it = m_properties.begin();
		it != m_properties.end();
		++it
	   )
	{
		picojson::value prop(picojson::object_type, false);
		prop.insert("type", it.value()->serializableType());
		sad::db::Variant tmp;
		it.value()->get(NULL, tmp);
		prop.insert("value", tmp.save());
		propertiesresult.insert(it.key(), prop);
	}
	resultvalue.insert("properties", propertiesresult);

	picojson::value tablesresult(picojson::object_type, false);
	for(sad::Hash<sad::String, sad::db::Table*>::iterator it = m_names_to_tables.begin();
		it!= m_names_to_tables.end();
		++it
	   )
	{
		picojson::value tablevalue(picojson::array_type, false);
		it.value()->save(tablevalue);
		tablesresult.insert(it.key(), tablevalue);
	}
	resultvalue.insert("tables", tablesresult);

	output = resultvalue.serialize(0);
}

void sad::db::Database::saveToFile(const sad::String& s)
{
	std::ofstream file(s.c_str(), std::ofstream::out);
	if (file.good())
	{
		sad::String output;
		this->save(output);
		file << output;
		file.close();
	}
}

bool sad::db::Database::load(const sad::String& text)
{
	bool result = false;
	if (text.consistsOfWhitespaceCharacters() == false)
	{
		picojson::value root = picojson::parse_string(text);
		if (root.is<picojson::object>())
		{
			picojson::value const* propertiesroot = picojson::get_property(root, "properties");
			picojson::value const* tablesroot = picojson::get_property(root, "tables");
			if (propertiesroot && tablesroot)
			{
				if (propertiesroot->is<picojson::object>() && tablesroot->is<picojson::object>())
				{
					result = this->loadPropertiesAndTables(
						propertiesroot->get<picojson::object>(),
						tablesroot->get<picojson::object>()
					);
				}
			}
		}
	}
	return result;
}

bool sad::db::Database::loadFromFile(const sad::String& name, sad::Renderer * r)
{
	bool loadingresult = false;
	sad::Maybe<sad::String> result;
	std::ifstream stream(name.c_str());
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
		if (util::isAbsolutePath(name) == false)
		{
			if (r == NULL)
			{
				r = this->renderer();
			}
			sad::String path = util::concatPaths(r->executablePath(), name);
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

	if (result.exists())
	{
		loadingresult = this->load(result.value());
	}

	return loadingresult;
}

void sad::db::Database::addProperty(const sad::String & name, sad::db::Property * p)
{
	assert( p );

	if (m_properties.contains(name))
	{
		delete m_properties[name];
		m_properties[name] = p;
	}
	else
	{
		m_properties.insert(name, p);
	}
}

void sad::db::Database::removeProperty(const sad::String & name)
{
	if (m_properties.contains(name))
	{
		delete m_properties[name];
		m_properties.remove(name);
	}
}

sad::db::Property* sad::db::Database::propertyByName(const sad::String & name) const
{
	sad::db::Property* p = NULL;
	if (m_properties.contains(name))
	{
		p = m_properties[name];
	}
	return p;
}

sad::db::StoredPropertyFactory* sad::db::Database::storedPropertyFactory() const
{
	return m_prop_factory;
}

void sad::db::Database::setStoredPropertyFactory(sad::db::StoredPropertyFactory * f)
{
	assert( f );
	delete m_prop_factory;
	m_prop_factory = f;
}

bool sad::db::Database::addTable(const sad::String& name, db::Table* table)
{
	assert( table );

	bool result = false;
	if (m_names_to_tables.contains(name) == false)
	{
		result = true;
		table->setDatabase(this);
		m_names_to_tables.insert(name, table);
	}
	return result;		
}

void sad::db::Database::removeTable(const sad::String& name)
{
	if (m_names_to_tables.contains(name))
	{
		delete m_names_to_tables[name];
		m_names_to_tables.remove(name);
	}
}

sad::db::Table* sad::db::Database::table(const sad::String& name)
{
	sad::db::Table* result = NULL;	
	if (m_names_to_tables.contains(name))
	{
		result = m_names_to_tables[name];
	}
	return result;
}

unsigned long long sad::db::Database::uniqueMajorId(sad::db::Table * t)
{
	m_majorid_to_table.insert(m_max_major_id, t);
	return m_max_major_id++;	
}

sad::Vector<sad::db::Object *> sad::db::Database::queryByName(const sad::String & name) const
{
	sad::Vector<sad::db::Object *> result;
	for(sad::Hash<sad::String, sad::db::Table*>::const_iterator it = m_names_to_tables.const_begin();
		it != m_names_to_tables.const_end();
		++it)
	{
		result << it.value()->queryByName(name);
	}
	return result;
}

sad::Vector<sad::db::Object *> sad::db::Database::queryByMinorId(unsigned long long id) const
{
	sad::Vector<sad::db::Object *> result;
	for(sad::Hash<sad::String, sad::db::Table*>::const_iterator it = m_names_to_tables.const_begin();
		it != m_names_to_tables.const_end();
		++it)
	{
		sad::db::Object * tmp = it.value()->queryByMinorId(id);
		if (tmp)
		{
			result << tmp;
		}
	}
	return result;
}

sad::db::Object* sad::db::Database::queryByMajorId(unsigned long long id) const
{
	sad::db::Object* result = NULL;
	if (m_majorid_to_table.contains(id))
	{
		result = m_majorid_to_table[id]->queryByMajorId(id);
	}
	return result;
}

void sad::db::Database::getTables(sad::Vector<sad::Pair<sad::String, sad::db::Table*> > & tables) const
{
	for(sad::Hash<sad::String, sad::db::Table*>::const_iterator it = m_names_to_tables.const_begin();
		it != m_names_to_tables.const_end();
		++it)
	{
		tables << sad::Pair<sad::String, sad::db::Table*>(it.key(), it.value());
	}
}

sad::db::ObjectFactory* sad::db::Database::factory()
{
	return m_factory;	
}

void sad::db::Database::setFactory(sad::db::ObjectFactory* f)
{
	assert( f );
	delete m_factory;
	m_factory = f;
}

void sad::db::Database::trySetMaxMajorId(unsigned long long v, sad::db::Table * t)
{
	if (v > m_max_major_id)
	{
		m_max_major_id = v + 1;
	}
	m_majorid_to_table.insert(v, t);
}

void sad::db::Database::removeMajorId(unsigned long long v)
{
	m_majorid_to_table.remove(v);
}

void sad::db::Database::setRenderer(sad::Renderer * r)
{
	m_renderer = r;
}

sad::Renderer* sad::db::Database::renderer() const
{
	if (!m_renderer)
	{
		return sad::Renderer::ref();
	}
	return m_renderer;
}

// Protected methods

bool sad::db::Database::loadPropertiesAndTables(
	const picojson::object & properties, 
	const picojson::object & tables
)
{
	bool result = true;
	sad::Hash<unsigned long long, sad::db::Table*> oldmajoridtotable = m_majorid_to_table;
	unsigned long long oldmaxmajorid = m_max_major_id;
	sad::Hash<sad::String, sad::db::Property*> newproperties;
	for(picojson::object::const_iterator it = properties.begin();
		it != properties.end();
		++it)
	{
		bool deserialized =  false;
		if (it->second.is<picojson::object>())
		{
			picojson::object o = it->second.get<picojson::object>();
			const picojson::value* maybetype = NULL;
			if (o.find("type") != o.end())
			{
				maybetype = &(o["type"]);
			}			
			const picojson::value* maybevalue = NULL;
			if (o.find("value") != o.end())
			{
				maybevalue = &(o["value"]);
			}

			if (maybetype && maybevalue)
			{
				if (maybetype->is<std::string>())
				{
					sad::db::Property * p = m_prop_factory->create(maybetype->get<std::string>());
					if (p)
					{
						sad::db::Variant value;
						p->get(NULL, value);
						deserialized = value.load(*maybevalue);
						if (deserialized)
						{
							deserialized = p->set(NULL, value);
							if (deserialized)
							{
								newproperties.insert(it->first, p);
							}
						}
						if (deserialized == false)
						{
							delete p;
						}
					}
				}
			}
		}
		
		result = result && deserialized;
	}

	sad::Hash<sad::String, sad::db::Table*> newtables;
	for(picojson::object::const_iterator it = tables.begin();
		it != tables.end();
		++it)
	{
		sad::db::Table* t = new sad::db::Table();
		t->setDatabase(this);
		bool deserialized = t->load(it->second, m_factory);
		if (deserialized)
		{
			newtables.insert(it->first, t);
		}
		result = result && deserialized;
	}

	if (result)
	{
		// Remove old keys
		for(sad::Hash<unsigned long long, sad::db::Table*>::iterator it = oldmajoridtotable.begin();
			it != oldmajoridtotable.end();
			++it)
		{
			m_majorid_to_table.remove(it.key());
		}
		
		// Reset old properties
		for(sad::Hash<sad::String, sad::db::Property*>::iterator it = m_properties.begin();
			it != m_properties.end();
			++it)
		{
			delete it.value();
		}
		m_properties.clear();
		for(sad::Hash<sad::String, sad::db::Property*>::iterator it = newproperties.begin();
			it != newproperties.end();
			++it)
		{
			m_properties.insert(it.key(), it.value());
		}

		// Reset old tables
		for(sad::Hash<sad::String, sad::db::Table*>::iterator it = m_names_to_tables.begin();
			it != m_names_to_tables.end();
			++it)
		{
			delete it.value();
		}
		m_names_to_tables.clear();
		m_names_to_tables = newtables;
	}
	else
	{
		// Restore old values
		m_majorid_to_table = oldmajoridtotable;
		m_max_major_id = oldmaxmajorid;

		// Free allocated resources
		for(sad::Hash<sad::String, sad::db::Property*>::iterator it = newproperties.begin();
			it != newproperties.end();
			++it)
		{
			delete it.value();
		}

		for(sad::Hash<sad::String, sad::db::Table*>::iterator it = newtables.begin();
			it != newtables.end();
			++it)
		{
			delete it.value();
		}

	}
	return result;
}

