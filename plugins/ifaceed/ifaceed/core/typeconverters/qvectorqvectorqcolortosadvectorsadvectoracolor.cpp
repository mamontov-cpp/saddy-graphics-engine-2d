#include "qvectorqvectorqcolortosadvectorsadvectoracolor.h"

void 
core
::typeconverters
::QVectorQVectorQColorToSadVectorSadVectorToAColor
::convert(
	const QVector<QVector<QColor> > & src,
	sad::Vector<sad::Vector<sad::AColor> > & dest
)
{
	dest = sad::Vector<sad::Vector<sad::AColor> >();
	for(size_t i = 0; i < src.size(); i++)
	{
		dest << sad::Vector<sad::AColor>();
		for(size_t j = 0; j < src[i].size(); j++)
		{
			dest[i] << sad::AColor(
				src[i][j].red(),
				src[i][j].green(),
				src[i][j].blue(),
				src[i][j].alpha()
			);
		}
	}
}


void 
core
::typeconverters
::QVectorQVectorQColorToSadVectorSadVectorToAColor
::convert(void * source, void * dest)
{
	QVector<QVector<QColor> > * src = reinterpret_cast<QVector<QVector<QColor> >*>(source);
	sad::Vector<sad::Vector<sad::AColor> > * dst = reinterpret_cast<
		sad::Vector<sad::Vector<sad::AColor> >*
	>(dest);
	core::typeconverters::QVectorQVectorQColorToSadVectorSadVectorToAColor::convert(*src, *dst);	
}

core
::typeconverters
::QVectorQVectorQColorToSadVectorSadVectorToAColor
::~QVectorQVectorQColorToSadVectorSadVectorToAColor()
{
	
}
