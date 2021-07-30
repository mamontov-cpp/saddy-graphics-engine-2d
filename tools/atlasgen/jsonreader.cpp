#include "jsonreader.h"

#include <math.h>

// ReSharper disable once CppUnusedIncludeDirective
#include <QtCore/QTextStream>

JSONReader::JSONReader()
{

}

JSONReader::~JSONReader()
{

}

void JSONReader::read(const QString& name, Atlas* atlas)
{
    m_ok = false;
    QFile file(name); 
    if (!file.open(QIODevice::ReadOnly)) {
        m_errors << QString("Can\'t open file \"") + name + QString("\"");
        return;
    }
    QTextStream in(&file);
    QString contents = in.readAll();
    file.close();

    picojson::value root = picojson::parse_string(contents.toStdString());
    if (picojson::get_last_error().size() == 0)
    {
        if (root.is<picojson::object>())
        {
            picojson::object ro = root.get<picojson::object>();
            if (ro.find("file") != ro.end() 
                && ro.find("resource") != ro.end() 
                && ro.find("atlas") != ro.end())
            {
                this->readAsAtlas(name, ro, atlas);
            }
            else
            {            
                this->readAsAtlasConfig(ro, atlas);
            }
        }
        else
        {
            m_errors << QString("Root element of file \"") + name + QString("\" is not an object");
        }
    }
    else
    {
        m_errors << QString("Can\'t parse file \"") + name + QString("\"");
    }

    m_ok = m_errors.size() == 0;
}


void JSONReader::readAsAtlas(const QString& name, picojson::object& ro, Atlas* atlas)
{
    QStringList output_file_name_parts = this->removeExtension(name);
    output_file_name_parts << "json";
    output_file_name_parts[0] += "-atlas";
    QString output_file_name = output_file_name_parts.join(".");	
    atlas->setOutputName(output_file_name);
    QImage source;
    bool loadedok = false;
    if (ro["file"].is<std::string>())
    {
        QString file = ro["file"].get<std::string>().c_str();
        QStringList file_parts = this->removeExtension(file);
        file_parts << "png";
        file_parts[0] += "-atlas";
        if (source.load(file) == false) 
        {
            m_errors << "Cannot load " + file;
        }
        else
        {
            loadedok = true;
        }
        atlas->setOutputTexture(file_parts.join("."));
    }
    else
    {
        m_errors << "Input texture in file " + name + " is not a string";
    }
    
    if (ro["resource"].is<std::string>())
    {
        QString resource = ro["resource"].get<std::string>().c_str();
        atlas->setResourceName(resource);
    }
    else
    {
        m_errors << "Input resource in file " + name + " is not a string";
    }
    
    if (ro["atlas"].is<picojson::array>() && loadedok)
    {
        picojson::array a = ro["atlas"].get<picojson::array>();
        for(size_t i = 0; i < a.size(); i++)
        {
            if (a[i].is<picojson::object>())
            {
                readAtlasElement(a[i].get<picojson::object>(), atlas, source);
            }
            else
            {
                m_errors << QString("Found entry of atlas, which is not an object: \n") + a[i].serialize().c_str();
            }
        }
    }
    else
    {
        m_errors << "Atlas list is not an array in file " + name;
    }
}

void JSONReader::readAsAtlasConfig(picojson::object& ro, Atlas* atlas)
{
    // Parse texture
    if (ro.find("texture") != ro.end())
    {
        if (ro["texture"].is<std::string>())
        {
            atlas->setOutputTexture(ro["texture"].get<std::string>().c_str());
        }
        else
        {
            m_errors << "Output texture is not a string";
        }
    }
    else
    {
        m_errors << "Output texture is not defined";
    }


    // Parse config
    if (ro.find("config") != ro.end())
    {
        if (ro["config"].is<std::string>())
        {
            atlas->setOutputName(ro["config"].get<std::string>().c_str());
        }
        else
        {
            m_errors << "Output atlas config filename is not a string";
        }
    }
    else
    {
        m_errors << "Output atlas config filename is not defined";
    }

    // Parse texture resource name
    if (ro.find("resource") != ro.end())
    {
        if (ro["resource"].is<std::string>())
        {
            atlas->setResourceName(ro["resource"].get<std::string>().c_str());
        }
        else
        {
            m_errors << "Output resource is not a string";
        }
    }
    else
    {
        m_errors << "Output resource is not defined";
    }

    if (ro.find("sprites") != ro.end())
    {
        if (ro["sprites"].is<picojson::array>())
        {
            picojson::array a = ro["sprites"].get<picojson::array>();
            for(size_t i = 0; i < a.size(); i++)
            {
                if (a[i].is<picojson::object>())
                {
                    readElement(a[i].get<picojson::object>(), atlas);
                }
                else
                {
                    m_errors << QString("Found entry of sprites, which is not an object: \n") + a[i].serialize().c_str();
                }
            }
        }
        else
        {
            m_errors << "Sprites list is not an array";
        }
    }
    else
    {
         m_errors << "Sprites list is not defined";
    }
}

void JSONReader::readElement(picojson::object& e, Atlas* atlas)
{
    QVector<QString> errors;
    AtlasEntry entry;

    if (e.find("name") != e.end())
    {
        if (e["name"].is<std::string>())
        {
            entry.Name.setValue(e["name"].get<std::string>().c_str());
        }
        else
        {
            errors << QString("Sprite name is not a string in entry: \n") + picojson::value(e).serialize().c_str();
        }
    }
    else
    {
        errors << QString("Sprite name is not found in entry: \n") + picojson::value(e).serialize().c_str();
    }

    if (e.find("index") != e.end())
    {
        bool handled = false;

        if (e["index"].is<double>())
        {
             entry.Index.setValue(static_cast<int>(e["index"].get<double>()));
        }

        if (e["index"].is<std::string>())
        {
            bool ok = false;
            int i = QString(e["index"].get<std::string>().c_str()).toInt(&ok);
            if (ok)
            {
                entry.Index.setValue(i);
            }
        }

        if (!handled)
        {
            errors << QString("Sprite index is not a string in entry: \n") + picojson::value(e).serialize().c_str();
        }
    }

    if (e.find("texture") != e.end())
    {
        if (e["texture"].is<std::string>())
        {
            QString texturename = e["texture"].get<std::string>().c_str();
            entry.InputTextureName.setValue(texturename);
            if (atlas->hasTexture(texturename) == false)
            {
                Texture* t = new Texture(texturename);
                if (t->load())
                {
                   atlas->pushTexture(t);
                }
                else
                {
                    delete t;
                    errors << QString("Can\'t load texture \"") + texturename + QString("\"");
                }
            }
            else
            {
                if (this->shouldPreserveUniqueTextures() == false)
                {
                    Texture* t = new Texture(*(atlas->getTexture(texturename)));
                    t->Name += entry.Name.value();
                    entry.InputTextureName.setValue(t->Name);
                    atlas->pushTexture(t);
                }
            }
        }
        else
        {
            errors << QString("Sprite texture is not a string in entry: \n") + picojson::value(e).serialize().c_str();
        }
    }
    else
    {
        errors << QString("Sprite texture is not found in entry: \n") + picojson::value(e).serialize().c_str();
    }

    if (e.find("size") != e.end())
    {
        if (e["size"].is<std::string>())
        {
            QString size = e["size"].get<std::string>().c_str();
            QStringList list = size.split(";");
            if (list.size() == 2)
            {
                entry.Size.setValue(QSizeF(list[0].toInt(), list[1].toInt()));
            }
            else
            {
                errors << QString("Sprite size is not defined as  \"width;height\": \n") + picojson::value(e).serialize().c_str();
            }

        }
        else
        {
            errors << QString("Sprite size is not a string in entry: \n") + picojson::value(e).serialize().c_str();
        }
    }

    if (e.find("transparent") != e.end())
    {
        if (e["transparent"].is<std::string>())
        {
            QString size = e["transparent"].get<std::string>().c_str();
            QStringList list = size.split(";");
            if (list.size() == 3)
            {
                int r = std::min(std::max(0, list[0].toInt()), 255);
                int g = std::min(std::max(0, list[1].toInt()), 255);
                int b = std::min(std::max(0, list[2].toInt()), 255);
                entry.Transparent.setValue(QColor(r, g, b));
            }
            else
            {
                errors << QString("Sprite transparency is not defined as  \"r;g;b\": \n") + picojson::value(e).serialize().c_str();
            }

        }
        else
        {
            errors << QString("Sprite transparency is not a string in entry: \n") + picojson::value(e).serialize().c_str();
        }
    }

    m_errors << errors;
    if (errors.size() == 0)
    {
        if (atlas->hasEntry(entry.Name.value(), entry.Index))
        {
            m_errors << QString("Element with name ") + entry.getFullName() + QString(" already exists");
        }
        else
        {
            atlas->pushEntry(entry);
        }
    }
}


void JSONReader::readAtlasElement(picojson::object& e, Atlas* atlas, QImage& large)
{
    QVector<QString> errors;
    AtlasEntry entry;

    if (e.find("name") != e.end())
    {
        if (e["name"].is<std::string>())
        {
            entry.Name.setValue(e["name"].get<std::string>().c_str());
        }
        else
        {
            errors << QString("Sprite name is not a string in entry: \n") + picojson::value(e).serialize().c_str();
        }
    }
    else
    {
        errors << QString("Sprite name is not found in entry: \n") + picojson::value(e).serialize().c_str();
    }

    if (e.find("index") != e.end())
    {
        bool handled = false;

        if (e["index"].is<double>())
        {
             entry.Index.setValue(static_cast<int>(e["index"].get<double>()));
        }

        if (e["index"].is<std::string>())
        {
            bool ok = false;
            int i = QString(e["index"].get<std::string>().c_str()).toInt(&ok);
            if (ok)
            {
                entry.Index.setValue(i);
            }
        }

        if (!handled)
        {
            errors << QString("Sprite index is not a string in entry: \n") + picojson::value(e).serialize().c_str();
        }
    }

    if (e.find("texrect") != e.end())
    {
        if (e["texrect"].is<std::string>())
        {
            QString texrect = e["texrect"].get<std::string>().c_str();
            QStringList texrectparts = texrect.split(";");
            if (texrectparts.size() == 4)
            {
                int x = std::max(0, texrectparts[0].toInt());
                int y = std::max(0, texrectparts[1].toInt());
                int w = std::max(0, texrectparts[2].toInt());
                int h = std::max(0, texrectparts[3].toInt());				
                char chars[37] = "0123456789abcdefghijklmnopqrstuvwxyz";
                QString resultingTextureName = entry.Name.value();
                resultingTextureName += "-atlas-entry-";
                for(size_t i = 0; i < 15; i++) {
                    resultingTextureName += chars[rand() % 36];
                }
                Texture * t = new Texture(resultingTextureName);
                t->Image = large.copy(x, y, w, h);
                atlas->pushTexture(t);
                entry.InputTextureName.setValue(resultingTextureName);				
            }                       
        }
        else
        {
            errors << QString("Sprite texture rectangle is not a string in entry: \n") + picojson::value(e).serialize().c_str();
        }
    }
    else
    {
        errors << QString("Sprite texture rectangle is not found in entry: \n") + picojson::value(e).serialize().c_str();
    }

    if (e.find("size") != e.end())
    {
        if (e["size"].is<std::string>())
        {
            QString size = e["size"].get<std::string>().c_str();
            QStringList list = size.split(";");
            if (list.size() == 2)
            {
                entry.Size.setValue(QSizeF(list[0].toInt(), list[1].toInt()));
            }
            else
            {
                errors << QString("Sprite size is not defined as  \"width;height\": \n") + picojson::value(e).serialize().c_str();
            }

        }
        else
        {
            errors << QString("Sprite size is not a string in entry: \n") + picojson::value(e).serialize().c_str();
        }
    }
    else
    {
         errors << QString("Sprite size is not found in entry: \n") + picojson::value(e).serialize().c_str();
    }

    if (e.find("transparent") != e.end())
    {
        if (e["transparent"].is<std::string>())
        {
            QString size = e["transparent"].get<std::string>().c_str();
            QStringList list = size.split(";");
            if (list.size() == 3)
            {
                int r = std::min(std::max(0, list[0].toInt()), 255);
                int g = std::min(std::max(0, list[1].toInt()), 255);
                int b = std::min(std::max(0, list[2].toInt()), 255);
                entry.Transparent.setValue(QColor(r, g, b));
            }
            else
            {
                errors << QString("Sprite transparency is not defined as  \"r;g;b\": \n") + picojson::value(e).serialize().c_str();
            }

        }
        else
        {
            errors << QString("Sprite transparency is not a string in entry: \n") + picojson::value(e).serialize().c_str();
        }
    }

    m_errors << errors;
    if (errors.size() == 0)
    {
        if (atlas->hasEntry(entry.Name.value(), entry.Index))
        {
            m_errors << QString("Element with name ") + entry.getFullName() + QString(" already exists");
        }
        else
        {
            atlas->pushEntry(entry);
        }
    }
}

QStringList JSONReader::removeExtension(const QString& name)
{
    QStringList name_parts = name.split(".");
    if (name_parts.size() > 1) {
        name_parts.removeAt(name_parts.size() - 1);
    }
    return name_parts;
}
