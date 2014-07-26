#include "db/dbstoredpropertyfactory.h"

#include "sadcolor.h"
#include "sadrect.h"
#include "sadpoint.h"
#include "sadsize.h"

sad::db::StoredPropertyFactory::AbstractDelegate::~AbstractDelegate()
{
	
}

sad::db::StoredPropertyFactory::StoredPropertyFactory()
{
	add<char>("char");
	add<unsigned char>("unsigned char");
	add<short>("short");
	add<unsigned short>("unsigned short");
	add<int>("int");
	add<unsigned int>("unsigned int");
	add<long>("long");
	add<unsigned long>("unsigned long");
	add<long long>("long long");
	add<unsigned long long>("unsigned long long");
	add<float>("float");
	add<double>("double");
	add<sad::String>("sad::String");
	add<std::string>("std::string");
	add<sad::Color>("sad::Color");
	add<sad::AColor>("sad::AColor");
	add<sad::Point2D>("sad::Point2D");
	add<sad::Point2I>("sad::Point2I");
	add<sad::Point3D>("sad::Point3D");
	add<sad::Point3I>("sad::Point3I");
	add<sad::Rect2D>("sad::Rect2D");
	add<sad::Rect2I>("sad::Rect2I");
	add<sad::Size2D>("sad::Size2D");
	add<sad::Size2I>("sad::Size2I");
}

sad::db::StoredPropertyFactory::~StoredPropertyFactory()
{
	
}

void sad::db::StoredPropertyFactory::add(
	const sad::String & name, 
	sad::db::StoredPropertyFactory::AbstractDelegate * d
)
{
	if (m_delegates.contains(name))
	{
		delete m_delegates[name];
		m_delegates[name] = d;
	}
	else
	{
		m_delegates.insert(name, d);			
	}
}

 sad::db::Property *  sad::db::StoredPropertyFactory::create(const sad::String & name) const
{
	sad::db::Property* p = NULL;
	if (m_delegates.contains(name))
	{
		p = m_delegates[name]->create();
	}
	return p;
}
