#include "core/typeconverters/qcolortosadacolor.h"


void core::typeconverters::QColorToSadAColor::convert(
        const QColor & source,
        sad::AColor & dest
)
{
    dest = sad::AColor(source.red(), source.green(), source.blue(), 255 - source.alpha());
}

void core::typeconverters::QColorToSadAColor::convert(void * source, void * dest)
{
    QColor * src = reinterpret_cast<QColor*>(source);
    sad::AColor * dst = reinterpret_cast<sad::AColor*>(dest);

    core::typeconverters::QColorToSadAColor::convert(*src, *dst);
}

core::typeconverters::QColorToSadAColor::~QColorToSadAColor()
{
    
}
