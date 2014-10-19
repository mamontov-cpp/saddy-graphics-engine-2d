#include "qlistqlistqcolortosadvectorsadvectoracolor.h"

void 
core
::typeconverters
::QListQListQColorToSadVectorSadVectorToAColor
::convert(
	const QList<QList<QColor> > & src,
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
				255- src[i][j].alpha()
			);
		}
	}
}


void 
core
::typeconverters
::QListQListQColorToSadVectorSadVectorToAColor
::convert(void * source, void * dest)
{
	QList<QList<QColor> > * src = reinterpret_cast<QList<QList<QColor> >*>(source);
	sad::Vector<sad::Vector<sad::AColor> > * dst = reinterpret_cast<
		sad::Vector<sad::Vector<sad::AColor> >*
	>(dest);
	core::typeconverters::QListQListQColorToSadVectorSadVectorToAColor::convert(*src, *dst);	
}

core
::typeconverters
::QListQListQColorToSadVectorSadVectorToAColor
::~QListQListQColorToSadVectorSadVectorToAColor()
{
	
}
