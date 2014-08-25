/*! \file qlistqlistqcolortosadvectorsadvectoracolor.h
	\author HiddenSeeker

	Defines conversion from QList<QList<QColor> > to sad::Vector<sad::Vector<sad::AColor> >.
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

/*! Defines a conversion from QList<QList<QColor> > to sad::Vector<sad::Vector<sad::AColor> >.
 */
class QListQListQColorToSadVectorSadVectorToAColor: public sad::db::AbstractTypeConverter
{
public:
	/*! Creates new converter
	 */
	inline QListQListQColorToSadVectorSadVectorToAColor()
	{
		
	}
	/*! Converts source value to dest value
		\param[in] src source value
		\param[in] dest destination value
	 */
	static void convert(
		const QList<QList<QColor> > & src,
		sad::Vector<sad::Vector<sad::AColor> > & dest
	);
	/*! Converts source value from another and to another type
		\param[in] source a pointer to QList<QList<QColor> > value
		\param[in] dest a pointer to sad::Vector<sad::Vector<sad::AColor> > value
	 */
	virtual void convert(void * source, void * dest);
	/*! Can be inherited
	 */
	virtual ~QListQListQColorToSadVectorSadVectorToAColor();
};

}
	
}
