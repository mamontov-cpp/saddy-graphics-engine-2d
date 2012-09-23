#include "fonttemplatesdatabase.h"
#include <QDomDocument>
#include <QFile>
#include <QMessageBox>

FontTemplatesMaps::FontTemplatesMaps()
{

}

bool FontTemplatesMaps::load(const QString & name)
{
	m_fonts.clear();
	m_configs.clear();
	
	QDomDocument doc("config");
	QFile file(name);
	if (!file.open(QIODevice::ReadOnly)) 
	{
		hst::log::inst()->owrite(hst::string("FontTemplatesMaps::load: can\'t load file"))
						 .owrite(hst::string(name.toStdString().c_str()))
						 .owrite(hst::string("\n"));			
		return false;
	}
	if (!doc.setContent(&file)) 
	{
		hst::log::inst()->owrite(hst::string("FontTemplatesMaps::load: can\'t parse file"))
						 .owrite(hst::string(name.toStdString().c_str()))
						 .owrite(hst::string("\n"));
		file.close();
		return false;
	}
	file.close();
	QDomElement root = doc.documentElement();
	if (root.isNull())
	{
		hst::log::inst()->owrite(hst::string("FontTemplatesMaps::load: no root element found"))
						 .owrite(hst::string(name.toStdString().c_str()))
						 .owrite(hst::string("\n"));
		return false;
	}

	
	return true;
}

