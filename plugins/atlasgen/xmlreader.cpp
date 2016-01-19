#include "xmlreader.h"

#include <QtCore/QFile>

XMLReader::XMLReader()
{

}

XMLReader::~XMLReader()
{

}


void XMLReader::read(const QString &name, Atlas* atlas)
{
    m_ok = false;
    QDomDocument doc;
    QFile file(name); 
    if (!file.open(QIODevice::ReadOnly)) {
        m_errors << QString("Can\'t open file \"") + name + QString("\"");
        return;
    }
    if (!doc.setContent(&file)) {
        m_errors << QString("Can\'t parse file \"") + name + QString("\"");
        file.close();
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();
    
    // Scan config part
    if (root.hasAttribute("config") == false)
    {
        m_errors << "Output atlas config filename is not defined";
    } 
    else
    {
        atlas->setOutputName(root.attribute("config"));
    }

    // Set output texture
    if (root.hasAttribute("texture") == false)
    {
        m_errors << "Output texture is not defined";
    } 
    else
    {
        atlas->setOutputTexture(root.attribute("texture"));
    }

    for(QDomNode n = root.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElement e = n.toElement();
        if (!e.isNull())
        {
             readElement(e, atlas);
        }
    }

    m_ok = m_errors.size() == 0;
}

void XMLReader::readElement(const QDomElement& e, Atlas* atlas)
{
    QVector<QString> errors;
    AtlasEntry entry;
    entry.Name.setValue(e.tagName());

    // Parse index
    if (e.hasAttribute("index"))
    {
        bool ok = false;
        int i = e.attribute("index").toInt(&ok);
        if (ok)
        {
            entry.Index.setValue(i);
        }
    }

    // Parse texture
    if (e.hasAttribute("texture"))
    {
        QString texturename = e.attribute("texture");
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
    }
    else
    {
        errors << QString("In element with full name") + entry.getFullName() + QString(" texture is not specified");
    }

    // Parse size
    if (e.hasAttribute("size"))
    {
        QString size = e.attribute("size");
        QStringList list = size.split(";");
        if (list.size() == 2)
        {
            entry.Size.setValue(QSizeF(list[0].toInt(), list[1].toInt()));
        }
        else
        {
            errors << QString("In element with full name") + entry.getFullName() + QString(" size must be defined as \"width;height\", but defined incorrectly");
        }
    }


    if (e.hasAttribute("transparent"))
    {
        QString size = e.attribute("transparent");
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
            errors << QString("In element with full name") + entry.getFullName() + QString(" color must be defined as \"r;g;b\", but defined incorrectly");
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
