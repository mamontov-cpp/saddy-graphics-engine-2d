#include <assert.h>
namespace hst
{

template<class T> rect<T>::rect()
{
    m_p[0]=m_p[1]=m_p[2]=m_p[3]=T();
}
template<class T> rect<T>::rect(const T & p1,const T & p2)
{
    m_p[0]=p1; m_p[1]=p1;  m_p[1].setX(p2.x());
    m_p[3]=p2; m_p[2]=p2;  m_p[3].setX(p1.x());
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
