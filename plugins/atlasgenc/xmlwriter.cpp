#include "xmlwriter.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

XMLWriter::XMLWriter()
{

}

XMLWriter::~XMLWriter()
{

}

bool XMLWriter::write(
		const Atlas& atlas,
		const QString& filename,
		const QString& outputTexture,
		bool withindex
)
{
	QDomDocument doc;
	QDomElement root = doc.createElement("sprites");
	doc.appendChild(root);
	bool result = false;
	for(size_t i = 0; i < atlas.Entries.size(); i++)
	{
		const AtlasEntry& entry = atlas.Entries[i];
		if (entry.canBeWritten() == false)
		{
			return false;
		}
		QDomElement tag = doc.createElement(entry.Name.value());
		if (entry.Index.exists() && withindex)
		{	
			tag.setAttribute("index", QString::number(entry.Index.value()));		
		}
		tag.setAttribute("texture", outputTexture);

		QString size = QString::number(entry.Size.value().width()) + QString(";") + QString::number(entry.Size.value().height());
		tag.setAttribute("size", size);

		QString texrect = QString::number(entry.TextureRectangle.value().topLeft().x()) 
						+ QString(";") 
						+ QString::number(entry.TextureRectangle.value().topLeft().y())
						+ QString(";") 
						+ QString::number(entry.TextureRectangle.value().width())
						+ QString(";") 
						+ QString::number(entry.TextureRectangle.value().height());
		tag.setAttribute("texrect", texrect);

		if (entry.Transparent.exists())
		{	
			QString transparent = QString::number(entry.Transparent.value().red()) 
						        + QString(";") 
						        + QString::number(entry.Transparent.value().green())
						        + QString(";") 
						        + QString::number(entry.Transparent.value().blue());
			tag.setAttribute("transparent", transparent);		
		}

		root.appendChild(tag);
	}

	QString stringdata = doc.toString();
	QFile file(filename); 
	if (!file.open(QIODevice::WriteOnly)) {
		this->Errors << QString("Can\'t open file \"") + filename + QString("\"");
		return false;
	}

	result = true;
	QTextStream stream(&file);
	stream << stringdata;
	file.close();
	return result;
}
