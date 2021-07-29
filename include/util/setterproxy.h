/*! \file setterproxy.h
    

    Describes proxy for setters
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include "../db/dbvariant.h"

namespace sad
{

namespace util
{

namespace setter
{

template<
    typename _Object,
    typename _FieldTypeName
>
class Proxy
{
public:
    /*! Creates new proxy
     */
    Proxy()
    = default;
    /*! Returns a proxy for method pair
        \param[in] o object, for which proxy should be get
        \param[in] v a value
     */
    virtual void set(_Object * o, const _FieldTypeName & v) = 0;
    /*! Clones a proxy
        \return proxy object
     */
    virtual sad::util::setter::Proxy<_Object, _FieldTypeName> * clone() = 0;
    /*! Can be inherited
     */
	virtual ~Proxy() = default;
};

template<
    typename _Object,
    typename _FieldTypeName
>
class ProxyNCNR: public sad::util::setter::Proxy<_Object, _FieldTypeName>
{
public:
    /*! Creates new proxy
     */
    ProxyNCNR(void (_Object::*f)(_FieldTypeName)) : m_f(f)
    {

    }
    /*! Returns a proxy for method pair
        \param[in] o object, for which proxy should be get
        \param[in] v a value
     */
    virtual void set(_Object * o, const _FieldTypeName & v) override
    {
        (o->*m_f)(v);
    }
    /*! Clones a proxy
        \return proxy object
     */
    virtual sad::util::setter::Proxy<_Object, _FieldTypeName> * clone() override
    {
        return new sad::util::setter::ProxyNCNR<_Object, _FieldTypeName>(m_f);
    }
    /*! Can be inherited
     */
	virtual ~ProxyNCNR() override = default;
protected:
    void (_Object::*m_f)(_FieldTypeName);
};

template<
    typename _Object,
    typename _FieldTypeName
>
class ProxyCNR: public sad::util::setter::Proxy<_Object, _FieldTypeName>
{
public:
    /*! Creates new proxy
     */
    ProxyCNR(void (_Object::*f)(_FieldTypeName) const) : m_f(f)
    {

    }
    /*! Returns a proxy for method pair
        \param[in] o object, for which proxy should be get
        \param[in] v a value
        \return field type name
     */
    virtual void set(_Object * o, const _FieldTypeName & v) override
    {
        (o->*m_f)(v);
    }
    /*! Clones a proxy
        \return proxy object
     */
    virtual sad::util::setter::Proxy<_Object, _FieldTypeName> * clone() override
    {
        return new sad::util::setter::ProxyCNR<_Object, _FieldTypeName>(m_f);
    }
    /*! Can be inherited
     */
    virtual ~ProxyCNR() override
    {

    }
protected:
    void (_Object::*m_f)(_FieldTypeName) const;
};

template<
    typename _Object,
    typename _FieldTypeName
>
class ProxyNCMR: public sad::util::setter::Proxy<_Object, _FieldTypeName>
{
public:
    /*! Creates new proxy
     */
    ProxyNCMR(void (_Object::*f)(_FieldTypeName &)) : m_f(f)
    {

    }
    /*! Returns a proxy for method pair
        \param[in] o object, for which proxy should be get
        \param[in] v a value
        \return field type name
     */
    virtual void set(_Object * o, const _FieldTypeName & v) override
    {
        (o->*m_f)(const_cast<_FieldTypeName &>(v));
    }
    /*! Clones a proxy
        \return proxy object
     */
    virtual sad::util::setter::Proxy<_Object, _FieldTypeName> * clone() override
    {
        return new sad::util::setter::ProxyNCMR<_Object, _FieldTypeName>(m_f);
    }
    /*! Can be inherited
     */
	virtual ~ProxyNCMR()  override = default;
protected:
    void (_Object::*m_f)(_FieldTypeName &);
};

template<
    typename _Object,
    typename _FieldTypeName
>
class ProxyCMR: public sad::util::setter::Proxy<_Object, _FieldTypeName>
{
public:
    /*! Creates new proxy
     */
    ProxyCMR(void (_Object::*f)(_FieldTypeName &) const) : m_f(f)
    {

    }
    /*! Returns a proxy for method pair
        \param[in] o object, for which proxy should be get
        \param[in] v a value
        \return field type name
     */
    virtual void set(_Object * o, const _FieldTypeName & v) override
    {
        (o->*m_f)(const_cast<_FieldTypeName &>(v));
    }
    /*! Clones a proxy
        \return proxy object
     */
    virtual sad::util::setter::Proxy<_Object, _FieldTypeName> * clone() override
    {
        return new sad::util::setter::ProxyCMR<_Object, _FieldTypeName>(m_f);
    }
    /*! Can be inherited
     */
	virtual ~ProxyCMR()  override = default;
protected:
    void (_Object::*m_f)(_FieldTypeName &) const;
};

template<
    typename _Object,
    typename _FieldTypeName
>
class ProxyNCCR: public sad::util::setter::Proxy<_Object, _FieldTypeName>
{
public:
    /*! Creates new proxy
     */
    ProxyNCCR(void (_Object::*f)(const _FieldTypeName&)) : m_f(f)
    {

    }
    /*! Returns a proxy for method pair
        \param[in] o object, for which proxy should be get
        \param[in] v a value
        \return field type name
     */
    virtual void set(_Object * o, const _FieldTypeName & v) override
    {
        (o->*m_f)(v);
    }
    /*! Clones a proxy
        \return proxy object
     */
    virtual sad::util::setter::Proxy<_Object, _FieldTypeName> * clone() override
    {
        return new sad::util::setter::ProxyNCCR<_Object, _FieldTypeName>(m_f);
    }
    /*! Can be inherited
     */
    virtual ~ProxyNCCR() override
    {

    }
protected:
    void (_Object::*m_f)(const _FieldTypeName&);
};

template<
    typename _Object,
    typename _FieldTypeName
>
class ProxyCCR: public sad::util::setter::Proxy<_Object, _FieldTypeName>
{
public:
    /*! Creates new proxy
     */
    ProxyCCR(void (_Object::*f)(const _FieldTypeName&) const) : m_f(f)
    {

    }
    /*! Returns a proxy for method pair
        \param[in] o object, for which proxy should be get
        \param[in] v a value
        \return field type name
     */
    virtual void set(_Object * o, const _FieldTypeName & v) override
    {
        (o->*m_f)(v);
    }
    /*! Clones a proxy
        \return proxy object
     */
    virtual sad::util::setter::Proxy<_Object, _FieldTypeName> * clone() override
    {
        return new sad::util::setter::ProxyCCR<_Object, _FieldTypeName>(m_f);
    }
    /*! Can be inherited
     */
	virtual ~ProxyCCR() = default;
protected:
    void (_Object::*m_f)(const _FieldTypeName&) const ;
};

}

template<
    typename _Object,
    typename _FieldTypeName
>
sad::util::setter::Proxy<_Object, _FieldTypeName> * define_setter(void (_Object::*f)(_FieldTypeName))
{
    return new sad::util::setter::ProxyNCNR<_Object, _FieldTypeName>(f);
}

template<
    typename _Object,
    typename _FieldTypeName
>
sad::util::setter::Proxy<_Object, _FieldTypeName> * define_setter(void (_Object::*f)(_FieldTypeName)  const)
{
    return new sad::util::setter::ProxyCNR<_Object, _FieldTypeName>(f);
}

template<
    typename _Object,
    typename _FieldTypeName
>
sad::util::setter::Proxy<_Object, _FieldTypeName> * define_setter(void (_Object::*f)(_FieldTypeName &) )
{
    return new sad::util::setter::ProxyNCMR<_Object, _FieldTypeName>(f);
}

template<
    typename _Object,
    typename _FieldTypeName
>
sad::util::setter::Proxy<_Object, _FieldTypeName> * define_setter(void (_Object::*f)(_FieldTypeName &) const)
{
    return new sad::util::setter::ProxyCMR<_Object, _FieldTypeName>(f);
}

template<
    typename _Object,
    typename _FieldTypeName
>
sad::util::setter::Proxy<_Object, _FieldTypeName> * define_setter(void (_Object::*f)(const _FieldTypeName &))
{
    return new sad::util::setter::ProxyNCCR<_Object, _FieldTypeName>(f);
}

template<
    typename _Object,
    typename _FieldTypeName
>
sad::util::setter::Proxy<_Object, _FieldTypeName> * define_setter(void (_Object::*f)(const _FieldTypeName &) const)
{
    return new sad::util::setter::ProxyCCR<_Object, _FieldTypeName>(f);
}

}

}
