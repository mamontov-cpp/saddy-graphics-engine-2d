#include "spritedatabase.h"
#include "fonttemplatesdatabase.h"
#include <QSet>
#include <QMessageBox>

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

QString SpriteDatabaseIterator::config() const
{
	return m_configs_iterator.key();
}

QString SpriteDatabaseIterator::group() const
{
	return m_config_iterator.key();
}

int SpriteDatabaseIterator::groupIndex() const
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
		++m_group_iterator;
		if (m_group_iterator == m_config_iterator.value().constEnd())
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
		if (!found)
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
		if (m_config_iterator.value().size()!=0)
			found = advanceNextGroup(false);
		if (!found)
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
	clear();
}


void SpriteDatabase::clear()
{
	m_qtimages.clear();
	for(hst::Configs::const_iterator it = m_configs.const_begin(); it!=m_configs.const_end();it++)
	{
		delete it.value();
	}
	m_configs.clear();
}



bool SpriteDatabase::load(FontTemplatesMaps & maps, int & counter)
{
	bool ok = true;
	const db::NameFileMap & map = maps.configs();
	QISpriteConfigs	 * qtimages = new QISpriteConfigs();
	hst::Configs	 * configs = new hst::Configs();
	for (db::NameFileMap::const_iterator it = map.begin();it != map.end();it++)
	{
		Sprite2DTemplateContainer testcontainer;
		XMLConfigLoader * loader = new XMLConfigLoader(it.value());
		if (loader->load(testcontainer))
		{
			QVector<QString> texturePaths = extractTexturePaths(testcontainer);
			QHash<QString, QImage> images;
			if (tryLoadImages(texturePaths,images))
			{
				this->importSprites(*qtimages,images,testcontainer,it.key());
				Sprite2DConfig * cnf = new Sprite2DConfig( hst::string::number(counter++) + it.key().toStdString().c_str());
				cnf->setLoader(loader);
				if (cnf->reload() != SCR_OK)
				{
					hst::log::inst()->owrite("Saddy can\'t load images from config with path ").owrite(it.value().toStdString()).
							  owrite("\n");
					QMessageBox::critical(NULL,"IFace Editor", QString("Saddy can\'t load images from config with path ")+
				                  it.value());

					qtimages->remove(it.key());
					delete cnf;
					ok = false;
				}
				else 
				{
					configs->insert(it.key().toStdString().c_str(),cnf);
				}
			}
			else
			{
				ok = false;
				hst::log::inst()->owrite("Can\'t load images from config with path ").owrite(it.value().toStdString()).
							  owrite("\n");
				QMessageBox::critical(NULL,"IFace Editor", QString("Can\'t load images from config with path ")+
				                  it.value());
			}
		}
		else 
		{
			ok = false;
			hst::log::inst()->owrite("Can\'t load config from path ").owrite(it.value().toStdString()).
							  owrite("\n");
			QMessageBox::critical(NULL,"IFace Editor", QString("Can\'t load config from path ")+
				                  it.value());
		}
	}

	if (ok)
	{
		m_qtimages = *qtimages;
		m_configs = *configs;
	}
	else
	{
		for (hst::Configs::const_iterator it = configs->const_begin(); it!=configs->const_end(); it++)
		{
			delete it.value();
		}
	}
	delete qtimages;
	delete configs;
	return ok;
}

void SpriteDatabase::importSprites(QISpriteConfigs & configs, 
								   QHash<QString, QImage> & images, 
								   Sprite2DTemplateContainer & t,
								   const QString & name)
{
	configs.insert(name, QISpriteConfig());
	Sprite2DTemplateContainer::const_iterator it = t.const_begin();
	for(it; it!=t.const_end();it++)
	{
		QString group = it.key().data();
		configs[name].insert(group, QISpriteGroup());
		for(hst::hash<int,Sprite2DTemplate>::const_iterator g = it.value().const_begin(); 
			g!=it.value().const_end();
			g++)
		{
			configs[name][group].insert(g.key(), QImage());
			Sprite2DTemplate s2dt = g.value();
			configs[name][group][g.key()] = extractImage(images[s2dt.textureName().data()],s2dt);
		}
	}
}

AbstractSpriteDatabaseIterator * SpriteDatabase::begin() const
{
	SpriteDatabaseIterator * it = new SpriteDatabaseIterator(const_cast<SpriteDatabase*>(this));
	it->m_configs_iterator = this->m_qtimages.begin();
	it->advanceNextConfig(false);
	return it;
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