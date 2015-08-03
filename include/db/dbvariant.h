/*! \file dbvariant.h
	
	
	Contains definition of class Variant, which could be used to
	box values of various types and work with them. 
	
	Note, that this is abstraction for value of property, not the property itself.
 */
#pragma once
#include "../sadstring.h"
#include "../maybe.h"
#include "dbconversiontable.h"
#include "saveloadfwd.h"
#include "../util/commoncheckedcast.h"


namespace sad
{

namespace db
{

namespace variant
{

/*! Copies a variant value
	\param[in] o a value
	\return value
 */
template<typename T>
void * copy_value(void * o)
{
	return new T(*(reinterpret_cast<T*>(o)));		
}

/*! Deletes a variant value
	\param[in] o a value
	\return value
 */
template<typename T>
void delete_value(void * o)
{
	delete reinterpret_cast<T*>(o);		
}

}

/*! \class Variant

	Could be used to box values of various types and work with them. 
	Note, that this is abstraction for value of property, not the property itself.
 */
class Variant   //-V690
{	
protected:
	/*! A boxed object in variant
	 */
	void * m_object;
	/*! Count of stars for base count
	 */
	int m_pointers_stars_count;
	/*! When type is pointer, this is part of type name without a pointer
	 */
	sad::String m_base_name;
	/*! Whether value of variant is sad object
	 */
	bool m_is_sad_object;
	/*! A name of type, boxed in variant
	 */
	sad::String  m_typename;
	/*! A function for copying a value
	 */
	void * (*m_copy)(void *);
	/*! A function for deleting a value
	 */
	void (*m_delete)(void *);
	/*! A function for saving a value
	 */
	picojson::value (*m_save)(void * ptr);
	/*! A function for loading value
	 */
	bool (*m_load)(void * ptr, const picojson::value & v);
	/*! Releases value
	 */
	void release();
	/*! Assigns a value
		\param[in] v value
		\return value
	 */
	void assign(const sad::db::Variant & v);
public:	
	/*! Construct an empty variant value
	 */
	Variant();
	/*! Copies a value into variant by copying stored values
		\param[in] v variant
	 */
	Variant(const sad::db::Variant & v);
	/*! Assigns a new value to variant by copying stored values
		\param[in] v variant
		\return self-reference
	 */
	Variant & operator=(const sad::db::Variant  & v);
	/*! A constructor, which assigns a value to a variant
		\param[in] v a new value for a variant
	 */
	template<typename T>
	Variant(T* v)
	{
		sad::db::TypeName<T *>::init();
		m_object = new T*(v);
		m_is_sad_object = sad::db::TypeName<T>::isSadObject();
		m_typename = sad::db::TypeName<T*>::name();
		m_copy = sad::db::variant::copy_value<T*>;
		m_delete = sad::db::variant::delete_value<T*>;
		m_save = sad::db::Save<T*>::perform;
		m_load = sad::db::Load<T*>::perform;
		m_pointers_stars_count = sad::db::TypeName<T *>::POINTER_STARS_COUNT;
		m_base_name = sad::db::TypeName<T *>::baseName();
	}
	/*! A constructor, which assigns a value to a variant
		\param[in] v a new value for a variant
	 */
	template<typename T>
	Variant(const T & v)
	{
		m_object = new T(v);
		m_is_sad_object = sad::db::TypeName<T>::isSadObject();
		m_typename = sad::db::TypeName<T>::name();
		m_copy = sad::db::variant::copy_value<T>;
		m_delete = sad::db::variant::delete_value<T>;
		m_save = sad::db::Save<T>::perform;
		m_load = sad::db::Load<T>::perform;
		m_pointers_stars_count = sad::db::TypeName<T>::POINTER_STARS_COUNT;
		m_base_name = sad::db::TypeName<T>::baseName();
	}
	/*! Frees a value from variant
	 */
	virtual ~Variant();
	/*! Sets a new value for variant
		\param[in] v new value for variant
	 */
	template<typename T>
	void set(T * v)
	{
		release();
		sad::db::TypeName<T *>::init();
		m_object = new T*(v);
		m_is_sad_object = sad::db::TypeName<T>::isSadObject();
		m_typename = sad::db::TypeName<T*>::name();
		m_copy = sad::db::variant::copy_value<T*>;
		m_delete = sad::db::variant::delete_value<T*>;
		m_save = sad::db::Save<T*>::perform;
		m_load = sad::db::Load<T*>::perform;
		m_pointers_stars_count = sad::db::TypeName<T *>::POINTER_STARS_COUNT;
		m_base_name = sad::db::TypeName<T *>::baseName();
	}
	/*! Sets a new value for variant
		\param[in] v new value for variant
	 */
	template<typename T>
	void set(const T & v)
	{
		release();
		m_object = new T(v);
		m_is_sad_object = sad::db::TypeName<T>::isSadObject();
		m_typename = sad::db::TypeName<T>::name();
		m_copy = sad::db::variant::copy_value<T>;
		m_delete = sad::db::variant::delete_value<T>;
		m_save = sad::db::Save<T>::perform;
		m_load = sad::db::Load<T>::perform;
		m_pointers_stars_count = sad::db::TypeName<T>::POINTER_STARS_COUNT;
		m_base_name = sad::db::TypeName<T>::baseName();
	}	
	/*! Returns a value for variant
        \param[in] ref whether we prefer to return by reference (if true), or by value (if false)
		\return value or throws exception if cannot cast
	 */
	template<typename T>
	sad::Maybe<T> get(bool ref = false) const
	{
		sad::Maybe<T> result;
		sad::db::TypeName<T>::init();		
		if (sad::db::TypeName<T>::name() == m_typename && sad::db::TypeName<T>::POINTER_STARS_COUNT == m_pointers_stars_count)
		{
            if (ref)
            {
			    result.setReference(reinterpret_cast<T*>(m_object));
            }
            else
            {
                result.setValue(*((T*)m_object));
            }
			return result;
		}
		if (sad::db::TypeName<T>::isSadObject() 
			&& m_is_sad_object 
			&& sad::db::TypeName<T>::POINTER_STARS_COUNT == m_pointers_stars_count
			&& m_pointers_stars_count == 1)
		{
			sad::util::CommonCheckedCast<T, sad::db::TypeName<T>::CAN_BE_CASTED_TO_OBJECT >::perform(
				result,
				m_object,
				sad::db::TypeName<T>::baseName()
			);	
			return result;
		}
		else
		{
			sad::db::AbstractTypeConverter * c = sad::db::ConversionTable::ref()
											  ->converter(m_typename, sad::db::TypeName<T>::name());
			if (c)
			{
				T tmp;
				c->convert(m_object, &tmp);
				result.setValue(tmp);
			}
		}
		return result;
	}
	/*! Saves a value to JSON value
		\throw throw sad::db::NotImplemented if not implemented
		\return a value
	 */
	picojson::value save() const;
	/*! Loads a value from JSON value
		\throw throw sad::db::NotImplemented if not implemented
		\param[in] v value
		\return whether it was successfull
	 */
	bool load(const picojson::value & v);
};

}

}
