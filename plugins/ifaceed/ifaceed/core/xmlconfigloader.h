/*!  \file xmlconfigloader.h
	 \author HiddenSeeker
	 A loader for config, that loads it from XML file
 */
#include <config/sprite2dconfigloader.h>
#include <QString>
#include <QDomElement>
#include <QVector>
#pragma once

/*! \class XMLConfigLoader 
	A loader for config in XML format
 */
class XMLConfigLoader: public Sprite2DConfigLoader
{
 private:
		/*! \class XMLConfigEntry
			An entry, that represents inner structure of elements in config
		 */
	    class XMLConfigEntry
		{
		 public:
				 hst::string spriteGroup;    //!< Name of sprite group
				 int         index;          //!< Index of sprite in group
				 Sprite2DTemplate spriteTemplate;  //!< Template description of entry
		};

		QString m_filename; //!< File name, where we should load
		/*! Returns elements from document
			\param[in] doc parent element
			\return elements vector
		 */ 
		QVector<QDomElement> getElements(const QDomNode & doc);
		/*! Tries to load entry from element
			\param[out] entry       entry of sprite
			\param[in]  element     xml element
			\return true, if successfull
		 */
		bool loadEntry(XMLConfigEntry & entry, const QDomElement & element);
 public:
	    /*! Inits a config loader
			\param[in] name name of loader
		 */
		XMLConfigLoader(const QString & name)
		{
			m_filename=name;
		}
		/*! A function for loading a config. For loading a config, you must fill a container
			with appropriate data and return true. If some errors occured, function must return false
			\param[out] container container to be filled
			\return     whether loading was correct
		 */
	    bool load(Sprite2DTemplateContainer & container);
		/*! Does nothing
		 */
		~XMLConfigLoader();
};
