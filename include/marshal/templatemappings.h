#pragma once

template<typename _Class>
class VoidMappedMethod0: public MappedMethod<_Class>
{
private:
        void (_Class::*m_callback)();
public:
       inline VoidMappedMethod0(  void (_Class::*callback)()  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=0) throw new InvalidParams(0,v.count(),c);
             (this->realObject()->*m_callback)();
       }    
};

template< typename _Class >
AbstractProperty * createVoidMethodBinding( void (_Class::*callback)()  )
{
 return new VoidMappedMethod0<_Class>(callback);
}
template<typename _Class, typename T0>
class VoidMappedMethod1: public MappedMethod<_Class>
{
private:
        void (_Class::*m_callback)(T0 a0);
public:
       inline VoidMappedMethod1(  void (_Class::*callback)(T0 a0)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=1) throw new InvalidParams(1,v.count(),c);
             (this->realObject()->*m_callback)(v[0].get<T0>(c));
       }    
};

template< typename _Class , typename T0>
AbstractProperty * createVoidMethodBinding( void (_Class::*callback)(T0 a0)  )
{
 return new VoidMappedMethod1<_Class,T0>(callback);
}
template<typename _Class, typename T0, typename T1>
class VoidMappedMethod2: public MappedMethod<_Class>
{
private:
        void (_Class::*m_callback)(T0 a0, T1 a1);
public:
       inline VoidMappedMethod2(  void (_Class::*callback)(T0 a0, T1 a1)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=2) throw new InvalidParams(2,v.count(),c);
             (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c));
       }    
};

template< typename _Class , typename T0, typename T1>
AbstractProperty * createVoidMethodBinding( void (_Class::*callback)(T0 a0, T1 a1)  )
{
 return new VoidMappedMethod2<_Class,T0,T1>(callback);
}
template<typename _Class, typename T0, typename T1, typename T2>
class VoidMappedMethod3: public MappedMethod<_Class>
{
private:
        void (_Class::*m_callback)(T0 a0, T1 a1, T2 a2);
public:
       inline VoidMappedMethod3(  void (_Class::*callback)(T0 a0, T1 a1, T2 a2)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=3) throw new InvalidParams(3,v.count(),c);
             (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c));
       }    
};

template< typename _Class , typename T0, typename T1, typename T2>
AbstractProperty * createVoidMethodBinding( void (_Class::*callback)(T0 a0, T1 a1, T2 a2)  )
{
 return new VoidMappedMethod3<_Class,T0,T1,T2>(callback);
}
template<typename _Class, typename T0, typename T1, typename T2, typename T3>
class VoidMappedMethod4: public MappedMethod<_Class>
{
private:
        void (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3);
public:
       inline VoidMappedMethod4(  void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=4) throw new InvalidParams(4,v.count(),c);
             (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c));
       }    
};

template< typename _Class , typename T0, typename T1, typename T2, typename T3>
AbstractProperty * createVoidMethodBinding( void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3)  )
{
 return new VoidMappedMethod4<_Class,T0,T1,T2,T3>(callback);
}
template<typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4>
class VoidMappedMethod5: public MappedMethod<_Class>
{
private:
        void (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4);
public:
       inline VoidMappedMethod5(  void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=5) throw new InvalidParams(5,v.count(),c);
             (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c));
       }    
};

template< typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4>
AbstractProperty * createVoidMethodBinding( void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4)  )
{
 return new VoidMappedMethod5<_Class,T0,T1,T2,T3,T4>(callback);
}
template<typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class VoidMappedMethod6: public MappedMethod<_Class>
{
private:
        void (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);
public:
       inline VoidMappedMethod6(  void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=6) throw new InvalidParams(6,v.count(),c);
             (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c));
       }    
};

template< typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
AbstractProperty * createVoidMethodBinding( void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)  )
{
 return new VoidMappedMethod6<_Class,T0,T1,T2,T3,T4,T5>(callback);
}
template<typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class VoidMappedMethod7: public MappedMethod<_Class>
{
private:
        void (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);
public:
       inline VoidMappedMethod7(  void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=7) throw new InvalidParams(7,v.count(),c);
             (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c));
       }    
};

template< typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
AbstractProperty * createVoidMethodBinding( void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)  )
{
 return new VoidMappedMethod7<_Class,T0,T1,T2,T3,T4,T5,T6>(callback);
}
template<typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class VoidMappedMethod8: public MappedMethod<_Class>
{
private:
        void (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7);
public:
       inline VoidMappedMethod8(  void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=8) throw new InvalidParams(8,v.count(),c);
             (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c));
       }    
};

template< typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
AbstractProperty * createVoidMethodBinding( void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7)  )
{
 return new VoidMappedMethod8<_Class,T0,T1,T2,T3,T4,T5,T6,T7>(callback);
}
template<typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class VoidMappedMethod9: public MappedMethod<_Class>
{
private:
        void (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8);
public:
       inline VoidMappedMethod9(  void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=9) throw new InvalidParams(9,v.count(),c);
             (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c), v[8].get<T8>(c));
       }    
};

template< typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
AbstractProperty * createVoidMethodBinding( void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8)  )
{
 return new VoidMappedMethod9<_Class,T0,T1,T2,T3,T4,T5,T6,T7,T8>(callback);
}
template<typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class VoidMappedMethod10: public MappedMethod<_Class>
{
private:
        void (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9);
public:
       inline VoidMappedMethod10(  void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=10) throw new InvalidParams(10,v.count(),c);
             (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c), v[8].get<T8>(c), v[9].get<T9>(c));
       }    
};

template< typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
AbstractProperty * createVoidMethodBinding( void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9)  )
{
 return new VoidMappedMethod10<_Class,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>(callback);
}
template<typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class VoidMappedMethod11: public MappedMethod<_Class>
{
private:
        void (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10);
public:
       inline VoidMappedMethod11(  void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=11) throw new InvalidParams(11,v.count(),c);
             (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c), v[8].get<T8>(c), v[9].get<T9>(c), v[10].get<T10>(c));
       }    
};

template< typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
AbstractProperty * createVoidMethodBinding( void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10)  )
{
 return new VoidMappedMethod11<_Class,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10>(callback);
}
template<typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class VoidMappedMethod12: public MappedMethod<_Class>
{
private:
        void (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11);
public:
       inline VoidMappedMethod12(  void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=12) throw new InvalidParams(12,v.count(),c);
             (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c), v[8].get<T8>(c), v[9].get<T9>(c), v[10].get<T10>(c), v[11].get<T11>(c));
       }    
};

template< typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
AbstractProperty * createVoidMethodBinding( void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11)  )
{
 return new VoidMappedMethod12<_Class,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11>(callback);
}
template<typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class VoidMappedMethod13: public MappedMethod<_Class>
{
private:
        void (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12);
public:
       inline VoidMappedMethod13(  void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=13) throw new InvalidParams(13,v.count(),c);
             (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c), v[8].get<T8>(c), v[9].get<T9>(c), v[10].get<T10>(c), v[11].get<T11>(c), v[12].get<T12>(c));
       }    
};

template< typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
AbstractProperty * createVoidMethodBinding( void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12)  )
{
 return new VoidMappedMethod13<_Class,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12>(callback);
}
template<typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class VoidMappedMethod14: public MappedMethod<_Class>
{
private:
        void (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13);
public:
       inline VoidMappedMethod14(  void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=14) throw new InvalidParams(14,v.count(),c);
             (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c), v[8].get<T8>(c), v[9].get<T9>(c), v[10].get<T10>(c), v[11].get<T11>(c), v[12].get<T12>(c), v[13].get<T13>(c));
       }    
};

template< typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
AbstractProperty * createVoidMethodBinding( void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13)  )
{
 return new VoidMappedMethod14<_Class,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13>(callback);
}
template<typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class VoidMappedMethod15: public MappedMethod<_Class>
{
private:
        void (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13, T14 a14);
public:
       inline VoidMappedMethod15(  void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13, T14 a14)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=15) throw new InvalidParams(15,v.count(),c);
             (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c), v[8].get<T8>(c), v[9].get<T9>(c), v[10].get<T10>(c), v[11].get<T11>(c), v[12].get<T12>(c), v[13].get<T13>(c), v[14].get<T14>(c));
       }    
};

template< typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
AbstractProperty * createVoidMethodBinding( void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13, T14 a14)  )
{
 return new VoidMappedMethod15<_Class,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14>(callback);
}
template<typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class VoidMappedMethod16: public MappedMethod<_Class>
{
private:
        void (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13, T14 a14, T15 a15);
public:
       inline VoidMappedMethod16(  void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13, T14 a14, T15 a15)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=16) throw new InvalidParams(16,v.count(),c);
             (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c), v[8].get<T8>(c), v[9].get<T9>(c), v[10].get<T10>(c), v[11].get<T11>(c), v[12].get<T12>(c), v[13].get<T13>(c), v[14].get<T14>(c), v[15].get<T15>(c));
       }    
};

template< typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
AbstractProperty * createVoidMethodBinding( void (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13, T14 a14, T15 a15)  )
{
 return new VoidMappedMethod16<_Class,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>(callback);
}
template<typename _ReturnType,typename _Class>
class NonVoidMappedMethod0: public ReturnMappedMethod<_Class,_ReturnType>
{
private:
        _ReturnType (_Class::*m_callback)();
public:
       inline NonVoidMappedMethod0(  _ReturnType (_Class::*callback)()  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=0) throw new InvalidParams(0,v.count(),c);
             m_return_data = (this->realObject()->*m_callback)();
       }    
};

template<typename _ReturnType, typename _Class >
AbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)()  )
{
 return new NonVoidMappedMethod0<_ReturnType,_Class>(callback);
}
template<typename _ReturnType,typename _Class, typename T0>
class NonVoidMappedMethod1: public ReturnMappedMethod<_Class,_ReturnType>
{
private:
        _ReturnType (_Class::*m_callback)(T0 a0);
public:
       inline NonVoidMappedMethod1(  _ReturnType (_Class::*callback)(T0 a0)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=1) throw new InvalidParams(1,v.count(),c);
             m_return_data = (this->realObject()->*m_callback)(v[0].get<T0>(c));
       }    
};

template<typename _ReturnType, typename _Class , typename T0>
AbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)(T0 a0)  )
{
 return new NonVoidMappedMethod1<_ReturnType,_Class,T0>(callback);
}
template<typename _ReturnType,typename _Class, typename T0, typename T1>
class NonVoidMappedMethod2: public ReturnMappedMethod<_Class,_ReturnType>
{
private:
        _ReturnType (_Class::*m_callback)(T0 a0, T1 a1);
public:
       inline NonVoidMappedMethod2(  _ReturnType (_Class::*callback)(T0 a0, T1 a1)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=2) throw new InvalidParams(2,v.count(),c);
             m_return_data = (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c));
       }    
};

template<typename _ReturnType, typename _Class , typename T0, typename T1>
AbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)(T0 a0, T1 a1)  )
{
 return new NonVoidMappedMethod2<_ReturnType,_Class,T0,T1>(callback);
}
template<typename _ReturnType,typename _Class, typename T0, typename T1, typename T2>
class NonVoidMappedMethod3: public ReturnMappedMethod<_Class,_ReturnType>
{
private:
        _ReturnType (_Class::*m_callback)(T0 a0, T1 a1, T2 a2);
public:
       inline NonVoidMappedMethod3(  _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=3) throw new InvalidParams(3,v.count(),c);
             m_return_data = (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c));
       }    
};

template<typename _ReturnType, typename _Class , typename T0, typename T1, typename T2>
AbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2)  )
{
 return new NonVoidMappedMethod3<_ReturnType,_Class,T0,T1,T2>(callback);
}
template<typename _ReturnType,typename _Class, typename T0, typename T1, typename T2, typename T3>
class NonVoidMappedMethod4: public ReturnMappedMethod<_Class,_ReturnType>
{
private:
        _ReturnType (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3);
public:
       inline NonVoidMappedMethod4(  _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=4) throw new InvalidParams(4,v.count(),c);
             m_return_data = (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c));
       }    
};

template<typename _ReturnType, typename _Class , typename T0, typename T1, typename T2, typename T3>
AbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3)  )
{
 return new NonVoidMappedMethod4<_ReturnType,_Class,T0,T1,T2,T3>(callback);
}
template<typename _ReturnType,typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4>
class NonVoidMappedMethod5: public ReturnMappedMethod<_Class,_ReturnType>
{
private:
        _ReturnType (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4);
public:
       inline NonVoidMappedMethod5(  _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=5) throw new InvalidParams(5,v.count(),c);
             m_return_data = (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c));
       }    
};

template<typename _ReturnType, typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4>
AbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4)  )
{
 return new NonVoidMappedMethod5<_ReturnType,_Class,T0,T1,T2,T3,T4>(callback);
}
template<typename _ReturnType,typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class NonVoidMappedMethod6: public ReturnMappedMethod<_Class,_ReturnType>
{
private:
        _ReturnType (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5);
public:
       inline NonVoidMappedMethod6(  _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=6) throw new InvalidParams(6,v.count(),c);
             m_return_data = (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c));
       }    
};

template<typename _ReturnType, typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
AbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)  )
{
 return new NonVoidMappedMethod6<_ReturnType,_Class,T0,T1,T2,T3,T4,T5>(callback);
}
template<typename _ReturnType,typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class NonVoidMappedMethod7: public ReturnMappedMethod<_Class,_ReturnType>
{
private:
        _ReturnType (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6);
public:
       inline NonVoidMappedMethod7(  _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=7) throw new InvalidParams(7,v.count(),c);
             m_return_data = (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c));
       }    
};

template<typename _ReturnType, typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
AbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6)  )
{
 return new NonVoidMappedMethod7<_ReturnType,_Class,T0,T1,T2,T3,T4,T5,T6>(callback);
}
template<typename _ReturnType,typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class NonVoidMappedMethod8: public ReturnMappedMethod<_Class,_ReturnType>
{
private:
        _ReturnType (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7);
public:
       inline NonVoidMappedMethod8(  _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=8) throw new InvalidParams(8,v.count(),c);
             m_return_data = (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c));
       }    
};

template<typename _ReturnType, typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
AbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7)  )
{
 return new NonVoidMappedMethod8<_ReturnType,_Class,T0,T1,T2,T3,T4,T5,T6,T7>(callback);
}
template<typename _ReturnType,typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class NonVoidMappedMethod9: public ReturnMappedMethod<_Class,_ReturnType>
{
private:
        _ReturnType (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8);
public:
       inline NonVoidMappedMethod9(  _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=9) throw new InvalidParams(9,v.count(),c);
             m_return_data = (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c), v[8].get<T8>(c));
       }    
};

template<typename _ReturnType, typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
AbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8)  )
{
 return new NonVoidMappedMethod9<_ReturnType,_Class,T0,T1,T2,T3,T4,T5,T6,T7,T8>(callback);
}
template<typename _ReturnType,typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class NonVoidMappedMethod10: public ReturnMappedMethod<_Class,_ReturnType>
{
private:
        _ReturnType (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9);
public:
       inline NonVoidMappedMethod10(  _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=10) throw new InvalidParams(10,v.count(),c);
             m_return_data = (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c), v[8].get<T8>(c), v[9].get<T9>(c));
       }    
};

template<typename _ReturnType, typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
AbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9)  )
{
 return new NonVoidMappedMethod10<_ReturnType,_Class,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>(callback);
}
template<typename _ReturnType,typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class NonVoidMappedMethod11: public ReturnMappedMethod<_Class,_ReturnType>
{
private:
        _ReturnType (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10);
public:
       inline NonVoidMappedMethod11(  _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=11) throw new InvalidParams(11,v.count(),c);
             m_return_data = (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c), v[8].get<T8>(c), v[9].get<T9>(c), v[10].get<T10>(c));
       }    
};

template<typename _ReturnType, typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
AbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10)  )
{
 return new NonVoidMappedMethod11<_ReturnType,_Class,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10>(callback);
}
template<typename _ReturnType,typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class NonVoidMappedMethod12: public ReturnMappedMethod<_Class,_ReturnType>
{
private:
        _ReturnType (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11);
public:
       inline NonVoidMappedMethod12(  _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=12) throw new InvalidParams(12,v.count(),c);
             m_return_data = (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c), v[8].get<T8>(c), v[9].get<T9>(c), v[10].get<T10>(c), v[11].get<T11>(c));
       }    
};

template<typename _ReturnType, typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
AbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11)  )
{
 return new NonVoidMappedMethod12<_ReturnType,_Class,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11>(callback);
}
template<typename _ReturnType,typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class NonVoidMappedMethod13: public ReturnMappedMethod<_Class,_ReturnType>
{
private:
        _ReturnType (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12);
public:
       inline NonVoidMappedMethod13(  _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=13) throw new InvalidParams(13,v.count(),c);
             m_return_data = (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c), v[8].get<T8>(c), v[9].get<T9>(c), v[10].get<T10>(c), v[11].get<T11>(c), v[12].get<T12>(c));
       }    
};

template<typename _ReturnType, typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
AbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12)  )
{
 return new NonVoidMappedMethod13<_ReturnType,_Class,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12>(callback);
}
template<typename _ReturnType,typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
class NonVoidMappedMethod14: public ReturnMappedMethod<_Class,_ReturnType>
{
private:
        _ReturnType (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13);
public:
       inline NonVoidMappedMethod14(  _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=14) throw new InvalidParams(14,v.count(),c);
             m_return_data = (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c), v[8].get<T8>(c), v[9].get<T9>(c), v[10].get<T10>(c), v[11].get<T11>(c), v[12].get<T12>(c), v[13].get<T13>(c));
       }    
};

template<typename _ReturnType, typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
AbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13)  )
{
 return new NonVoidMappedMethod14<_ReturnType,_Class,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13>(callback);
}
template<typename _ReturnType,typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
class NonVoidMappedMethod15: public ReturnMappedMethod<_Class,_ReturnType>
{
private:
        _ReturnType (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13, T14 a14);
public:
       inline NonVoidMappedMethod15(  _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13, T14 a14)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=15) throw new InvalidParams(15,v.count(),c);
             m_return_data = (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c), v[8].get<T8>(c), v[9].get<T9>(c), v[10].get<T10>(c), v[11].get<T11>(c), v[12].get<T12>(c), v[13].get<T13>(c), v[14].get<T14>(c));
       }    
};

template<typename _ReturnType, typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
AbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13, T14 a14)  )
{
 return new NonVoidMappedMethod15<_ReturnType,_Class,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14>(callback);
}
template<typename _ReturnType,typename _Class, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
class NonVoidMappedMethod16: public ReturnMappedMethod<_Class,_ReturnType>
{
private:
        _ReturnType (_Class::*m_callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13, T14 a14, T15 a15);
public:
       inline NonVoidMappedMethod16(  _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13, T14 a14, T15 a15)  ) { m_callback=callback }
       virtual void call(const sad::VariantVector & v,ActionContext * c)
       {    
             if (v.count()!=16) throw new InvalidParams(16,v.count(),c);
             m_return_data = (this->realObject()->*m_callback)(v[0].get<T0>(c), v[1].get<T1>(c), v[2].get<T2>(c), v[3].get<T3>(c), v[4].get<T4>(c), v[5].get<T5>(c), v[6].get<T6>(c), v[7].get<T7>(c), v[8].get<T8>(c), v[9].get<T9>(c), v[10].get<T10>(c), v[11].get<T11>(c), v[12].get<T12>(c), v[13].get<T13>(c), v[14].get<T14>(c), v[15].get<T15>(c));
       }    
};

template<typename _ReturnType, typename _Class , typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
AbstractProperty * createNonVoidMethodBinding( _ReturnType (_Class::*callback)(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8, T9 a9, T10 a10, T11 a11, T12 a12, T13 a13, T14 a14, T15 a15)  )
{
 return new NonVoidMappedMethod16<_ReturnType,_Class,T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>(callback);
}
