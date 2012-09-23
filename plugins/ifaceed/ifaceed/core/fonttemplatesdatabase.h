/** \file   fonttemplatesdatabase.h
	\author HiddenSeeker

	A database for holding all of fonts and templates, loaded at time.
	You can with
 */
#include <QHash>
#include <QString>
#include <QDomDocument>
#include <log.h>
#pragma once

namespace db
{
typedef QHash<QString, QString> NameFileMap;
}

/** A maps, to be read from XML file data
 */
class FontTemplatesMaps
{
 private:
		db::NameFileMap m_fonts;
		db::NameFileMap m_configs;
		/** Loads a found entry
			\param[in] entry element
			\param[in] parent parent directory
		 */
		void loadFont(QDomElement & entry, const hst::string & parent);
		/** Loads a found config
			\param[in] entry element
			\param[in] parent parent directory
		 */
		void loadConfig(QDomElement & entry, const hst::string & parent);

 public:
		/** Constructs new empty maps
		 */
		FontTemplatesMaps();
		/** Loads a configs from file, specified by path and name
			Also preprocesses a local paths from config data
		 */
		bool load(const QString & name);
		/** Read fonts
			\return entities of fonts
		 */
	    inline const db::NameFileMap & fonts() { return m_fonts; }
		/** Read config
			\return entities  of configs
		 */
		inline const db::NameFileMap & configs() { return m_configs;}
};