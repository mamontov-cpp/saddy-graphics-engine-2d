#include "sadvectorsadvectoracolortoqlistqlistqcolor.h"

DECLARE_COMMON_TYPE(QList<QList<QColor> >)


void 
core
::typeconverters
::SadVectorSadVectorToAColorToQListQListQColor
::convert(
	const sad::Vector<sad::Vector<sad::AColor> > & src,
	QList<QList<QColor> > & dest
)
{
	dest = QList<QList<QColor> >();
	for(size_t i = 0; i < src.size(); i++)
	{
		dest << QList<QColor>();
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
::SadVectorSadVectorToAColorToQListQListQColor
::convert(void * source, void * dest)
{
	sad::Vector<sad::Vector<sad::AColor> > * src = reinterpret_cast<
		sad::Vector<sad::Vector<sad::AColor> >*
	>(source);
	QList<QList<QColor> > * dst = reinterpret_cast<QList<QList<QColor> >*>(dest);

	core
	::typeconverters
	::SadVectorSadVectorToAColorToQListQListQColor
	::convert(*src, *dst);
}

core
::typeconverters
::SadVectorSadVectorToAColorToQListQListQColor
::~SadVectorSadVectorToAColorToQListQListQColor()
{
	
}
