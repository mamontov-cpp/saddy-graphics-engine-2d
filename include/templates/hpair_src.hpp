namespace hst
{

template<class T1,class T2> pair<T1,T2>::pair()
{
    m_1=T1();
    m_2=T2();
}
template<class T1,class T2> pair<T1,T2>::pair(const T1 & m1,const T2 & m2)
{
    m_1=m1;
    m_2=m2;
}
template<class T1,class T2> pair<T1,T2>::pair(const pair<T1,T2> & o)
{
    set1(o.p1());
    set2(o.p2());
}
template<class T1,class T2> pair<T1,T2>& pair<T1,T2>::operator=(const pair<T1,T2> & o)
{
    set1(o.p1());
    set2(o.p2());
    return *this;
}
template<class T1,class T2> pair<T1,T2>::~pair()
{
}
template<class T1,class T2> bool pair<T1,T2>::operator==(const pair & o) const {return m_1==o.m_1 && m_2==o.m_2;}
template<class T1,class T2> bool pair<T1,T2>::operator!=(const pair & o) const {return !((*this)==o);}
template<class T1,class T2> const T1 & pair<T1,T2>::p1() const {return m_1;}
template<class T1,class T2> const T2 & pair<T1,T2>::p2() const {return m_2;}
template<class T1,class T2> inline pair<T1,T2> & pair<T1,T2>::set1(const T1 & p) { m_1=p;return *this; }
template<class T1,class T2> inline pair<T1,T2> & pair<T1,T2>::set2(const T2 & p) { m_2=p;return *this; }


template<class T1,class T2,class T3> triplet<T1,T2,T3>::triplet()
{
    m_1=T1();
    m_2=T2();
	m_3=T3();
}
template<class T1,class T2,class T3> triplet<T1,T2,T3>::triplet(const T1 & m1,const T2 & m2,const T3 & m3)
{
    m_1=m1;
    m_2=m2;
	m_3=m3;
}
template<class T1,class T2,class T3> triplet<T1,T2,T3>::triplet(const triplet<T1,T2,T3> & o)
{
    set1(o.p1());
    set2(o.p2());
	set3(o.p3());
}
template<class T1,class T2,class T3> triplet<T1,T2,T3>& triplet<T1,T2,T3>::operator=(const triplet<T1,T2,T3> & o)
{
    set1(o.p1());
    set2(o.p2());
	set3(o.p3());
    return *this;
}
template<class T1,class T2,class T3> triplet<T1,T2,T3>::~triplet()
{
}
template<class T1,class T2,class T3> bool triplet<T1,T2,T3>::operator==(const triplet & o) {return m_1==o.m_1 && m_2==o.m_2 && m_3==o.m_3;}
template<class T1,class T2,class T3> bool triplet<T1,T2,T3>::operator!=(const triplet & o) {return !((*this)==o);}
template<class T1,class T2,class T3> const T1 & triplet<T1,T2,T3>::p1() const {return m_1;}
template<class T1,class T2,class T3> const T2 & triplet<T1,T2,T3>::p2() const {return m_2;}
template<class T1,class T2,class T3> const T3 & triplet<T1,T2,T3>::p3() const {return m_3;}
template<class T1,class T2,class T3> T1 & triplet<T1,T2,T3>::_1() {return m_1;}
template<class T1,class T2,class T3> T2 & triplet<T1,T2,T3>::_2() {return m_2;}
template<class T1,class T2,class T3> T3 & triplet<T1,T2,T3>::_3() {return m_3;}
template<class T1,class T2,class T3> inline triplet<T1,T2,T3> & triplet<T1,T2,T3>::set1(const T1 & p) { m_1=p;return *this; }
template<class T1,class T2,class T3> inline triplet<T1,T2,T3> & triplet<T1,T2,T3>::set2(const T2 & p) { m_2=p;return *this; }
template<class T1,class T2,class T3> inline triplet<T1,T2,T3> & triplet<T1,T2,T3>::set3(const T3 & p) { m_3=p;return *this; }

}
