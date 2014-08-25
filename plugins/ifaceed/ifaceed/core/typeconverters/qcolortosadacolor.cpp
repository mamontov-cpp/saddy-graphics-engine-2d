#include "core/typeconverters/qcolortosadacolor.h"


void core::typeconverters::QColorToSadAColor::convert(void * source, void * dest)
{
	QColor * src = reinterpret_cast<QColor*>(source);
	sad::AColor * dst = reinterpret_cast<sad::AColor*>(dest);

	*dst = sad::AColor(src->red(), src->green(), src->blue(), src->alpha());
}

core::typeconverters::QColorToSadAColor::~QColorToSadAColor()
{
	
}
