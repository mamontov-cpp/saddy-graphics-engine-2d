#include "core/typeconverters/sadacolortoqcolor.h"


void core::typeconverters::SadAColorToQColor::convert(void * source, void * dest)
{
	sad::AColor * src = reinterpret_cast<sad::AColor*>(source);
	QColor * dst = reinterpret_cast<QColor*>(dest);

	*dst = QColor(src->r(), src->g(), src->b(), 255 - src->a());
}

core::typeconverters::SadAColorToQColor::~SadAColorToQColor()
{
	
}
