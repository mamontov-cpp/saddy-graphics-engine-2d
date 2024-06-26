#include "db/dbproperty.h"
#include "db/dbvariant.h"
#include "opticksupport.h"

sad::db::Property::Property()
: m_pointer_stars_count(0),
m_type_is_kind_of_sad_object(false),
m_default_value(nullptr) 
{
    PROFILER_EVENT;

}


sad::db::Property::~Property()
{
    PROFILER_EVENT;
    delete m_default_value;
}

const sad::String & sad::db::Property::baseType() const
{
    PROFILER_EVENT;
    return m_base_type;
}

bool sad::db::Property::typeIsKindOfSadObject() const
{
    PROFILER_EVENT;
    return m_type_is_kind_of_sad_object;
}

int sad::db::Property::pointerStarsCount() const
{
    PROFILER_EVENT;
    return m_pointer_stars_count;
}


bool sad::db::Property::check(const sad::String& key, const picojson::value& v)
{
    PROFILER_EVENT;
    return true;
}

bool  sad::db::Property::hasEqualTypeAs(sad::db::Property * o) const
{
    PROFILER_EVENT;
    return baseType() == o->baseType() 
        && typeIsKindOfSadObject() == o->typeIsKindOfSadObject()    
        && pointerStarsCount() == o->pointerStarsCount();
}

bool sad::db::Property::copyAndSet(sad::db::Object * o, const sad::db::Variant & v)
{
    PROFILER_EVENT;
    return this->set(o, v);
}

sad::String sad::db::Property::serializableType() const
{
    PROFILER_EVENT;
    sad::String result = this->baseType();
    if (pointerStarsCount() != 0)
    {
        result += " ";
        for(int i = 0; i < this->pointerStarsCount(); i++)
        {
            result += "*";
        }
    }
    return result;
}

bool sad::db::Property::makeNonRequiredWithDefaultValue(
    sad::db::Variant* default_value   
)
{
    PROFILER_EVENT;
    bool result = false;
    if (default_value)
    {
        if (this->couldBeSetFrom(*default_value))
        {
            if (m_default_value)
            {
                delete m_default_value;
            }
            m_default_value = default_value;
            result = true;
        }
    }
    return result;
}

bool sad::db::Property::makeRequired()
{
    PROFILER_EVENT;
    delete m_default_value;
    m_default_value = nullptr;
    return true;
}

bool sad::db::Property::hasDefaultValue() const
{
    PROFILER_EVENT;
    return m_default_value != nullptr;
}

sad::db::Variant* sad::db::Property::defaultValue() const
{
    PROFILER_EVENT;
    return m_default_value;
}
