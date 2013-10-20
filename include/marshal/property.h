/*! \file property.h
	\author HiddenSeeker

	Defines a property and editor, used a base classes by user for implementing own functions
 */
#include "saveloadcallbacks.h"

#include "../unused.h"
#pragma once

/*! \class PropertyListener
   Defines a listener, that will be notified, that props has changed
 */
template<typename T>
class PropertyListener
{
 public:
	    /*! Method, that are called when property is notified
		 */
		virtual void notify(const T & data)=0;
		/*! Destructor
		 */
		virtual ~PropertyListener() {}
};

/*! A field, that represents a field of class, computational or not
 */
template<typename T>
class AbstractField: public AbstractProperty
{
 protected:
			bool m_already_changed;            //!< Determines whether set() is called from another set
			PropertyListener<T> * m_listener;  //!< Listener for a property
			/*! Sets a new value. Called from set to handle all work
			 */
			virtual void _set(const T & new_value)=0;
 public:
	        inline AbstractField() { m_already_changed = false, m_listener=NULL;}
			/*! Sets a new listener for property
				\param[in] listener listener data
			 */
			inline void setListener(PropertyListener<T> * listener)
			{ m_listener = listener; }
			/*! Sets a property value
			 */
			virtual void set(const sad::Variant & value)
			{
				//Guard condition because listener can call set from notify and thus 
				//have recursion call
				if (m_already_changed) return;

				m_already_changed = true;
				if (m_listener)
					m_listener->notify(value.get<T>());
				m_already_changed = false;

				_set(value.get<T>());
			}

			/*! Returns a string representation of property
			    \return string representation
		     */
	        virtual sad::String save() const
			{
                return SaveLoadCallback< T >::save( get()->template get< T > ());
			}

			/*! Loads data from string
				\param[in] str      string data
				\param[in] context  action context
			 */
			virtual void load(const sad::String & str)
			{
				_set(SaveLoadCallback<T>::load(str));
			}

			/*! Whether we should load this property. Methods won't be saveable
			 */
			virtual bool saveable() const { return true; }
};

/*! A field, that is mapped to a real object field
 */
template<typename T>
class MappedField: public AbstractField<T>
{
 protected:
			 T * m_real_field;  //!< Real object field;
			 sad::Variant * m_variant; //!< Variant data
			 /*! Sets a new value. Called from set to handle all work
			  */
			 virtual void _set(const T & new_value)
			 { *m_real_field = new_value; }
 public:
			/*! Constructs new field with initial value
				\param[in] field_ptr field pointer
				\param[in] init      initial value
			 */
			inline MappedField( T * field_ptr, const T & init)
			{
				m_real_field = field_ptr;
				*m_real_field = init;
				m_variant = NULL;
			}
			/*! Returns a property value
			 */
            virtual sad::Variant * get() const
			{
				delete m_variant;
				const_cast<MappedField<T> *>(this)->m_variant = new sad::Variant(*m_real_field);
				return m_variant;
			}

			~MappedField()
			{
				delete m_variant;
			}
};

/*! This is a method type, which maps to method of target class, allowing
	to call it via accessing it with a string. A method must not return anything
 */
template<typename _RealSerializable>
class MappedMethod: public AbstractProperty
{
public:
	/*! Returns true, since we can call this method, by supplying vector of 
		variant in runtime
		\return true
	 */
	virtual bool callable() 
	{ 
		return true;
	}
protected:
	/*! Returns real object, which method attached to
		\return object
	 */
	inline _RealSerializable realObject() 
	{ 
		return  static_cast<_RealSerializable*>(getParent());
	}
};

/*! This is a method type, which maps to method of target class, allowing
	to call it via accessing it with a string. A method can returns a value, which
	can be accessed via returnValue method.
 */
template<typename _RealSerializable, typename _ReturnData>
class ReturnMappedMethod: MappedMethod<_RealSerializable>
{
public:
	/*! Returns new value, returned by mapped method.
		NOTE: You must free returned sad::Variant. A mapped method,
		does not hold it
		\return a value of method
	 */
	sad::Variant * returnValue() const 
	{ 
		return new sad::Variant(m_return_data); 
	} 
	/*! Determined, whether method returns a value.
		\return true
	 */
	bool returnsValue() const  
	{ 
		return true; 
	}
protected:
	/*! A value, returned by method
	 */
	_ReturnData m_return_data; 
};

#include "templatemappings.h"

typedef MappedField<bool>                  BoolMappedField;
typedef MappedField<float>                 FloatMappedField;
typedef MappedField<double>                DoubleMappedField;
typedef MappedField<int>                   IntMappedField;
typedef MappedField<unsigned int>          UIntMappedField;
typedef MappedField<long>                  LongMappedField;
typedef MappedField<unsigned long>         ULongMappedField;
typedef MappedField<long long>             LongLongMappedField;
typedef MappedField<unsigned long long>    ULongLongMappedField;
typedef MappedField<sad::String>           HStringMappedField;
typedef MappedField< sad::Vector<int> >    HIntVectorMappedField;
typedef MappedField<sad::Point2D>                   PointMappedField;
typedef MappedField<sad::Rect2D>                    RectMappedField;
typedef MappedField< sad::Vector<sad::Point2D> >    PointVectorMappedField;
typedef MappedField< sad::Color >			   ColorMappedField;
