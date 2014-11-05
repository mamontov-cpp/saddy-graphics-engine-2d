/*! \file animations/animationsanimationfastcall.h
	\author HiddenSeeker

	Defines a cached call, needed for more fast work of animations
 */
#pragma once
#include "../db/dbobject.h"

namespace sad
{

namespace animations
{

/*! Defines cached call, needed for more fast work of animations
 */
class AnimationFastCall
{
public:
	/*! Constructs new empty cached call
	 */
	inline AnimationFastCall() { }
	/*! Must be inherited
	 */
	virtual ~AnimationFastCall();
};

/*! Defines a fasdt animation call for argument
 */
template<
	typename T
>
class AnimationFastCallFor: public sad::animations::AnimationFastCall
{
public:
	/*! Constructs new empty cached call
	 */
	inline AnimationFastCallFor() { }
	/*! Calls an animation delegate for specified argument
		\param[in] a argument
	 */
	virtual void call(const T& a)
	{

	}
	/*! Could be inherited
	 */
	virtual ~AnimationFastCallFor() { }
};

/*! Defines a fast call as cast to object and calling a method on it
 */
template<
	typename _Object,
	typename _Method,
	typename _Argument
>
class FastCall : public sad::animations::AnimationFastCallFor<_Argument>
{
public:
	/*! Constructs new empty cached call
		\param[in]  o object object, which method should be called upon
		\param[in]  f a method, which should be called
	 */
	inline FastCall(sad::db::Object* o, _Method f) : m_o(o), m_f(f) { }
	/*! Calls an animation delegate for specified argument
		\param[in] a argument
	 */
	virtual void call(const _Argument& a)
	{
		(static_cast<_Object *>(m_o)->*m_f)(a);
	}
	virtual ~FastCall() { }
protected:
	/*! An object link for fast call
	 */
	sad::db::Object* m_o;
	/*! A method to be called on object
	 */
	_Method m_f;
};


/*! Defines a fast call as setting property for an object
 */
template<
	typename _Argument
>
class SetProperty : public sad::animations::AnimationFastCallFor<_Argument>
{
public:
	/*! Constructs new empty cached call
		\param[in]  o object object, which method should be called upon
		\param[in]  prop a property name to be set
	 */
	inline SetProperty(sad::db::Object* o, const sad::String& prop) : m_o(o), m_property_name(prop) { }
	/*! Calls an animation delegate for specified argument
		\param[in] a argument
	 */
	virtual void call(const _Argument& a)
	{
		m_o->setProperty(m_property_name, a);
	}
	virtual ~SetProperty() { }
protected:
	/*! An object link for fast call
	 */
	sad::db::Object* m_o;
	/*! A method to be called on object
	 */
	sad::String m_property_name;
};

/*! Makes fast call, based on method
	\param[in] o object
	\param[in] f method
	\return fast call
 */
template<
	typename _Object,
	typename _Arg
>
sad::animations::AnimationFastCall* make_fastcall(sad::db::Object* o, void (_Object::*f)(const _Arg&))
{
	return new sad::animations::FastCall<_Object, void (_Object::*)(const _Arg&), _Arg>(o, f);
}

/*! Makes fast call, based on method
	\param[in] o object
	\param[in] f method
	\return fast call
 */
template<
	typename _Object,
	typename _Arg
>
sad::animations::AnimationFastCall* make_fastcall(sad::db::Object* o, void (_Object::*f)(_Arg))
{
	return new sad::animations::FastCall<_Object, void (_Object::*)(_Arg), _Arg>(o, f);
}

/*! Makes fast call, based on method
	\param[in] o object
	\param[in] f method
	\return fast call
 */
template<
	typename _Object,
	typename _Arg
>
sad::animations::AnimationFastCall* make_fastcall(sad::db::Object* o, void (_Object::*f)(const _Arg&) const)
{
	return new sad::animations::FastCall<_Object, void (_Object::*)(const _Arg&) const, _Arg>(o, f);
}

/*! Makes fast call, based on method
	\param[in] o object
	\param[in] f method
	\return fast call
 */
template<
	typename _Object,
	typename _Arg
>
sad::animations::AnimationFastCall* make_fastcall(sad::db::Object* o, void (_Object::*f)(_Arg) const)
{
	return new sad::animations::FastCall<_Object, void (_Object::*)(_Arg) const, _Arg>(o, f);
}


}

}
