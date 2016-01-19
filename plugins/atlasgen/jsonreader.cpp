#include "jsonreader.h"

#include <QtCore/QTextStream>

JSONReader::JSONReader()
{

}

JSONReader::~JSONReader()
{

}

void JSONReader::read(const QString& name)
{
    this->Successfull = false;
    QFile file(name); 
    if (!file.open(QIODevice::ReadOnly)) {
        this->Errors << QString("Can\'t open file \"") + name + QString("\"");
        Result->Errors << this->Errors;
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

            // Parse texture
            if (ro.find("texture") != ro.end())
            {
                if (ro["texture"].is<std::string>())
                {
                    this->OutputTexture = ro["texture"].get<std::string>().c_str();
                }
                else
                {
                    this->Errors << "Output texture is not a string";
                }
            }
            else
            {
                this->Errors << "Output texture is not defined";
            }


            // Parse config
            if (ro.find("config") != ro.end())
            {
                if (ro["config"].is<std::string>())
                {
                    this->OutputName = ro["config"].get<std::string>().c_str();
                }
                else
                {
                    this->Errors << "Output atlas config filename is not a string";
                }
            }
            else
            {
                this->Errors << "Output atlas config filename is not defined";
            }

            // Parse texture resource name
            if (ro.find("resource") != ro.end())
            {
                if (ro["resource"].is<std::string>())
                {
                    this->Result->TextureResourceName.setValue(ro["resource"].get<std::string>().c_str());
                }
                else
                {
                    this->Errors << "Output resource is not a string";
                }
            }
            else
            {
                this->Errors << "Output resource is not defined";
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
                            readElement(a[i].get<picojson::object>());
                        }
                        else
                        {
                            this->Errors << QString("Found entry of sprites, which is not an object: \n") + a[i].serialize().c_str();
                        }
                    }
                }
                else
                {
                    this->Errors << "Sprites list is not an array";
                }
            }
            else
            {
                 this->Errors << "Sprites list is not defined";
            }

        }
        else
        {
            this->Errors << QString("Root element of file \"") + name + QString("\" is not an object");
        }
    }
    else
    {
        this->Errors << QString("Can\'t parse file \"") + name + QString("\"");
    }

    this->Successfull = this->Errors.size() == 0;
    Result->Errors << this->Errors;
}


void JSONReader::readElement(picojson::object& e)
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
            int i = 0;
            i = QString(e["index"].get<std::string>().c_str()).toInt(&ok);
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
            if (Result->Textures.contains(texturename) == false)
            {
                Texture* t = new Texture(texturename);
                if (t->load())
                {
                    Result->Textures << t;
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
					Texture* t = new Texture(*(Result->Textures.get(texturename)));
					t->Name += entry.Name.value();
					entry.InputTextureName.setValue(t->Name);
					Result->Textures << t;
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

    this->Errors << errors;
    if (errors.size() == 0)
    {
        if (this->Result->hasEntry(entry.Name.value(), entry.Index))
        {
            this->Errors << QString("Element with name ") + entry.getFullName() + QString(" already exists");
        }
        else
        {
            this->Result->Entries << entry;
        }
    }
}
