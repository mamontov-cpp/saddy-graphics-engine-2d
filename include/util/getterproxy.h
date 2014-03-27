/*! \file getterproxy.h
	\author HiddenSeeker

	Describes proxy for getters
 */
#pragma once
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
	virtual _FieldTypeName get(_Object * o) = 0;
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
	virtual _FieldTypeName get(_Object * o) 
	{
		return (o->*m_f)();
	}
	/*! Can be inherited
	 */
	virtual ~ProxyNCNR()
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
	virtual _FieldTypeName get(_Object * o) 
	{
		return (o->*m_f)();
	}
	/*! Can be inherited
	 */
	virtual ~ProxyCNR()
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
	virtual _FieldTypeName get(_Object * o)
	{
		return (o->*m_f)();
	}
	/*! Can be inherited
	 */
	virtual ~ProxyNCMR()
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
	virtual _FieldTypeName get(_Object * o)
	{
		return (o->*m_f)();
	}
	/*! Can be inherited
	 */
	virtual ~ProxyCMR()
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
	virtual _FieldTypeName get(_Object * o)
	{
		return (o->*m_f)();
	}
	/*! Can be inherited
	 */
	virtual ~ProxyNCCR()
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
	virtual _FieldTypeName get(_Object * o) 
	{
		return (o->*m_f)();
	}
	/*! Can be inherited
	 */
	virtual ~ProxyCCR()
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
