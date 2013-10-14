#include "fonttemplatesdatabase.h"
// Avoid nasty bug with Qt's data stream
#ifdef Status
    #undef Status
#endif
#ifdef Bool
    #undef Bool
#endif
#ifdef CursorShape
    #undef CursorShape
#endif
#include <QDomDocument>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include "../editorcore/path.h"
#include "xmlconfigloader.h"
#include "spritedatabase.h"
#include <log/log.h>
#include <unused.h>

FontTemplatesMaps::FontTemplatesMaps()
{

}

bool FontTemplatesMaps::load(const QString & name, sad::log::Log * log)
{
	SL_SCOPE( QString("FontTemplatesMaps::load(\"%1\")").arg(name).toStdString() );

	sad::String parentdir =  path::directory(name.toStdString().c_str());
	log->message(QString("Parent dir is \"%1\"").arg(parentdir.data()));

	m_fonts.clear();
	m_configs.clear();
	
	QDomDocument doc("globalconfig");
	QFile file(name);
	if (!file.open(QIODevice::ReadOnly)) 
	{
		SL_MESSAGE(QString("Can\'t load file %1").arg(name));			
		return false;
	}
	if (!doc.setContent(&file)) 
	{
		SL_MESSAGE(QString("Can\'t parse file %1").arg(name));	;
		file.close();
		return false;
	}
	file.close();
	QDomElement root = doc.documentElement();
	if (root.isNull())
	{
		SL_MESSAGE(QString("No root element found in file %1").arg(name));
		return false;
	}
	
	QDomElement entry = root.firstChildElement();
    //bool test = entry.isNull();
	while(entry.isNull() == false) 
	{
		if (entry.tagName() == "font")
		{
			this->loadFont(entry, parentdir, log);
		} 
		else if (entry.tagName() == "config")
		{
			this->loadConfig(entry, parentdir, log);
		}
		else 
		{
			SL_WARNING(QString("Found unknown tag %1, while loading file %2")
				         .arg(entry.tagName()).arg(name));
		}
		entry = entry.nextSiblingElement();
	}
	
	return true;
}

void FontTemplatesMaps::loadFont(QDomElement & entry, const sad::String & parent, UNUSED sad::log::Log * log)
{
	SL_SCOPE("FontTemplatesMaps::loadFont");
	if (entry.hasAttribute("name")==false || entry.hasAttribute("file")==false)
	{
		SL_MESSAGE("In one or more font elements skipped attributes \"name\" or \"file\"\n");
	} 
	else 
	{
		QString name = entry.attribute("name");
		QString file = entry.attribute("file");
		if (m_fonts.contains(name))
		{
			SL_WARNING(QString("Duplicate font entry skipped %1").arg(name));
		}
		else
		{
			sad::String path = path::concat(parent,file.toStdString().c_str());
			m_fonts.insert(name, path.data());
			SL_DEBUG(QString("Deserialized XML Font entry \"%1\" with path \"%2\"")
						 .arg(name).arg(path.data()));
		}
	}
}

void FontTemplatesMaps::loadConfig(QDomElement & entry, const sad::String & parent, UNUSED sad::log::Log * log)
{
	SL_SCOPE("FontTemplatesMaps::loadConfig");
	if (entry.hasAttribute("name")==false || entry.hasAttribute("file")==false)
	{
		SL_MESSAGE("In one or more font elements skipped attributes \"name\" or \"file\"\n");
	} 
	else 
	{
		QString name = entry.attribute("name");
		QString file = entry.attribute("file");
		if (m_configs.contains(name))
		{
			SL_WARNING(QString("Duplicate config entry skipped %1").arg(name));
		}
		else 
		{
			sad::String path = path::concat(parent,file.toStdString().c_str());
			m_configs.insert(name, path.data());
			SL_DEBUG(QString("Deserialized XML Config entry \"%1\" with path \"%2\"")
						 .arg(name).arg(path.data()));
		}
	}
}

FontTemplateDatabase::FontTemplateDatabase(int * counter)
{
	m_sprites =  new SpriteDatabase();
	m_fonts = new IFaceEditorFontList();
	m_counter = counter;
}

FontTemplateDatabase::~FontTemplateDatabase()
{
	delete m_sprites;
	delete m_fonts;
}

bool FontTemplateDatabase::load(FontTemplatesMaps & maps,UNUSED sad::log::Log * log)
{
	SL_SCOPE("FontTemplateDatabase::load()");
	m_maps = maps;
	//log->push
	IFaceEditorFontList * fonts = new IFaceEditorFontList();
	{
		db::NameFileMap::const_iterator it = maps.fonts().constBegin();
		db::NameFileMap::const_iterator end = maps.fonts().constEnd();
		bool success = true;
		
        for (;it!=end;it++)
		{
			IFaceEditorFontLoadResult r = fonts->tryLoadFont(it.key(), it.value());
			if (r!=IEFLR_OK) 
			{
				SL_CRITICAL(QString("Can't load font from %1").arg(it.value()));
				success = false;
			}
		}
		// Don't proceed further if can't load stuff
		if (!success) {
			delete fonts;
			return false;
		}
	}
	SpriteDatabase * sprites = new SpriteDatabase();
	// Load some configs
	{
		if (sprites->load(maps, *m_counter) == false)
		{
			delete sprites;
			delete fonts;
			return false;
		}
	}
	// Apply changes
	delete m_sprites; m_sprites = sprites;
	delete m_fonts; m_fonts = fonts;

	return true;
}

