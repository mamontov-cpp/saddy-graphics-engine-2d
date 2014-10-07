#include "qrectftosadrect2d.h"

void core::typeconverters::QRectFToSadRect2D::convert(
        const QRectF& source,
        sad::Rect2D& dest
)
{
    dest =  sad::Rect2D(source.left(), source.top(), source.right(), source.bottom());
}

void core::typeconverters::QRectFToSadRect2D::convert(void * source, void * dest)
{
    QRectF * src = reinterpret_cast<QRectF*>(source);
    sad::Rect2D * dst = reinterpret_cast<sad::Rect2D*>(dest);

    core::typeconverters::QRectFToSadRect2D::convert(*src, *dst);
}

core::typeconverters::QRectFToSadRect2D::~QRectFToSadRect2D()
{

}

