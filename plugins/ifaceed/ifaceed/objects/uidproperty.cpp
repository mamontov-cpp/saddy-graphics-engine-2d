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

void UidProperty::set(const sad::Variant & value)
{
	if (this->getParent())
	{
		AbstractScreenObject * object = static_cast<AbstractScreenObject *>(this->getParent());
		if (object->screenTemplate())
		{
			object->screenTemplate()->setUid(object,value.get<sad::String>());
		}
		else
		{
			m_uid_deferred = value.get<sad::String>();
		}
	}
}


sad::Variant * UidProperty::get() const
{
	if (this->getParent())
	{
		delete m_variant;
		AbstractScreenObject * object = static_cast<AbstractScreenObject *>(this->getParent());
		sad::String uid = m_uid_deferred;
		if (object->screenTemplate() && !m_loaded_data) 
		{
			uid = object->screenTemplate()->uid(object);
		} 
		else
		{
			const_cast<UidProperty*>(this)->m_loaded_data = false;
		}
		const_cast<UidProperty*>(this)->m_variant = new sad::Variant(uid);
		return m_variant;
	}
	return NULL;
}

sad::String UidProperty::save() const
{
	return  SaveLoadCallback<sad::String>::save( get()->get<sad::String> ());
}

void UidProperty::load(const sad::String & str)
{
	m_uid_deferred = SaveLoadCallback<sad::String>::load(str);
	m_loaded_data = true;
}



bool UidProperty::saveable() const
{
	return true;
}

