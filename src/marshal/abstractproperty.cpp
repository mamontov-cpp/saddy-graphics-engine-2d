#include "marshal/abstractproperty.h"

DEFINE_PROPERTY_TYPESTRING( int )

void AbstractProperty::call(const sad::VariantVector & args)
{
	throw std::logic_error("Method AbstractProperty::call not implemented");
}


void AbstractProperty::set(const sad::Variant & value)
{
	throw std::logic_error("Method AbstractProperty::set not implemented");
}

sad::Variant * AbstractProperty::get() const
{
	throw std::logic_error("Method AbstractProperty::get not implemented");
	return NULL;
}

sad::String AbstractProperty::save() const
{
	throw std::logic_error("Method AbstractProperty::save not implemented");
}

void AbstractProperty::load(const sad::String & str)
{
	throw std::logic_error("Method AbstractProperty::load not implemented");
}

void AbstractProperty::resolveDeferred()
{

}

bool AbstractProperty::saveable() const
{
	return false;
}
bool AbstractProperty::callable() const
{
	return false;
}


AbstractProperty::~AbstractProperty()
{
}

sad::Variant * AbstractProperty::returnValue() const
{
	return NULL;
}

bool AbstractProperty::returnsValue() const
{
	return false;
}
