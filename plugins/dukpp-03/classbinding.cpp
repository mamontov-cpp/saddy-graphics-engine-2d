#include "dukpp-03/classbinding.h"
#include "dukpp-03/getter.h"
#include "dukpp-03/setter.h"


sad::dukpp03::ClassBinding::ClassBinding()
{
    
}

sad::dukpp03::ClassBinding::~ClassBinding()
{
    
}

void sad::dukpp03::ClassBinding::registerSchema(const sad::db::schema::Schema* schema)
{
    sad::Vector<sad::String> property_names;
    schema->getPropertyNames(property_names);
    for(size_t i = 0; i < property_names.size(); i++)
    {
        const sad::String& name = property_names[i];
        this->addAccessor(name, new sad::dukpp03::Getter(name), new sad::dukpp03::Setter(name));
    }
}