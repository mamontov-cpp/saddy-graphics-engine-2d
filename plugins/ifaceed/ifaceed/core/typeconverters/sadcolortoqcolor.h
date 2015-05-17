/*! \file sadcolortoqcolor.h
	\author HiddenSeeker

	Defines a converter from sad::Color to QColor, used in conversion table
 */
#pragma once
#include <QColor>

#include <sadcolor.h>

#include <db/dbconversiontable.h>


namespace core
{

namespace typeconverters
{

/*! Defines a conversion from sad::Color to QColor
 */
class SadColorToQColor: public sad::db::AbstractTypeConverter
{
public:
	/*! Creates new converter
	 */
	inline SadColorToQColor()
	{
		
	}
	/*! Converts source value from another and to another type
		\param[in] source a pointer to sad::Color value
		\param[in] dest a pointer to QColor value
	 */
	virtual void convert(void * source, void * dest);
	/*! Can be inherited
	 */
	virtual ~SadColorToQColor();
};

}
	
}
