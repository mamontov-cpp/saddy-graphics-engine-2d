#include "spritedatabase.h"
#include "fonttemplatesdatabase.h"
#include <QSet>

AbstractSpriteDatabaseIterator::~AbstractSpriteDatabaseIterator()
{
}


AbstractSpriteDatabase::~AbstractSpriteDatabase()
{
}



SpriteDatabaseIterator::SpriteDatabaseIterator(SpriteDatabase * db)
{
	m_db = db;
}

SpriteDatabaseIterator::~SpriteDatabaseIterator()
{

}

QString SpriteDatabaseIterator::config()
{
	return m_configs_iterator.key();
}

QString SpriteDatabaseIterator::group()
{
	return m_config_iterator.key();
}

int SpriteDatabaseIterator::groupIndex()
{
	return m_group_iterator.key();
}

QImage SpriteDatabaseIterator::image()
{
	return m_group_iterator.value();
}

Sprite2DConfig * SpriteDatabaseIterator::spriteConfig()
{
	return m_db->m_configs[this->config().toStdString().c_str()];
}

AbstractSpriteDatabaseIterator & SpriteDatabaseIterator::next()
{
	if (m_configs_iterator!= m_db->m_qtimages.constEnd())
	{
		if (m_group_iterator != m_config_iterator.value().constEnd())
		{
			++m_group_iterator;
		}
		else
		{
			bool foundNextGroup = advanceNextGroup(true);
			if (!foundNextGroup)
			{
				advanceNextConfig(true);	
			}
		}
	}
	return *this;
}

bool SpriteDatabaseIterator::advanceNextGroup(bool move)
{
	if (m_config_iterator == m_configs_iterator.value().constEnd())
		return false;
	if (move)
		++m_config_iterator;
	bool found = false;
	while(m_config_iterator!=m_configs_iterator.value().constEnd() && !found)
	{
		m_group_iterator = m_config_iterator.value().constBegin();
		if (m_group_iterator != m_config_iterator.value().constEnd())
			found = true;
		++m_config_iterator;
	}

	return found;
}

bool SpriteDatabaseIterator::advanceNextConfig(bool move)
{
	if (m_configs_iterator == m_db->m_qtimages.constEnd())
		return false;
	if (move)
		++m_configs_iterator;
	bool found = false;
	while(m_configs_iterator != m_db->m_qtimages.constEnd() && !found)
	{
		m_config_iterator = m_configs_iterator.value().constBegin();
		found = advanceNextGroup(false);
		++m_configs_iterator;
	}
	return found;
}

bool SpriteDatabaseIterator::isEnd() const 
{
	return m_configs_iterator==m_db->m_qtimages.constEnd();
}


SpriteDatabase::~SpriteDatabase()
{
	for(hst::Configs::const_iterator it = m_configs.const_begin(); it!=m_configs.const_end();it++)
	{
		delete it.value();
	}
}


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