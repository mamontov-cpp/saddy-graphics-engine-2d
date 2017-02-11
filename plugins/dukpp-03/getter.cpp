#include "dukpp-03/getter.h"
#include "dukpp-03/getvalue.h"
#include "dukpp-03/pushvalue.h"


sad::dukpp03::Getter::Getter(const sad::String& name) : sad::dukpp03::Accessor(name)
{
    
}

sad::dukpp03::Getter::~Getter()
{
    
}

dukpp03::Callable<sad::dukpp03::BasicContext>* sad::dukpp03::Getter::clone()
{
    return new sad::dukpp03::Getter(m_name);    
}

int sad::dukpp03::Getter::requiredArguments()
{
    return 0;
}


int sad::dukpp03::Getter::_call(sad::dukpp03::BasicContext* c)
{
    sad::db::Object* obj = this->checkThis(c, true);
    if (obj)
    {
        sad::db::Variant v;
        obj->getObjectProperty(m_name)->get(obj, v);
        ::dukpp03::PushValue<sad::db::Variant, sad::dukpp03::BasicContext>::perform(c, v);
        return 1;
    }
    return 0;
}