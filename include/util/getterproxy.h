/*! \file getterproxy.h
    

    Describes proxy for getters
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include "../db/dbvariant.h"

namespace sad
{

namespace util
{

namespace getter
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
    {

    }
    /*! Returns a proxy for method pair
        \param[in] o object, for which proxy should be get
        \return field type name
     */
    virtual _FieldTypeName get(_Object const* o) = 0;
    /*! Clones a proxy
        \return proxy object
     */
    virtual sad::util::getter::Proxy<_Object, _FieldTypeName> * clone() = 0;
    /*! Can be inherited
     */
    virtual ~Proxy()
    {

    }
};

template<
    typename _Object,
    typename _FieldTypeName
>
class ProxyNCNR: public sad::util::getter::Proxy<_Object, _FieldTypeName>
{
public:
    /*! Creates new proxy
     */
    ProxyNCNR(_FieldTypeName (_Object::*f)()) : m_f(f)
    {

    }
    /*! Returns a proxy for method pair
        \param[in] o object, for which proxy should be get
        \return field type name
     */
    virtual _FieldTypeName get(_Object const* o)  override
    {
        return (const_cast<_Object *>(o)->*m_f)();
    }
    
    /*! Clones a proxy
        \return proxy object
     */
    virtual sad::util::getter::Proxy<_Object, _FieldTypeName> * clone() override
    {
        return new sad::util::getter::ProxyNCNR<_Object, _FieldTypeName>(m_f);
    }
    /*! Can be inherited
     */
    virtual ~ProxyNCNR() override
    {

    }
protected:
    _FieldTypeName (_Object::*m_f)();
};

template<
    typename _Object,
    typename _FieldTypeName
>
class ProxyCNR: public sad::util::getter::Proxy<_Object, _FieldTypeName>
{
public:
    /*! Creates new proxy
     */
    ProxyCNR(_FieldTypeName (_Object::*f)() const) : m_f(f)
    {

    }
    /*! Returns a proxy for method pair
        \param[in] o object, for which proxy should be get
        \return field type name
     */
    virtual _FieldTypeName get(_Object const* o)  override
    {
        return (o->*m_f)();
    }
    /*! Clones a proxy
        \return proxy object
     */
    virtual sad::util::getter::Proxy<_Object, _FieldTypeName> * clone() override
    {
        return new sad::util::getter::ProxyCNR<_Object, _FieldTypeName>(m_f);
    }
    /*! Can be inherited
     */
    virtual ~ProxyCNR() override
    {

    }
protected:
    _FieldTypeName (_Object::*m_f)() const;
};


template<
    typename _Object,
    typename _FieldTypeName
>
class ProxyNCMR: public sad::util::getter::Proxy<_Object, _FieldTypeName>
{
public:
    /*! Creates new proxy
     */
    ProxyNCMR(_FieldTypeName & (_Object::*f)()) : m_f(f)
    {

    }
    /*! Returns a proxy for method pair
        \param[in] o object, for which proxy should be get
        \return field type name
     */
    virtual _FieldTypeName get(_Object const* o) override
    {
        return (const_cast<_Object *>(o)->*m_f)();
    }
    /*! Clones a proxy
        \return proxy object
     */
    virtual sad::util::getter::Proxy<_Object, _FieldTypeName> * clone() override
    {
        return new sad::util::getter::ProxyNCMR<_Object, _FieldTypeName>(m_f);
    }
    /*! Can be inherited
     */
    virtual ~ProxyNCMR() override
    {

    }
protected:
    _FieldTypeName & (_Object::*m_f)();
};

template<
    typename _Object,
    typename _FieldTypeName
>
class ProxyCMR: public sad::util::getter::Proxy<_Object, _FieldTypeName>
{
public:
    /*! Creates new proxy
     */
    ProxyCMR(_FieldTypeName & (_Object::*f)() const) : m_f(f)
    {

    }
    /*! Returns a proxy for method pair
        \param[in] o object, for which proxy should be get
        \return field type name
     */
    virtual _FieldTypeName get(_Object const* o) override
    {
        return (o->*m_f)();
    }
    /*! Clones a proxy
        \return proxy object
     */
    virtual sad::util::getter::Proxy<_Object, _FieldTypeName> * clone() override
    {
        return new sad::util::getter::ProxyCMR<_Object, _FieldTypeName>(m_f);
    }
    /*! Can be inherited
     */
    virtual ~ProxyCMR() override
    {

    }
protected:
    _FieldTypeName & (_Object::*m_f)() const;
};

template<
    typename _Object,
    typename _FieldTypeName
>
class ProxyNCCR: public sad::util::getter::Proxy<_Object, _FieldTypeName>
{
public:
    /*! Creates new proxy
     */
    ProxyNCCR(const _FieldTypeName & (_Object::*f)()) : m_f(f)
    {

    }
    /*! Returns a proxy for method pair
        \param[in] o object, for which proxy should be get
        \return field type name
     */
    virtual _FieldTypeName get(_Object const* o) override
    {
        return (const_cast<_Object*>(o)->*m_f)();
    }
    /*! Clones a proxy
        \return proxy object
     */
    virtual sad::util::getter::Proxy<_Object, _FieldTypeName> * clone() override
    {
        return new sad::util::getter::ProxyNCCR<_Object, _FieldTypeName>(m_f);
    }
    /*! Can be inherited
     */
    virtual ~ProxyNCCR() override
    {

    }
protected:
    const _FieldTypeName & (_Object::*m_f)();
};

template<
    typename _Object,
    typename _FieldTypeName
>
class ProxyCCR: public sad::util::getter::Proxy<_Object, _FieldTypeName>
{
public:
    /*! Creates new proxy
     */
    ProxyCCR(const _FieldTypeName & (_Object::*f)() const) : m_f(f)
    {

    }
    /*! Returns a proxy for method pair
        \param[in] o object, for which proxy should be get
        \return field type name
     */
    virtual _FieldTypeName get(_Object const* o)  override
    {
        return (o->*m_f)();
    }
    /*! Clones a proxy
        \return proxy object
     */
    virtual sad::util::getter::Proxy<_Object, _FieldTypeName> * clone() override
    {
        return new sad::util::getter::ProxyCCR<_Object, _FieldTypeName>(m_f);
    }
    /*! Can be inherited
     */
    virtual ~ProxyCCR() override
    {

    }
protected:
    const _FieldTypeName & (_Object::*m_f)() const ;
};


}

template<
    typename _Object,
    typename _FieldTypeName
>
sad::util::getter::Proxy<_Object, _FieldTypeName> * define_getter(_FieldTypeName (_Object::*f)())
{
    return new sad::util::getter::ProxyNCNR<_Object, _FieldTypeName>(f);
}

template<
    typename _Object,
    typename _FieldTypeName
>
sad::util::getter::Proxy<_Object, _FieldTypeName> * define_getter(_FieldTypeName (_Object::*f)() const)
{
    return new sad::util::getter::ProxyCNR<_Object, _FieldTypeName>(f);
}

template<
    typename _Object,
    typename _FieldTypeName
>
sad::util::getter::Proxy<_Object, _FieldTypeName> * define_getter(_FieldTypeName & (_Object::*f)())
{
    return new sad::util::getter::ProxyNCMR<_Object, _FieldTypeName>(f);
}

template<
    typename _Object,
    typename _FieldTypeName
>
sad::util::getter::Proxy<_Object, _FieldTypeName> * define_getter(_FieldTypeName & (_Object::*f)() const)
{
    return new sad::util::getter::ProxyCMR<_Object, _FieldTypeName>(f);
}

template<
    typename _Object,
    typename _FieldTypeName
>
sad::util::getter::Proxy<_Object, _FieldTypeName> * define_getter(const _FieldTypeName & (_Object::*f)())
{
    return new sad::util::getter::ProxyNCCR<_Object, _FieldTypeName>(f);
}

template<
    typename _Object,
    typename _FieldTypeName
>
sad::util::getter::Proxy<_Object, _FieldTypeName> * define_getter(const _FieldTypeName & (_Object::*f)() const)
{
    return new sad::util::getter::ProxyCCR<_Object, _FieldTypeName>(f);
}

}

}
