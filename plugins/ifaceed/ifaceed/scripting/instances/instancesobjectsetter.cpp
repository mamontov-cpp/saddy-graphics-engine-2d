#include "instancesobjectsetter.h"

#include "../scripting.h"
#include "../queryobject.h"
#include "../tovalue.h"


#include "../../mainpanel.h"

#include "../../core/editor.h"

#include "../../history/instances/instanceschangeobject.h"

Q_DECLARE_METATYPE(sad::animations::Instance*); //-V566


// ===================== scripting::instances::ObjectSetter::IsObject =====================

scripting::instances::ObjectSetter::IsObject::IsObject()
{
    
}

scripting::instances::ObjectSetter::IsObject::~IsObject()
{
    
}

sad::Maybe<QString> scripting::instances::ObjectSetter::IsObject::check(const unsigned long long& a)
{
    sad::Maybe<QString> result;
    if (a != 0)
    {
        bool valid = false;
        sad::db::Object* r = sad::Renderer::ref()->database("")->queryByMajorId(a);
        if (r)
        {
            valid = r->isInstanceOf("sad::Scene") || r->isInstanceOf("sad::SceneNode");
        }
        if (!valid)
        {
            QString name = QString::number(a);
            result.setValue(name + " : is not a reference to an object from database");
        }
    }
    return result;
}


// ===================== scripting::instances::ObjectSetter::StringToObject =====================

scripting::instances::ObjectSetter::StringToObject::StringToObject()
{
    
}

scripting::instances::ObjectSetter::StringToObject::~StringToObject()
{

}

sad::Maybe<unsigned long long> scripting::instances::ObjectSetter::StringToObject::toValue(const QScriptValue& v)
{
    sad::Maybe<unsigned long long> result;
    sad::Maybe<sad::db::Object*> a  = scripting::query_object(v);
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


// ===================== scripting::instances::ObjectSetter =====================

scripting::instances::ObjectSetter::ObjectSetter(
        QScriptEngine* e
) : scripting::AbstractSetter<sad::animations::Instance*, unsigned long long>(e, "object")
{
    m_matched_property_names << "object";
    this->addCondition(new scripting::instances::ObjectSetter::IsObject());
    this->addConverter(new scripting::instances::ObjectSetter::StringToObject());
}

scripting::instances::ObjectSetter::~ObjectSetter()
{
        
}

void scripting::instances::ObjectSetter::setProperty(
    sad::animations::Instance* obj, 
    const sad::String& propertyname, 
    unsigned long long oldid,  
    unsigned long long newid
)
{
    QScriptValue main = this->engine()->globalObject().property("---");
    scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
    MainPanel* panel = e->panel();
    core::Editor* editor =  panel->editor();

    history::Command* c  = new history::instances::ChangeObject(obj, oldid, newid);		
    editor->currentBatchCommand()->add(c);
    c->commit(editor);
}
