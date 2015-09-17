namespace sad
{

template<class T1,class T2> Pair<T1,T2>::Pair()
{
}
template<class T1,class T2> Pair<T1,T2>::Pair(const T1 & m1,const T2 & m2) : m_1(m1), m_2(m2)
{
}
template<class T1,class T2> Pair<T1,T2>::Pair(const Pair<T1,T2> & o) : m_1(o.p1()) , m_2(o.p2())
{
}
template<class T1,class T2> Pair<T1,T2>& Pair<T1,T2>::operator=(const Pair<T1,T2> & o)
{
    set1(o.p1());
    set2(o.p2());
    return *this;
}
template<class T1,class T2> Pair<T1,T2>::~Pair()
{
}
template<class T1,class T2> bool Pair<T1,T2>::operator==(const Pair & o) const {return m_1==o.m_1 && m_2==o.m_2;}
template<class T1,class T2> bool Pair<T1,T2>::operator!=(const Pair & o) const {return !((*this)==o);}
template<class T1,class T2> const T1 & Pair<T1,T2>::p1() const {return m_1;}
template<class T1,class T2> const T2 & Pair<T1,T2>::p2() const {return m_2;}
template<class T1,class T2> inline Pair<T1,T2> & Pair<T1,T2>::set1(const T1 & p) { m_1=p;return *this; }
template<class T1,class T2> inline Pair<T1,T2> & Pair<T1,T2>::set2(const T2 & p) { m_2=p;return *this; }


template<class T1,class T2,class T3> Triplet<T1,T2,T3>::Triplet()
{
}
template<class T1,class T2,class T3> Triplet<T1,T2,T3>::Triplet(const T1 & m1,const T2 & m2,const T3 & m3)  : m_1(m1), m_2(m2), m_3(m3)
{
}
template<class T1,class T2,class T3> Triplet<T1,T2,T3>::Triplet(const Triplet<T1,T2,T3> & o) : m_1(o.p1()) , m_2(o.p2()), m_3(o.p3())
{
}
template<class T1,class T2,class T3> Triplet<T1,T2,T3>& Triplet<T1,T2,T3>::operator=(const Triplet<T1,T2,T3> & o)
{
    set1(o.p1());
    set2(o.p2());
    set3(o.p3());
    return *this;
}
template<class T1,class T2,class T3> Triplet<T1,T2,T3>::~Triplet()
{
}
template<class T1,class T2,class T3> bool Triplet<T1,T2,T3>::operator==(const Triplet & o) {return m_1==o.m_1 && m_2==o.m_2 && m_3==o.m_3;}
template<class T1,class T2,class T3> bool Triplet<T1,T2,T3>::operator!=(const Triplet & o) {return !((*this)==o);}
template<class T1,class T2,class T3> const T1 & Triplet<T1,T2,T3>::p1() const {return m_1;}
template<class T1,class T2,class T3> const T2 & Triplet<T1,T2,T3>::p2() const {return m_2;}
template<class T1,class T2,class T3> const T3 & Triplet<T1,T2,T3>::p3() const {return m_3;}
template<class T1,class T2,class T3> T1 & Triplet<T1,T2,T3>::_1() {return m_1;}
template<class T1,class T2,class T3> T2 & Triplet<T1,T2,T3>::_2() {return m_2;}
template<class T1,class T2,class T3> T3 & Triplet<T1,T2,T3>::_3() {return m_3;}
template<class T1,class T2,class T3> inline Triplet<T1,T2,T3> & Triplet<T1,T2,T3>::set1(const T1 & p) { m_1=p;return *this; }
template<class T1,class T2,class T3> inline Triplet<T1,T2,T3> & Triplet<T1,T2,T3>::set2(const T2 & p) { m_2=p;return *this; }
template<class T1,class T2,class T3> inline Triplet<T1,T2,T3> & Triplet<T1,T2,T3>::set3(const T3 & p) { m_3=p;return *this; }


template<class T1,class T2,class T3,class T4> Quadruplet<T1,T2,T3,T4>::Quadruplet()
{
}
template<class T1,class T2,class T3,class T4> Quadruplet<T1,T2,T3,T4>::Quadruplet(const T1 & m1,const T2 & m2,const T3 & m3, const T4 & m4)  : m_1(m1), m_2(m2), m_3(m3), m_4(m4)
{
}
template<class T1,class T2,class T3,class T4> Quadruplet<T1,T2,T3,T4>::Quadruplet(const Quadruplet<T1,T2,T3,T4> & o) : m_1(o.p1()) , m_2(o.p2()), m_3(o.p3()), m_4(o.p4())
{
}
template<class T1,class T2,class T3,class T4> Quadruplet<T1,T2,T3,T4>& Quadruplet<T1,T2,T3,T4>::operator=(const Quadruplet<T1,T2,T3,T4> & o)
{
    set1(o.p1());
    set2(o.p2());
    set3(o.p3());
    set4(o.p4());	
    return *this;
}
template<class T1,class T2,class T3,class T4> Quadruplet<T1,T2,T3,T4>::~Quadruplet()
{
}
template<class T1,class T2,class T3,class T4> bool Quadruplet<T1,T2,T3,T4>::operator==(const Quadruplet & o) {return m_1==o.m_1 && m_2==o.m_2 && m_3==o.m_3 && m_4==o.m_4;}
template<class T1,class T2,class T3,class T4> bool Quadruplet<T1,T2,T3,T4>::operator!=(const Quadruplet & o) {return !((*this)==o);}
template<class T1,class T2,class T3,class T4> const T1 & Quadruplet<T1,T2,T3,T4>::p1() const {return m_1;}
template<class T1,class T2,class T3,class T4> const T2 & Quadruplet<T1,T2,T3,T4>::p2() const {return m_2;}
template<class T1,class T2,class T3,class T4> const T3 & Quadruplet<T1,T2,T3,T4>::p3() const {return m_3;}
template<class T1,class T2,class T3,class T4> const T4 & Quadruplet<T1,T2,T3,T4>::p4() const {return m_4;}
template<class T1,class T2,class T3,class T4> T1 & Quadruplet<T1,T2,T3,T4>::_1() {return m_1;}
template<class T1,class T2,class T3,class T4> T2 & Quadruplet<T1,T2,T3,T4>::_2() {return m_2;}
template<class T1,class T2,class T3,class T4> T3 & Quadruplet<T1,T2,T3,T4>::_3() {return m_3;}
template<class T1,class T2,class T3,class T4> T4 & Quadruplet<T1,T2,T3,T4>::_4() {return m_4;}
template<class T1,class T2,class T3,class T4> inline Quadruplet<T1,T2,T3,T4> & Quadruplet<T1,T2,T3,T4>::set1(const T1 & p) { m_1=p;return *this; }
template<class T1,class T2,class T3,class T4> inline Quadruplet<T1,T2,T3,T4> & Quadruplet<T1,T2,T3,T4>::set2(const T2 & p) { m_2=p;return *this; }
template<class T1,class T2,class T3,class T4> inline Quadruplet<T1,T2,T3,T4> & Quadruplet<T1,T2,T3,T4>::set3(const T3 & p) { m_3=p;return *this; }
template<class T1,class T2,class T3,class T4> inline Quadruplet<T1,T2,T3,T4> & Quadruplet<T1,T2,T3,T4>::set4(const T4 & p) { m_4=p;return *this; }
}
