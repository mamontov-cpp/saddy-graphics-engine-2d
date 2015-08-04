#include "instanceswaysetter.h"

#include "../scripting.h"
#include "../queryobject.h"
#include "../tovalue.h"


#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../history/instances/instanceschangeway.h"

Q_DECLARE_METATYPE(sad::animations::Instance*); //-V566


// ===================== scripting::instances::WaySetter::IsWay =====================

scripting::instances::WaySetter::IsWay::IsWay()
{
	
}

scripting::instances::WaySetter::IsWay::~IsWay()
{
	
}

sad::Maybe<QString> scripting::instances::WaySetter::IsWay::check(const unsigned long long& a)
{
	sad::Maybe<QString> result;
	if (a != 0)
	{
		bool valid = false;
		sad::db::Object* r = sad::Renderer::ref()->database("")->queryByMajorId(a);
		if (r)
		{
			valid = r->isInstanceOf("sad::p2d::app::Way");
		}
		if (!valid)
		{
			QString name = QString::number(a);
			result.setValue(name + " : is not a reference to a way from database");
		}
	}
	return result;
}


// ===================== scripting::instances::WaySetter::StringToObject =====================

scripting::instances::WaySetter::StringToWay::StringToWay()
{
	
}

scripting::instances::WaySetter::StringToWay::~StringToWay()
{

}

sad::Maybe<unsigned long long> scripting::instances::WaySetter::StringToWay::toValue(const QScriptValue& v)
{
	sad::Maybe<unsigned long long> result;
	sad::Maybe<sad::p2d::app::Way*> a  = scripting::query<sad::p2d::app::Way*>(v);
	if (a.exists())
	{
		result.setValue(a.value()->MajorId);
	}
	else
	{
		sad::Maybe<sad::String> maybename  = scripting::ToValue<sad::String>::perform(v);
		if (maybename.exists())
		{
			if (maybename.value().length() == 0)
			{
				result.setValue(0);				
			}
		}
	}
	return result;
}


// ===================== scripting::instances::WaySetter =====================

scripting::instances::WaySetter::WaySetter(
		QScriptEngine* e
) : scripting::AbstractSetter<sad::animations::WayInstance*, unsigned long long>(e, "way")
{
	m_matched_property_names << "way";
	this->addCondition(new scripting::instances::WaySetter::IsWay());
	this->addConverter(new scripting::instances::WaySetter::StringToWay());
}

scripting::instances::WaySetter::~WaySetter()
{
		
}

void scripting::instances::WaySetter::setProperty(
	sad::animations::WayInstance* obj, 
	const sad::String&, 
	unsigned long long oldid,  
	unsigned long long newid
)
{
	QScriptValue main = this->engine()->globalObject().property("---");
	scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
	MainPanel* panel = e->panel();
	core::Editor* editor =  panel->editor();

	history::Command* c  = new history::instances::ChangeWay(obj, oldid, newid);		
	editor->currentBatchCommand()->add(c);
	c->commit(editor);
}
