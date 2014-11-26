#include "jsonwriter.h"

JSONWriter::JSONWriter()
{

}

JSONWriter::~JSONWriter()
{

}


bool JSONWriter::write(
		const Atlas& atlas,
		const QString& filename,
		const QString& outputTexture,
		bool withindex
)
{
	picojson::value root(picojson::object_type, false);
	bool result = false;
	root.insert("resource", atlas.TextureResourceName.value().toStdString());
	root.insert("file", outputTexture.toStdString());

	picojson::value atlasnode(picojson::array_type, false);
	
	for(size_t i = 0; i < atlas.Entries.size(); i++)
	{
		const AtlasEntry& entry = atlas.Entries[i];
		if (entry.canBeWritten() == false)
		{
			return false;
		}

		picojson::value entrynode(picojson::object_type, false);

		entrynode.insert("name", entry.Name.value().toStdString());		
		if (entry.Index.exists() && withindex)
		{	
			entrynode.insert("index", QString::number(entry.Index.value()).toStdString());		
		}

		QString size = QString::number(entry.Size.value().width()) + QString(";") + QString::number(entry.Size.value().height());
		entrynode.insert("size", size.toStdString());

		QString texrect = QString::number(entry.TextureRectangle.value().topLeft().x()) 
						+ QString(";") 
						+ QString::number(entry.TextureRectangle.value().topLeft().y())
						+ QString(";") 
						+ QString::number(entry.TextureRectangle.value().width())
						+ QString(";") 
						+ QString::number(entry.TextureRectangle.value().height());
		entrynode.insert("texrect", texrect.toStdString());

		if (entry.Transparent.exists())
		{	
			QString transparent = QString::number(entry.Transparent.value().red()) 
						        + QString(";") 
						        + QString::number(entry.Transparent.value().green())
						        + QString(";") 
						        + QString::number(entry.Transparent.value().blue());
			entrynode.insert("transparent", transparent.toStdString());		
		}

		atlasnode.push_back(entrynode);
	}

	root.insert("atlas", atlasnode);

	QString stringdata = root.serialize().c_str();
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