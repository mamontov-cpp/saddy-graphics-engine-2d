#include "core/typeconverters/sadcolortoqcolor.h"
#include "object.h"

DECLARE_COMMON_TYPE(QColor)

void core::typeconverters::SadColorToQColor::convert(void * source, void * dest)
{
	sad::Color * src = reinterpret_cast<sad::Color*>(source);
	QColor * dst = reinterpret_cast<QColor*>(dest);

	*dst = QColor(src->r(), src->g(), src->b());
}

core::typeconverters::SadColorToQColor::~SadColorToQColor()
{
	
}
