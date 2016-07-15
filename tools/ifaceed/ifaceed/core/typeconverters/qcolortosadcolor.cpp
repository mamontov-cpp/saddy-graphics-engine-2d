#include "core/typeconverters/qcolortosadcolor.h"

void core::typeconverters::QColorToSadColor::convert(
    const QColor & source,
    sad::Color & dest
)
{
    dest = sad::Color(source.red(), source.green(), source.blue());
}

void core::typeconverters::QColorToSadColor::convert(void * source, void * dest)
{
    QColor * src = reinterpret_cast<QColor*>(source);
    sad::Color * dst = reinterpret_cast<sad::Color*>(dest);

    core::typeconverters::QColorToSadColor::convert(*src, *dst);
}

core::typeconverters::QColorToSadColor::~QColorToSadColor()
{
    
}
