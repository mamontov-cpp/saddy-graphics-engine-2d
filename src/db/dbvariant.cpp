#include "db/dbvariant.h"
#include "db/save.h"

sad::db::Variant::Variant() : 
m_object(NULL), 
m_pointers_stars_count(0), 
m_is_sad_object(false),
m_typename(""), 
m_copy(NULL),
m_delete(NULL),
m_save(NULL),
m_load(NULL)
{

}

sad::db::Variant::Variant(const sad::db::Variant & v)
{
    assign(v);
}

sad::db::Variant & sad::db::Variant::operator=(const sad::db::Variant  & v)
{
    release();
    assign(v);
    return *this;
}

sad::db::Variant::Variant(const char* v)
{
    m_object = new sad::String(v);
    sad::db::TypeName<sad::String>::init();
    m_is_sad_object = sad::db::TypeName<sad::String>::isSadObject();
    m_typename = sad::db::TypeName<sad::String>::name();
    m_copy = sad::db::variant::copy_value<sad::String>;
    m_delete = sad::db::variant::delete_value<sad::String>;
    m_save = sad::db::Save<sad::String>::perform;
    m_load = sad::db::Load<sad::String>::perform;
    m_pointers_stars_count = sad::db::TypeName<sad::String>::POINTER_STARS_COUNT;
    m_base_name = sad::db::TypeName<sad::String>::baseName();
}

sad::db::Variant::~Variant()
{
    release();
}


picojson::value sad::db::Variant::save() const
{
    picojson::value v;
    if (m_object)
    {
        v = m_save(m_object);
    }
    return v;
}

bool sad::db::Variant::load(const picojson::value & v)
{
    if (m_object)
    {
        return m_load(m_object, v);
    }
    return false;
}


void sad::db::Variant::release()
{
    if (m_object)
    {
        m_delete(m_object);
    }
}


void sad::db::Variant::assign(const sad::db::Variant & v)
{
    if (v.m_object)
    {
        m_object = (v.m_copy)(v.m_object);
        m_is_sad_object = v.m_is_sad_object;
        m_typename = v.m_typename;
        m_copy = v.m_copy;
        m_delete = v.m_delete;
        m_save = v.m_save;
        m_load = v.m_load;
        m_pointers_stars_count = v.m_pointers_stars_count;
        m_base_name = v.m_base_name;
    }
    else
    {
        m_object = NULL;
        m_is_sad_object = false;
        m_typename = "";
        m_copy = NULL;
        m_delete = NULL;
        m_save = NULL;
        m_load = NULL;
        m_pointers_stars_count = 0;
        m_base_name.clear();
    }
}
