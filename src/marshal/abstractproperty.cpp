#include "marshal/abstractproperty.h"

DEFINE_PROPERTY_TYPESTRING( int )

void AbstractProperty::call(const sad::VariantVector & args)
{
	throw new serializable::MethodIsNotImplemented("AbstractProperty","call");
}


void AbstractProperty::set(const sad::Variant & value)
{
	throw new serializable::MethodIsNotImplemented("AbstractProperty","set");
}

sad::Variant * AbstractProperty::get() const
{
	throw new serializable::MethodIsNotImplemented("AbstractProperty","get");
	return new sad::Variant(0);
}

sad::String AbstractProperty::save() const
{
	throw new serializable::MethodIsNotImplemented("AbstractProperty","save");
}

void AbstractProperty::load(const sad::String & str)
{
	throw new serializable::MethodIsNotImplemented("AbstractProperty","load");
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
