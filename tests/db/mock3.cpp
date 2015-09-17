#include "mock3.h"

#include <db/dbfield.h>
#include <db/dbmethodpair.h>
#include "db/save.h"
#include "db/load.h"


DECLARE_TYPE_AS_SAD_OBJECT_ENUM(Mock3);
DECLARE_SOBJ_INHERITANCE(Mock3, sad::Object)

Mock3::Mock3()
{
    m_schema.addParent(sad::db::Object::basicSchema());
    m_schema.add("prop", new sad::db::Field<Mock3, int>(&Mock3::m_id));
    m_schema.add("prop2", new sad::db::MethodPair<Mock3, int>(&Mock3::id_c, &Mock3::setId));
}

int Mock3::id()
{
    return m_id;
}

int Mock3::id_c() const
{
    return m_id;
}

int& Mock3::id_r()
{
    return m_id;
}

int& Mock3::id_rc() const
{
    return const_cast<Mock3&>(*this).m_id;
}

const int& Mock3::id_cr()
{
    return m_id;
}

const int& Mock3::id_crc() const
{
    return m_id;
}

void Mock3::setId(int i)
{
    m_id = i;
}

void Mock3::setIdC(int i) const
{
    const_cast<Mock3 *>(this)->m_id = i;
}

void Mock3::setIdR(int & i)
{
    m_id = i;
}

void Mock3::setIdRC(int & i) const
{
    const_cast<Mock3 *>(this)->m_id = i;
}

void Mock3::setIdCR(const int & i)
{
    m_id = i;
}

void Mock3::setIdCRC(const int & i) const
{
    const_cast<Mock3 *>(this)->m_id = i;
}

sad::db::schema::Schema * Mock3::schema() const
{
    return &(const_cast<Mock3*>(this)->m_schema);	
}
