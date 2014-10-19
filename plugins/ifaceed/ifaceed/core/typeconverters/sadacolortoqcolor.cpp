#include "core/typeconverters/sadacolortoqcolor.h"

void core::typeconverters::SadAColorToQColor::convert(
    const sad::AColor& source,
    QColor& dest
)
{
    dest = QColor(source.r(), source.g(), source.b(), 255 - source.a());
}

void core::typeconverters::SadAColorToQColor::convert(void * source, void * dest)
{
	sad::AColor * src = reinterpret_cast<sad::AColor*>(source);
	QColor * dst = reinterpret_cast<QColor*>(dest);

    core::typeconverters::SadAColorToQColor::convert(*src, *dst);
}

core::typeconverters::SadAColorToQColor::~SadAColorToQColor()
{
	
}
