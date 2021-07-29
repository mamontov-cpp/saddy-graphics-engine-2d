#pragma once
#include "../3rdparty/format/format.h"

namespace sad
{

namespace formattedlabel
{
class Arg
{
public:
    /*! Returns substituted arg in notions of format library
        \return substituted argument
     */
    virtual fmt::BasicFormatter<char>::Arg * substitute() = 0;
    virtual ~Arg();
};
template<
typename _Value
>
class StaticArg: public sad::formattedlabel::Arg
{
private:
    _Value m_arg; //!< Argument for substitution
public:
    StaticArg(const _Value & arg) : m_arg(arg) { }
    virtual fmt::BasicFormatter<char>::Arg * substitute()  override
    { 
        return new fmt::BasicFormatter<char>::Arg(m_arg);
    }
};
template<
 typename _Value
>
class ZeroFunctionArg: public sad::formattedlabel::Arg
{
private:
    _Value (*m_f)(); 
public:
    ZeroFunctionArg(_Value  (*f)()) : m_f(f) { }
    virtual fmt::BasicFormatter<char>::Arg * substitute()  override
    { 
        return new fmt::BasicFormatter<char>::Arg( m_f() );
    }
};
template<
typename _Class,
typename _Value
>
class ZeroMethodArg: public sad::formattedlabel::Arg
{
private:
    _Class * m_o;
    _Value (_Class::*m_f)(); 
public:
    ZeroMethodArg(_Class * o, _Value (_Class::*f)()) : m_o(o), m_f(f) { }
    virtual fmt::BasicFormatter<char>::Arg * substitute()  override
    { 
        return new fmt::BasicFormatter<char>::Arg( (m_o->*m_f)() );
    }
};
template<
typename _Class,
typename _Value
>
class ZeroConstMethodArg: public sad::formattedlabel::Arg
{
private:
    _Class * m_o;
    _Value (_Class::*m_f)() const; 
public:
    ZeroConstMethodArg(_Class * o, _Value (_Class::*f)() const) : m_o(o), m_f(f) { }
    virtual fmt::BasicFormatter<char>::Arg * substitute()  override
    { 
        return new fmt::BasicFormatter<char>::Arg( (m_o->*m_f)() );
    }
};
template<
typename _RetValue,
typename _Class,
typename _Value
>
class ZeroCastedConstMethodArg: public sad::formattedlabel::Arg
{
private:
    _Class * m_o;
    _Value (_Class::*m_f)() const; 
public:
    ZeroCastedConstMethodArg(_Class * o, _Value (_Class::*f)() const) : m_o(o), m_f(f) { }
    virtual fmt::BasicFormatter<char>::Arg * substitute()  override
    { 
        return new fmt::BasicFormatter<char>::Arg( static_cast<_RetValue>((m_o->*m_f)()) );
    }
};
template<
typename _Class1,
typename _Class2,
typename _Value
>
class ZeroMethodsCompositionArg: public sad::formattedlabel::Arg
{
private:
    _Class1 * m_o;
    _Class2 * (_Class1::*m_f)();
    _Value (_Class2::*m_g)();

public:
    ZeroMethodsCompositionArg(_Class1 * o, 
                              _Class2 * (_Class1::*f)(), 
                              _Value (_Class2::*g)() ) : m_o(o), m_f(f), m_g(g) 
    { 
    }
    virtual fmt::BasicFormatter<char>::Arg * substitute()  override
    { 
        _Class2 * p = (m_o->*m_f)();
        return new fmt::BasicFormatter<char>::Arg( (p->*m_g)() );
    }
};
template<
typename _Class1,
typename _Class2,
typename _Class3,
typename _Value
>
class ZeroCompatibleCompositionArg3C: public sad::formattedlabel::Arg
{
private:
    _Class1 * m_o;
    _Class2 * (_Class1::*m_f)();
    _Value (_Class3::*m_g)() const;

public:
    ZeroCompatibleCompositionArg3C(_Class1 * o, 
                                _Class2 * (_Class1::*f)(), 
                                _Value (_Class3::*g)() const ) : m_o(o), m_f(f), m_g(g) 
    { 
    }
    virtual fmt::BasicFormatter<char>::Arg * substitute()  override
    { 
        _Class2 * p = (m_o->*m_f)();
        _Class3 * k = static_cast<_Class3*>(p);
        return new fmt::BasicFormatter<char>::Arg( (k->*m_g)() );
    }
};

}

}
