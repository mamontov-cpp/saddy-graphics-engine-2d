/*! \file sadvectorsadvectoracolortoqvectorqvectorqcolor.h
	\author HiddenSeeker

	Defines conversion from sad::Vector<sad::Vector<sad::AColor> > to QVector<QVector<QColor> >.
 */
#pragma once

#include <QVector>
#include <QColor>

#include <sadvector.h>
#include <sadcolor.h>

#include <db/dbconversiontable.h>

namespace core
{

namespace typeconverters
{

/*! Defines a conversion from sad::Vector<sad::Vector<sad::AColor> > to QVector<QVector<QColor> >.
 */
class SadVectorSadVectorToAColorToQVectorQVectorQColor: public sad::db::AbstractTypeConverter
{
public:
	/*! Creates new converter
	 */
	inline SadVectorSadVectorToAColorToQVectorQVectorQColor()
	{
		
	}
	/*! Converts source value to dest value
		\param[in] src source value
		\param[in] dest destination value
	 */
	static void convert(
		const sad::Vector<sad::Vector<sad::AColor> > & src,
		QVector<QVector<QColor> > & dest
	);
	/*! Converts source value from another and to another type
		\param[in] source a pointer to sad::Vector<sad::Vector<sad::AColor> > value
		\param[in] dest a pointer to QVector<QVector<QColor> > value
	 */
	virtual void convert(void * source, void * dest);
	/*! Can be inherited
	 */
	virtual ~SadVectorSadVectorToAColorToQVectorQVectorQColor();
};

}
	
}
