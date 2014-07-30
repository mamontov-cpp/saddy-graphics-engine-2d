#include "db/custom/customschemafile.h"

#include "util/fs.h"

#include "renderer.h"

#include <fstream>
#include <cassert>

sad::db::custom::SchemaFile::SchemaFile(const sad::String& name) 
: sad::resource::PhysicalFile(name)
{
	m_factory = new sad::db::StoredPropertyFactory();
}

sad::db::custom::SchemaFile::~SchemaFile()
{
	if (m_factory)
	{
		delete m_factory;
	}
}

void sad::db::custom::SchemaFile::setFactory(sad::db::StoredPropertyFactory * factory)
{
	assert( m_factory );
	m_factory = factory->clone();
}

sad::db::StoredPropertyFactory * sad::db::custom::SchemaFile::factory() const
{
	return m_factory;
}


void sad::db::custom::SchemaFile::tryParsePartial(
		sad::db::custom::SchemaFile::parse_result & result,
		sad::Vector<sad::resource::Error *> & errors 
	)
{
	sad::Maybe<sad::String> maybecontent = this->tryReadToString();
	if (maybecontent.exists())
	{
		if (maybecontent.value().consistsOfWhitespaceCharacters())
		{
			errors << new sad::resource::JSONParseError();
			return;
		}
		picojson::value rootvalue = picojson::parse_string(maybecontent.value());
		if (picojson::get_last_error().size() == 0)
		{
			if (rootvalue.is<picojson::array>())
			{
				picojson::array parse_item_list = rootvalue.get<picojson::array>();
				for(size_t i = 0; i < parse_item_list.size(); i++)
				{
					sad::db::custom::SchemaFile::parse_result_entry entry;
					if (this->tryParseEntry(entry, errors, parse_item_list[i]))
					{
						// Validate whether we have a resource with name
						if (this->validateTreeReferences(entry, errors))
						{
							result.push_back(entry);
						}
					}
				}
			}
			else
			{
				errors << new sad::resource::MalformedResourceEntry(rootvalue);
			}
		}
		else
		{
			errors << new sad::resource::JSONParseError();
		}
	}
	else
	{
		errors << new sad::resource::FileLoadError(m_name);
	}

	// If no errors, check schemas for unique names
	if (errors.size() == 0)
	{
		sad::Hash<sad::String, char> names;
		for(size_t i = 0; i < result.size(); i++)
		{
			if (names.contains(result[i]._1()))
			{
				names.insert(result[i]._1(), 1);
			}
			else
			{
				errors << new sad::resource::ResourceAlreadyExists(result[i]._1());
			}
		}
	}
}

bool sad::db::custom::SchemaFile::tryParseEntry(
		sad::db::custom::SchemaFile::parse_result_entry & parse_result,
		sad::Vector<sad::resource::Error *> & errors,
		const picojson::value & v
)
{
	bool result = false;
	if (v.is<picojson::object>())
	{
		sad::Maybe<sad::String> maybename = picojson::to_type<sad::String>(
				picojson::get_property(v, "name")
		);
		sad::Maybe<sad::String> mayberesourcename = picojson::to_type<sad::String>(
				picojson::get_property(v, "resource")
		);
		picojson::value const * maybeschema = picojson::get_property(v, "schema");
		if (maybeschema && maybename.exists() && mayberesourcename.exists())
		{
			if (maybeschema->is<picojson::object>())
			{
				parse_result.set1(maybename.value());
				parse_result.set2(mayberesourcename.value());
				sad::Vector< sad::Pair<sad::String, sad::String> > proplist;
				bool loadresult = true;
				picojson::object o = maybeschema->get<picojson::object>();
				for(picojson::object::iterator it = o.begin(); it != o.end(); ++it)
				{
					sad::Maybe<sad::String> maybeproptype = picojson::to_type<sad::String>(&(it->second));
					if (maybeproptype.exists())
					{
						bool propresult = m_factory->canCreate(maybeproptype.value());
						if (propresult)
						{
							proplist.push_back(sad::Pair<sad::String, sad::String>(
								it->first, maybeproptype.value()
							));
						}
						else
						{
							errors << new sad::resource::MalformedResourceEntry(it->second);
							loadresult = false;
						}
					}
					else
					{
						errors << new sad::resource::MalformedResourceEntry(it->second);
						loadresult = false;
					}
				}
				if (loadresult)
				{
					result = true;
					parse_result.set3(proplist);
				}
			}
			else
			{
				errors << new sad::resource::MalformedResourceEntry(v);
			}
		}
		else
		{
			errors << new sad::resource::MalformedResourceEntry(v);
		}
	}
	else
	{
		errors << new sad::resource::MalformedResourceEntry(v);
	}
	return result;
}

bool sad::db::custom::SchemaFile::validateTreeReferences(
		sad::db::custom::SchemaFile::parse_result_entry & parse_result,
		sad::Vector<sad::resource::Error *> & errors
)
{
	bool result = true;
	sad::resource::Resource * resource = this->tree()->root()->resource(parse_result._2());
	if (resource)
	{
		if (resource->metaData()->name() != "sad::Sprite2D::Options")
		{
			result = true;
		}
		else
		{
			errors << new sad::resource::MissingResource(parse_result._2());
		}
	}
	else
	{
		errors << new sad::resource::MissingResource(parse_result._2());
	}
	return result;
}

sad::Maybe<sad::String> sad::db::custom::SchemaFile::tryReadToString()
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
