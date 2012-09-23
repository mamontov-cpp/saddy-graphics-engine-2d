#include "spritedatabase.h"

QImage extractImage(const QImage & im, Sprite2DTemplate & t)
{
	QImage result = im.convertToFormat(QImage::Format_RGB32);
	result = result.copy( QRect(QPoint(t.textureRect().p().x(),t.textureRect().p().y()),
							    QSize(t.textureRect().width(),t.textureRect().height())));
	if (t.isTransparent()) 
	{
		for (int x=0;x<result.width();x++) 
		{
			for (int y=0;y<result.height();y++)
			{
				QRgb color = im.pixel(x,y);
				if (qRed(color) == t.transparencyColor().r()
					&& qGreen(color) == t.transparencyColor().g()
					&& qBlue(color) == t.transparencyColor().b())
				{
					result.setPixel(x,y, Qt::transparent);
				}
			}
		}
	}
	return result;
}

