#include "mock2.h"

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(Mock2);
DECLARE_SOBJ_INHERITANCE(Mock2, sad::Object)

Mock2::Mock2()
{
    
}

int Mock2::id()
{
    return m_id;
}

int Mock2::id_c() const
{
    return m_id;
}

int& Mock2::id_r()
{
    return m_id;
}

int& Mock2::id_rc() const
{
    return const_cast<Mock2&>(*this).m_id;
}

const int& Mock2::id_cr()
{
    return m_id;
}

const int& Mock2::id_crc() const
{
    return m_id;
}

void Mock2::setId(int i)
{
    m_id = i;
}

void Mock2::setIdC(int i) const
{
    const_cast<Mock2 *>(this)->m_id = i;
}

void Mock2::setIdR(int & i)
{
    m_id = i;
}

void Mock2::setIdRC(int & i) const
{
    const_cast<Mock2 *>(this)->m_id = i;
}

void Mock2::setIdCR(const int & i)
{
    m_id = i;
}

void Mock2::setIdCRC(const int & i) const
{
    const_cast<Mock2 *>(this)->m_id = i;
}