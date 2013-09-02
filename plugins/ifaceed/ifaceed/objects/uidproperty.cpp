#include "uidproperty.h"
#include "abstractscreenobject.h"
#include "../editorcore/editor.h"
#include "marshal/saveloadcallbacks.h"
#include "screentemplate.h"



UidProperty::UidProperty()
{
	m_variant = NULL;
	m_loaded_data = false;
}

UidProperty::~UidProperty()
{
	delete m_variant;
}

void UidProperty::set(const sad::Variant & value,ActionContext * context)
{
	if (this->getParent())
	{
		context->pushAction("Entering UidProperty::set");
		AbstractScreenObject * object = static_cast<AbstractScreenObject *>(this->getParent());
		if (object->screenTemplate())
		{
			object->screenTemplate()->setUid(object,value.get<hst::string>(context));
		}
		else
		{
			m_uid_deferred = value.get<hst::string>(context);
		}
		context->popAction();
	}
}


sad::Variant * UidProperty::get(ActionContext * context) const
{
	if (this->getParent())
	{
		delete m_variant;
		context->pushAction("Entering UidProperty::get");
		AbstractScreenObject * object = static_cast<AbstractScreenObject *>(this->getParent());
		hst::string uid = m_uid_deferred;
		if (object->screenTemplate() && !m_loaded_data) 
		{
			uid = object->screenTemplate()->uid(object);
		} 
		else
		{
			const_cast<UidProperty*>(this)->m_loaded_data = false;
		}
		const_cast<UidProperty*>(this)->m_variant = new sad::Variant(uid);
		context->popAction();
		return m_variant;
	}
	return NULL;
}

hst::string UidProperty::save(ActionContext * context) const
{
	return  SaveLoadCallback<hst::string>::save( get(context)->get<hst::string> (context));
}

void UidProperty::load(const hst::string & str, ActionContext * context)
{
	m_uid_deferred = SaveLoadCallback<hst::string>::load(context,str);
	m_loaded_data = true;
}



bool UidProperty::saveable() const
{
	return true;
}

