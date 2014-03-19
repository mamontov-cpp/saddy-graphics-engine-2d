/*! \file dbconversiontable.h
	\author HiddenSeeker

	Defines a table for conversion between types
 */
#pragma once
#include "../sadhash.h"
#include "../sadstring.h"


namespace sad
{
	
namespace db
{

/*! An abstract class, which converts one class to other
 */
class TypeConverter
{
/*! Must perform conversion and assignment from source type, in source pointer
	to destination type dest
	\param[in] source source converted type
	\param[in] dest destination converted type
 */
virtual void convert(void * source, void * dest) = 0;
/*! This class should be inherited
 */
virtual ~TypeConverter();
};

}

}
