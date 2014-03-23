/*! \file dbconversiontable.h
	\author HiddenSeeker

	Defines a table for conversion between types
 */
#pragma once
#include "../sadptrhash.h"
#include "../sadstring.h"
#include "dbtypename.h"

namespace sad
{
	
namespace db
{

/*! An abstract class, which converts one class to other
 */
class AbstractTypeConverter
{
public:
/*! Must perform conversion and assignment from source type, in source pointer
	to destination type dest
	\param[in] source source converted type
	\param[in] dest destination converted type
 */
virtual void convert(void * source, void * dest) = 0;
/*! This class should be inherited
 */
virtual ~AbstractTypeConverter();
};

/**
 * A converter, which, using implicit casts converts one type to another 
 */
template<
	typename _FromType,
	typename _ToType
>
class ImplicitTypeConverter: public sad::db::AbstractTypeConverter
{
public:
	/*! Creates new converter
	 */
	inline ImplicitTypeConverter()
	{
		
	}
	/*! Converts source value from another and to another type
		\param[in] source source converted type
		\param[in] dest destination converted type
	 */
	virtual void convert(void * source, void * dest)
	{
		*((_ToType *)dest) =  (_ToType)(*(_FromType *)source);
	}
	/*! Can be inherited
	 */
	virtual ~ImplicitTypeConverter()
	{
		
	}
};

/** A list of conversion table for conversions of types
 */
class ConversionTable
{
	/*! Creates new conversion table
	 */
	ConversionTable();
	/*! Inserts a conveter into table. Replaces an existing converter, erasing it, if
		needed
		\param[in] from a typename from which conversion is performed
		\param[in] to a typename, which we should confirm
		\param[in] c a converter
	 */
	void add(const sad::String & from, 
			 const sad::String & to, 
			 sad::db::AbstractTypeConverter * c);
	/*! Declares implicit conversion from one type to other
	 */
	template< typename _FromType, typename _ToType>
	void declareImplicit()
	{
		add(sad::db::TypeName<_FromType>::Name, 
			sad::db::TypeName<_ToType>::Name,
			new sad::db::ImplicitTypeConverter<_FromType, _ToType>()
		   );
	}
	/*! Gets converter from one type to another
		\param[in] from a typename for conversion from type
		\param[in] to a typename for conversion to 
		\return NULL, if not found or converter
	 */
	sad::db::AbstractTypeConverter * converter(
		const sad::String & from, 
		const sad::String & to
	);
	/*! Returns instance of conversion table
		\return instance of table
	 */
	static sad::db::ConversionTable * ref();
protected:
	/*! Frees instance of conversion table
	 */
	static void freeInstance();
	/*! An instance for conversion table
	 */
	static sad::db::ConversionTable * m_instance;
	/*! A list of conversions for table
	 */
	sad::Hash<sad::String, sad::PtrHash<sad::String, sad::db::AbstractTypeConverter> > m_converters;
};

}

}
