#include "sadrect2dtoqrectf.h"

void core::typeconverters::SadRect2DToQRectF::convert(
        const sad::Rect2D& source,
        QRectF& dest
)
{
    dest = QRectF(source[0].x(), source[0].y(), source.width(), source.height());
}

void core::typeconverters::SadRect2DToQRectF::convert(void * source, void * dest)
{
    sad::Rect2D * src = reinterpret_cast<sad::Rect2D*>(source);
    QRectF * dst = reinterpret_cast<QRectF*>(dest);

    core::typeconverters::SadRect2DToQRectF::convert(*src, *dst);
}

core::typeconverters::SadRect2DToQRectF::~SadRect2DToQRectF()
{

}
