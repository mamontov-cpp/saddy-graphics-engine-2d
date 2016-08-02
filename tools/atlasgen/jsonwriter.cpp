#include "jsonwriter.h"
#include "outputoptions.h"

#define TAR7Z_SADDY

#include "../../include/3rdparty/tar7z/include/reader.h"

JSONWriter::JSONWriter()
{

}

JSONWriter::~JSONWriter()
{

}


bool JSONWriter::write(
        const Atlas& atlas,
        OutputOptions& opts
)
{
    bool withindex = (*(opts.ProgramOptions))["with-index"].value<bool>();
    picojson::value root(picojson::object_type, false);
    root.insert("resource", atlas.resourceName().toStdString());
    root.insert("file", atlas.outputTexture().toStdString());

    picojson::value atlasnode(picojson::array_type, false);
    
    for(size_t i = 0; i < atlas.entries().size(); i++)
    {
        const AtlasEntry& entry = atlas.entries()[i];
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

    if (opts.ProgramOptions->contains("write-to-tar") && (opts.Archive))
    {
        std::string s = stringdata.toStdString();
        std::vector<char> v;
        v.insert(v.end(), s.begin(), s.end());


        QString  output_name = OutputOptions::tar7zCompatibleName(atlas.outputName());
        opts.Archive->add(output_name.toStdString(), v);
    }
    else
    {
        QFile file(atlas.outputName()); 
        if (!file.open(QIODevice::WriteOnly)) 
        {
            opts.ReturnCode = 5;
            m_errors << QString("Can\'t open file \"") + atlas.outputName() + QString("\"");
            return false;
        }

        QTextStream stream(&file);
        stream << stringdata;
        file.close();
    }

    return true;
}
