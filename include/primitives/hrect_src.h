#include <assert.h>
namespace hst
{

template<class T> rect<T>::rect()
{
    m_p[0]=m_p[1]=m_p[2]=m_p[3]=T();
}
template<class T> rect<T>::rect(const T & p1,const T & p2)
{
    if (p1.x()>p2.x()) { m_p[0].setX(p2.x());m_p[3].setX(p2.x());m_p[1].setX(p1.x());m_p[2].setX(p1.x()); }
    else               { m_p[0].setX(p1.x());m_p[3].setX(p1.x());m_p[1].setX(p2.x());m_p[2].setX(p2.x()); }
    if (p1.y()>p2.y()) { m_p[0].setY(p2.y());m_p[3].setY(p1.y());m_p[1].setY(p2.y());m_p[2].setY(p1.y()); }
    else               { m_p[0].setY(p1.y());m_p[3].setY(p2.y());m_p[1].setY(p1.y());m_p[2].setY(p2.y()); }
}
template<class T> rect<T>::rect(const T & p1,const T & p2,const T & p3,const T & p4)
{
    m_p[0]=p1;
    m_p[1]=p2;
    m_p[2]=p3;
    m_p[3]=p4;
}

template<class T> rect<T>::~rect()
{

}
template<class T> rect<T>::rect(const rect<T>& o)
{
    m_p[0]=o.m_p[0];
    m_p[1]=o.m_p[1];
    m_p[2]=o.m_p[2];
    m_p[3]=o.m_p[3];
}
template<class T> rect<T>& rect<T>::operator=(const rect<T>& o)
{
    m_p[0]=o.m_p[0];
    m_p[1]=o.m_p[1];
    m_p[2]=o.m_p[2];
    m_p[3]=o.m_p[3];
    return *this;
}
template<class T> bool rect<T>::operator==(const rect<T>& o)
{
    return m_p[0]==o.m_p[0] && m_p[1]==o.m_p[1] && m_p[2]==o.m_p[1] && m_p[2]==o.m_p[3];
}
template<class T> bool rect<T>::operator!=(const rect<T>& o)
{
    return !(*this==o);
}
template<class T> const T & rect<T>::operator[](unsigned int i) const
{
    assert(i<4);
    return m_p[i];
}
template<class T> T & rect<T>::operator[](unsigned int i)
{
    assert(i<4);
    return m_p[i];
}

}
