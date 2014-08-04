#include "db/dbobjectfactory.h"

#include "3rdparty/picojson/getproperty.h"
#include "3rdparty/picojson/valuetotype.h"

#include "label.h"
#include "sprite2d.h"
#include "sprite3d.h"
#include "scene.h"

sad::db::ObjectFactory::AbstractDelegate::~AbstractDelegate()
{
	
}

sad::db::ObjectFactory::ObjectFactory()
{
	add<sad::Label>("sad::Label", sad::Label::basicSchema(), false);
	add<sad::Scene>("sad::Scene", sad::Scene::basicSchema(), false);
	add<sad::Sprite2D>("sad::Sprite2D", sad::Sprite2D::basicSchema(), false);
	add<sad::Sprite3D>("sad::Sprite3D", sad::Sprite3D::basicSchema(), false);

	// TODO: Add handling for sad::db::ExtensibleObject
}

void sad::db::ObjectFactory::add(
	const sad::String & name,
	sad::db::schema::Schema* schema, 
	bool own, 
	sad::db::ObjectFactory::AbstractDelegate * d
)
{	
	if (m_metadata_container.contains(name) == false && schema && d)
	{
		m_metadata_container.insert(name, new sad::db::ObjectFactory::Entry(own, schema, d) );
	}
}

sad::db::schema::Schema* sad::db::ObjectFactory::schema(const sad::String& name)
{
	sad::db::schema::Schema* result = NULL;
	if (m_metadata_container.contains(name))
	{
		result = m_metadata_container[name]->Schema;
	}
	return result;
}

sad::db::Object* sad::db::ObjectFactory::create(const sad::String& name)
{
	sad::db::Object* result = NULL;
	if (m_metadata_container.contains(name))
	{
		result = m_metadata_container[name]->Delegate->create();
	}
	return result;
}

sad::db::Object* sad::db::ObjectFactory::createFromEntry(const picojson::value & v)
{
	const picojson::value * type = picojson::get_property(v, "type");
	sad::db::Object*  result = NULL;
	if (type)
	{
		sad::Maybe<sad::String> maybetype = picojson::ValueToType<sad::String>::get(*type);
		if (maybetype.exists())
		{
			result = this->create(maybetype.value());
		}
	}
	return result;
}

sad::db::ObjectFactory::~ObjectFactory()
{
	
}

sad::db::ObjectFactory::Entry::~Entry()
{
	if (OwnsSchema)
	{
		delete Schema;
	}
	delete Delegate;
}
