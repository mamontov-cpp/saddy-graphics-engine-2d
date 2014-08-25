#include "core/typeconverters/qcolortosadcolor.h"


void core::typeconverters::QColorToSadColor::convert(void * source, void * dest)
{
	QColor * src = reinterpret_cast<QColor*>(source);
	sad::Color * dst = reinterpret_cast<sad::Color*>(dest);

	*dst = sad::Color(src->red(), src->green(), src->blue());
}

core::typeconverters::QColorToSadColor::~QColorToSadColor()
{
	
}
