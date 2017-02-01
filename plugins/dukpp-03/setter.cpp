#include "dukpp-03/setter.h"
#include "dukpp-03/context.h"


// ===================================== PUBLIC METHODS =====================================

sad::dukpp03::Setter::Setter(const sad::String& name) : sad::dukpp03::Accessor(name)
{
    
}

sad::dukpp03::Setter::~Setter()
{
    
}

dukpp03::Callable<sad::dukpp03::BasicContext>* sad::dukpp03::Setter::clone()
{
    return new sad::dukpp03::Setter(m_name);    
}

int sad::dukpp03::Setter::requiredArguments()
{
    return 1;
}


int sad::dukpp03::Setter::_call(sad::dukpp03::BasicContext* c)
{
    sad::db::Object* obj = this->checkThis(c, true);
    if (obj)
    {
        ::dukpp03::Maybe<sad::db::Variant> v = DUKPP03_FROM_STACK(sad::db::Variant,c, 0);
        if (v.exists())
        {
            sad::db::Property* prop = obj->getObjectProperty(m_name);
            if (prop->couldBeSetFrom(v.mutableValue()))
            {
                 prop->set(obj, v.mutableValue());
            }
            else
            {
                c->throwInvalidTypeError(1, v.mutableValue().typeName());
            }
        }
        else
        {
            c->throwInvalidTypeError(1, "unknown");
        }
    }
    return 0;
}

// ===================================== PROTECTED METHODS =====================================


int sad::dukpp03::Setter::checkRemainingArguments(sad::db::Object* o, sad::dukpp03::BasicContext* c) const
{
    ::dukpp03::Maybe<sad::db::Variant> v = DUKPP03_FROM_STACK(sad::db::Variant,c, 0);
    if (v.exists())
    {
        if (o->getObjectProperty(m_name)->couldBeSetFrom(v.mutableValue()))
        {
            return 1;
        }
    }
    return 0;
}