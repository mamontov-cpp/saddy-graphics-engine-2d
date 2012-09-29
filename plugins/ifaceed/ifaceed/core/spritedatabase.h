/* \file spritedatabase.h
   \author HiddenSeeker

   A database for holding all of the sprites
*/
#include "xmlconfigloader.h"
#include <QImage>
#include <QHash>
#include <config/sprite2dconfig.h>
#pragma once

/** Abstract sprite database iterator, used to iterate through database
 */
class AbstractSpriteDatabaseIterator
{
 public:
		/** Returns a config
			\return config 
		 */
		virtual QString config()=0;
		/** Returns a group of data
			\return group of config
		 */
		virtual QString group()=0;
		/** Returns a group index for sprite
			\return group index
		 */
		virtual int groupIndex()=0;
		/** Returns an image
			\return image
		 */
		virtual QImage image()=0;
		/** Sprite config data
			\return resulting config
		 */
		virtual Sprite2DConfig * spriteConfig()=0;
		/** Goes to next item
			\return this
		 */
		virtual AbstractSpriteDatabaseIterator & next()=0;
		/** Whether end 
			\return whether end
		 */
		virtual bool isEnd() const =0;
		virtual ~AbstractSpriteDatabaseIterator();
};

/** Database of sprites and data
 */
class AbstractSpriteDatabase
{

 public:
		/** Begin of database
		 */
		virtual AbstractSpriteDatabaseIterator * begin() const =0;
		/** End of database
		 */
		virtual AbstractSpriteDatabaseIterator * end() const =0;

		virtual ~AbstractSpriteDatabase();
};

class FontTemplatesMaps;

/** A duplicate of group for one group
 */
typedef QHash<int, QImage> QISpriteGroup;
/** A duplicate of group for config
 */
typedef QHash<QString, QISpriteGroup> QISpriteConfig;
/** A total config data
 */ 
typedef QHash<QString, QISpriteConfig> QISpriteConfigs;

namespace hst
{
	typedef hst::hash<hst::string, Sprite2DConfig *> Configs;
}

class SpriteDatabaseIterator: public AbstractSpriteDatabaseIterator
{
 friend class SpriteDatabase;
 protected:
    /** Database for data
	 */
    SpriteDatabase * m_db;
	/** Iterator for configs
	 */
	QISpriteConfigs::const_iterator m_configs_iterator;
	/** Iterator for groups and images
	 */
	QISpriteConfig::const_iterator  m_config_iterator;
	/** Iterator for indexes and images
	 */
	QISpriteGroup::const_iterator m_group_iterator;
	/** Constructs iterator
		\param[in] db database
	 */ 
	SpriteDatabaseIterator(SpriteDatabase * db);
	/** Seeks for new non-empty group, setting to beginning
		\param[in] move whether we should move a config to advance
		\return whether we successfully found some item
	 */
	bool advanceNextGroup(bool move);
	/** Seeks for new non-empty config, setting to beginning
		\param[in] move whether we should move a config to advance
		\return whether we successfully found some item
	 */
	bool advanceNextConfig(bool move);
 public:
	/** Returns a config
		\return config 
	  */
	virtual QString config();
	/** Returns a group of data
		\return group of config
	  */
	virtual QString group();
	/** Returns a group index for sprite
		\return group index
	  */
	virtual int groupIndex();
	/** Returns an image
		\return image
	  */
	virtual QImage image();
	/** Sprite config data
	    \return resulting config
	  */
	virtual Sprite2DConfig * spriteConfig();
	/** Goes to next item
		\return this
	  */
	virtual AbstractSpriteDatabaseIterator & next();
	/** Whether end 
		\return whether end
	 */
	virtual bool isEnd() const;	
	virtual ~SpriteDatabaseIterator();
};

/** A sprite database
 */
class SpriteDatabase: public AbstractSpriteDatabase
{
 friend class SpriteDatabaseIterator;
 protected:
	 QISpriteConfigs						  m_qtimages;
	 hst::Configs m_configs;
 public:


	 virtual ~SpriteDatabase();
};

/** Extracts texture paths
	\param[in] c container
 */
QVector<QString> extractTexturePaths(const Sprite2DTemplateContainer & c);

/** Tries to load all of images to data
	\param[in] loads a texture paths
	\param[out] images images hash
	\return whether loading was successfull
 */
bool tryLoadImages(const QVector<QString> & loads, QHash<QString, QImage> & images);
/** Extract image from big texture
	\param[in] im image data
	\param[in] t  template 
	\return image data
 */
QImage extractImage(const QImage & im, Sprite2DTemplate & t);
