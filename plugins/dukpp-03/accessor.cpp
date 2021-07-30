#include "dukpp-03/accessor.h"
#include "dukpp-03/getvalue.h"
#include "dukpp-03/pushvalue.h"

// ================================ PUBLIC METHODS ================================

sad::dukpp03::Accessor::Accessor(const sad::String& name) : m_name(name)
{
    
}

std::pair<int, bool> sad::dukpp03::Accessor::canBeCalled(sad::dukpp03::BasicContext* c)
{
    if (c->getTop() != this->requiredArguments())
    {
        return std::make_pair(-1, false);
    }
    int matchedargs = 0;
    sad::db::Object* obj = this->checkThis(c, false);
    if (obj)
    {
        matchedargs += 1;
        matchedargs += this->checkRemainingArguments(obj, c);
    }
    return std::make_pair(matchedargs, matchedargs == (this->requiredArguments() + 1));
}

sad::dukpp03::Accessor::~Accessor()
{
    
}

bool sad::dukpp03::Accessor::canBeCalledAsConstructor()
{
    return false;
}

// ================================ PROTECTED METHODS ================================

int sad::dukpp03::Accessor::checkRemainingArguments(sad::db::Object* o, sad::dukpp03::BasicContext* c) const
{
    return 0;
}

sad::db::Object* sad::dukpp03::Accessor::checkThis(sad::dukpp03::BasicContext* c, bool throw_error) const
{
    ::dukpp03::Maybe< sad::db::Object* > maybethisobject;

    sad::dukpp03::BasicContext::LocalCallable::CheckArgument< sad::db::Object* >::passedAsThis(c, maybethisobject);
    if (maybethisobject.exists())
    {
        sad::db::Object* thisobj  = maybethisobject.value();
        if (thisobj->getObjectProperty(m_name))
        {
            return thisobj;
        }
        else
        {
            if (throw_error)
            {
                sad::String error = "The object, passed as this, doesn't have property \"";
                error << m_name;
                error << "\""; 
                c->throwError(error);
            }
        }
    } 
    else
    {
        if (throw_error)
        {
            c->throwError("The object, passed as this, is not a subclass of sad::db::Object");
        }
    }
    return nullptr;
}
