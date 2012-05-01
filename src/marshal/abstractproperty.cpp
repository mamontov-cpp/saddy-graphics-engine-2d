#include "marshal/abstractproperty.h"

DEFINE_PROPERTY_TYPESTRING( int )

void AbstractProperty::call(const sad::VariantVector & args,ActionContext * context)
{
	throw new serializable::MethodIsNotImplemented("AbstractProperty","call",this,context);
}


void AbstractProperty::set(const sad::Variant & value,ActionContext * context)
{
	throw new serializable::MethodIsNotImplemented("AbstractProperty","set",this,context);
}

sad::Variant * AbstractProperty::get(ActionContext * context) const
{
	throw new serializable::MethodIsNotImplemented("AbstractProperty","get",const_cast<AbstractProperty*>(this),context);
	return new sad::Variant(0);
}

hst::string AbstractProperty::save(ActionContext * context) const
{
	throw new serializable::MethodIsNotImplemented("AbstractProperty","save",const_cast<AbstractProperty*>(this),context);
}

void AbstractProperty::load(const hst::string & str, ActionContext * context)
{
	throw new serializable::MethodIsNotImplemented("AbstractProperty","load",this,context);
}

void AbstractProperty::resolveDeferred(ActionContext * context)
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