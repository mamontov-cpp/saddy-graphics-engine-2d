#include "sadvectorsadvectoracolortoqvectorqvectorqcolor.h"

DECLARE_COMMON_TYPE(QVector<QVector<QColor> >)


void 
core
::typeconverters
::SadVectorSadVectorToAColorToQVectorQVectorQColor
::convert(
	const sad::Vector<sad::Vector<sad::AColor> > & src,
	QVector<QVector<QColor> > & dest
)
{
	dest = QVector<QVector<QColor> >();
	for(size_t i = 0; i < src.size(); i++)
	{
		dest << QVector<QColor>();
		for(size_t j = 0; j < src[i].size(); j++)
		{
			dest[i] << QColor(
				src[i][j].r(),
				src[i][j].g(),
				src[i][j].b(),
				src[i][j].a()
			);
		}
	}
}

void 
core
::typeconverters
::SadVectorSadVectorToAColorToQVectorQVectorQColor
::convert(void * source, void * dest)
{
	sad::Vector<sad::Vector<sad::AColor> > * src = reinterpret_cast<
		sad::Vector<sad::Vector<sad::AColor> >*
	>(source);
	QVector<QVector<QColor> > * dst = reinterpret_cast<QVector<QVector<QColor> >*>(dest);

	core
	::typeconverters
	::SadVectorSadVectorToAColorToQVectorQVectorQColor
	::convert(*src, *dst);
}

core
::typeconverters
::SadVectorSadVectorToAColorToQVectorQVectorQColor
::~SadVectorSadVectorToAColorToQVectorQVectorQColor()
{
	
}
