namespace hst
{

template<class T> point<hst::D2,T>::point()
{
    m_x=(T)(0.0f);
    m_y=(T)(0.0f);
}
template<class T> point<hst::D2,T>::point(T x, T y)
{
    m_x=x;
    m_y=y;
}
template<class T> point<hst::D2,T>::point(const point<hst::D2,T> & o)
{
    m_x=o.m_x;
    m_y=o.m_y;
}
template<class T> point<hst::D2,T>& point<hst::D2,T>::operator=(const point<hst::D2,T> & o)
{
    m_x=o.m_x;
    m_y=o.m_y;
    return *this;
}
template<class T> point<hst::D2,T>::~point()
{
}

template<class T> inline bool point<hst::D2,T>::operator==(const point<hst::D2,T>& o) const { return m_x==o.m_x && m_y==o.m_y;}
template<class T> inline bool point<hst::D2,T>::operator!=(const point<hst::D2,T>& o) const { return m_x!=o.m_x || m_y!=o.m_y;}

template<class T> point<hst::D2,T> point<hst::D2,T>::operator+(const point<hst::D2,T> & o) const { return point<hst::D2,T>(m_x+o.m_x,m_y+o.m_y);}
template<class T> point<hst::D2,T> point<hst::D2,T>::operator-(const point<hst::D2,T> & o) const { return point<hst::D2,T>(m_x-o.m_x,m_y-o.m_y);}
template<class T> point<hst::D2,T> point<hst::D2,T>::operator*(T  o) const { return point<hst::D2,T>(m_x*o,m_y*o);}
template<class T> point<hst::D2,T> point<hst::D2,T>::operator/(T  o) const { return point<hst::D2,T>(m_x/o,m_y/o);}

template<class T> inline point<hst::D2,T>& point<hst::D2,T>::operator+=(const point<hst::D2,T> & o) { m_x+=o.m_x;m_y+=o.m_y; return *this; }
template<class T> inline point<hst::D2,T>& point<hst::D2,T>::operator-=(const point<hst::D2,T> & o) { m_x-=o.m_x;m_y-=o.m_y; return *this; }
template<class T> inline point<hst::D2,T>& point<hst::D2,T>::operator*=(T o) { m_x*=o;m_y*=o; return *this; }
template<class T> inline point<hst::D2,T>& point<hst::D2,T>::operator/=(T o) { m_x/=o;m_y/=o; return *this; }

template<class T> inline T point<hst::D2,T>::x() const {return m_x;}
template<class T> inline T point<hst::D2,T>::y() const {return m_y;}

template<class T> inline point<hst::D2,T>& point<hst::D2,T>::setX(T x) {m_x=x; return *this;}
template<class T> inline point<hst::D2,T>& point<hst::D2,T>::setY(T y) {m_y=y; return *this;}

template<class T> point<hst::D3,T>::point()
{
    m_x=(T)(0);
    m_y=(T)(0);
    m_z=(T)(0);
}
template<class T> point<hst::D3,T>::point(T x, T y, T z)
{
    m_x=x;
    m_y=y;
    m_z=z;
}
template<class T> point<hst::D3,T>::point(const point<hst::D3,T> & o)
{
    m_x=o.m_x;
    m_y=o.m_y;
    m_z=o.m_z;
}
template<class T> point<hst::D3,T>& point<hst::D3,T>::operator=(const point<hst::D3,T> & o)
{
    m_x=o.m_x;
    m_y=o.m_y;
    m_z=o.m_z;
    return *this;
}
template<class T> point<hst::D3,T>::~point()
{
}

template<class T> inline bool point<hst::D3,T>::operator==(const point<hst::D3,T>& o) const { return m_x==o.m_x && m_y==o.m_y && m_z=o.m_z;}
template<class T> inline bool point<hst::D3,T>::operator!=(const point<hst::D3,T>& o) const { return m_x!=o.m_x || m_y!=o.m_y || m_z!=o.m_z;}

template<class T> point<hst::D3,T> point<hst::D3,T>::operator+(const point<hst::D3,T> & o) const { return point<hst::D3,T>(m_x+o.m_x,m_y+o.m_y,m_z+o.m_z);}
template<class T> point<hst::D3,T> point<hst::D3,T>::operator-(const point<hst::D3,T> & o) const { return point<hst::D3,T>(m_x-o.m_x,m_y-o.m_y,m_z-o.m_z);}
template<class T> point<hst::D3,T> point<hst::D3,T>::operator*(T  o) const { return point<hst::D3,T>(m_x*o,m_y*o,m_z*o);}
template<class T> point<hst::D3,T> point<hst::D3,T>::operator/(T  o) const { return point<hst::D3,T>(m_x/o,m_y/o,m_z/o);}

template<class T> inline point<hst::D3,T>& point<hst::D3,T>::operator+=(const point<hst::D3,T> & o) { m_x+=o.m_x;m_y+=o.m_y;m_z+=o.m_z; return *this; }
template<class T> inline point<hst::D3,T>& point<hst::D3,T>::operator-=(const point<hst::D3,T> & o) { m_x-=o.m_x;m_y-=o.m_y;m_z-=o.m_z; return *this; }
template<class T> inline point<hst::D3,T>& point<hst::D3,T>::operator*=(T o) { m_x*=o;m_y*=o;m_z*=o; return *this; }
template<class T> inline point<hst::D3,T>& point<hst::D3,T>::operator/=(T o) { m_x/=o;m_y/=o;m_z/=o; return *this; }

template<class T> inline T point<hst::D3,T>::x() const {return m_x;}
template<class T> inline T point<hst::D3,T>::y() const {return m_y;}
template<class T> inline T point<hst::D3,T>::z() const {return m_z;}

template<class T> inline point<hst::D3,T>& point<hst::D3,T>::setX(T x) {m_x=x; return *this;}
template<class T> inline point<hst::D3,T>& point<hst::D3,T>::setY(T y) {m_y=y; return *this;}
template<class T> inline point<hst::D3,T>& point<hst::D3,T>::setZ(T z) {m_z=z; return *this;}

template<class T>
point<hst::D2,T> toPolar(const point<hst::D2,T> & o)
{
 return point<hst::D2,T>(o.x()*cos(o.y()/180*3.14),o.x()*sin(o.y()/180*3.14));
}

}
