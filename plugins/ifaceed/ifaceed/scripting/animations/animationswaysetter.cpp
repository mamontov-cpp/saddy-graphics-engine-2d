#include "animationswaysetter.h"

#include "../tovalue.h"
#include "../queryobject.h"

#include <renderer.h>

#include <db/dbdatabase.h>

#include <p2d/app/way.h>

// ================================= scripting::animations::WaySetter::WayFromString =================================

scripting::animations::WaySetter::WayFromString::WayFromString()
{
    
}


scripting::animations::WaySetter::WayFromString::~WayFromString()
{
    
}

sad::Maybe<unsigned long long> scripting::animations::WaySetter::WayFromString::toValue(const QScriptValue& v)
{
    sad::Maybe<unsigned long long>  result;
    sad::Maybe<sad::String> maybename = scripting::ToValue<sad::String>::perform(v);
    if (maybename.exists())
    {
        if (maybename.value().length() != 0)
        {
            sad::Maybe<sad::p2d::app::Way*> w = scripting::query<sad::p2d::app::Way*>(v);
            if (w.exists())
            {
                result.setValue(w.value()->MajorId);
            }
        }
        else
        {
            result.setValue(0);
        }
    }
    return result;
}

// ================================= scripting::animations::WaySetter::IsAWay =================================


scripting::animations::WaySetter::IsAWay::IsAWay()
{
    
}


scripting::animations::WaySetter::IsAWay::~IsAWay()
{
    
}

sad::Maybe<QString> scripting::animations::WaySetter::IsAWay::check(const unsigned long long& a)
{
    sad::Maybe<QString> result;
    if (a != 0)
    {
        sad::db::Object* o = sad::Renderer::ref()->database("")->queryByMajorId(a);
        bool valid = false;
        if (o)
        {
            if (o->isInstanceOf("sad::p2d::app::Way"))
            {
                valid = true;
            }
        }
        
        if (!valid)
        {
            result.setValue("set() : argument 3 is not a valid reference to way");
        }
    }
    return result;
}

// ================================= scripting::animations::WaySetter =================================

scripting::animations::WaySetter::WaySetter(
    QScriptEngine* e
) :  scripting::animations::Setter<sad::animations::WayMoving, unsigned long long, history::animations::ChangeWayMovingWay>(e, "way")
{
    addConverter(new scripting::animations::WaySetter::WayFromString());
    addCondition(new scripting::animations::WaySetter::IsAWay());
}

scripting::animations::WaySetter::~WaySetter()
{

}