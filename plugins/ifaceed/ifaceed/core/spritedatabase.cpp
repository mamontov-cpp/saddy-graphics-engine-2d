#include "spritedatabase.h"
#include <QSet>

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

QVector<QString> extractTexturePaths(const Sprite2DTemplateContainer & c)
{
	QVector<QString> result;
	QSet<QString> set;

	Sprite2DTemplateContainer::const_iterator x = c.const_begin();
	Sprite2DTemplateContainer::const_iterator x_end = c.const_end();
	for (x; x!=x_end; x++)
	{
		hst::hash<int, Sprite2DTemplate>::const_iterator y = x.value().const_begin();
		hst::hash<int, Sprite2DTemplate>::const_iterator y_end = x.value().const_end();
		for(y; y!=y_end; y++)
		{
			hst::string path = y.value().textureName();
			if (set.contains(path.data()) == false) 
			{
				set << path.data();
				result << path.data();
			}
		}
	}	
	return result;
}

bool tryLoadImages(const QVector<QString> & loads, QHash<QString, QImage> & images)
{
	bool success = true;
	images.clear();
	for (int i=0;i<loads.count();i++)
	{
		QImage im;
		bool loaded = im.load(loads[i]);
		if (loaded)
		{
			images.insert(loads[i],im);
		}
		else
		{
			success = false;
		}
	}
	return success;
}