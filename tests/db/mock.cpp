#include "mock.h"

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(Mock);
DECLARE_SOBJ_INHERITANCE(Mock, sad::Object)

Mock::Mock()
{
    
}

int Mock::id()
{
    return m_id;
}

int Mock::id_c() const
{
    return m_id;
}

int& Mock::id_r()
{
    return m_id;
}

int& Mock::id_rc() const
{
    return const_cast<Mock&>(*this).m_id;
}

const int& Mock::id_cr()
{
    return m_id;
}

const int& Mock::id_crc() const
{
    return m_id;
}

void Mock::setId(int i)
{
    m_id = i;
}

void Mock::setIdC(int i) const
{
    const_cast<Mock *>(this)->m_id = i;
}

void Mock::setIdR(int & i)
{
    m_id = i;
}

void Mock::setIdRC(int & i) const
{
    const_cast<Mock *>(this)->m_id = i;
}

void Mock::setIdCR(const int & i)
{
    m_id = i;
}

void Mock::setIdCRC(const int & i) const
{
    const_cast<Mock *>(this)->m_id = i;
}