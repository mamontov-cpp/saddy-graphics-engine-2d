/*! \file classmetadatacastfunction.h
	\author HiddenSeeker

	A function, that could perform user-defined cast from sad::Object from one real-type to sad::Object to other real-type.
	Could be used to define getters or something
 */
#pragma once

namespace sad
{
class Object;

/*! A class, that could perform user-defined cast from sad::Object of one type
 */
class AbstractClassMetaDataCastFunction
{
public:
	/*! Casts one object to another
		\param[in] o a metadata function for class
		\return other object, which was casted
	 */
	virtual sad::Object * cast(sad::Object * o) = 0;
	/*! This class can be inherited to implement your own casts
	 */
	virtual ~AbstractClassMetaDataCastFunction();
};

/*! A cast function, which represents a method call on _SourceType, that returns a sad::Object, or inherited from it
 */
template<
	typename _SourceType,
	typename _Method
>
class ClassMetaDataCastMethod: public AbstractClassMetaDataCastFunction
{
public:
	/*! Creates new method for casted
		\parma[in] f a casted function
	 */
	ClassMetaDataCastMethod(_Method f) : m_f(f)
	{

	}
	/*! Casts one object to another
		\param[in] o a metadata function for class
		\return other object, which was casted
	 */
	virtual sad::Object * cast(sad::Object * o)
	{
		return (static_cast<_SourceType*>(o)->*m_f)();
	}
	/*! This class can be inherited
	 */
	~ClassMetaDataCastMethod()
	{
	}
protected:
	/*! A called method for returning result
	 */
	_Method m_f;
};

/*! Created cast metadata function family
 */
template<typename _SourceClass>
class MetaDataCastFunctionFamily
{
public:
	/*! Creates new cast method
		\param[in] f a method
		\return a method
	 */
	template<typename _Method>
	static inline ClassMetaDataCastMethod<_SourceClass, _Method> * cast(_Method f)
	{
		return new ClassMetaDataCastMethod<_SourceClass, _Method>(f);
	}

};



}
