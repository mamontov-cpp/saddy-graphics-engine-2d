#include "atlas.h"
#include "outputoptions.h"

Atlas::Atlas() : m_take_first(true)
{

}

bool Atlas::hasEntry(const QString& name, const sad::Maybe<int>& index)
{
    for(size_t i = 0; i < m_entries.size(); i++)
    {
        bool ok  = name == m_entries[i].Name.value();
        if (ok && index.exists() == m_entries[i].Index.exists())
        {
            if (index.exists())
            {
                if (m_entries[i].Index.value() == index.value())
                {
                    return true;
                }
            }
            else
            {
                return true;
            }
        }
    }
    return false;
}

void Atlas::pushEntry(const AtlasEntry& entry)
{
    m_entries << entry;
}

const TextureArray& Atlas::textures() const
{
    return m_textures;
}

const QVector<AtlasEntry>& Atlas::entries() const
{
    return m_entries;
}

QVector<AtlasEntry>& Atlas::entries()
{
    return m_entries;
}

::TextureArray& Atlas::textures()
{
    return m_textures;
}


void Atlas::prepareForOutput(const OutputOptions& options)
{
    bool added_one_pixel = (*(options.ProgramOptions))["add-pixel"].toBool();
    QString tarname_protocol_string;
    if (options.ProgramOptions->contains("write-to-tar"))
    {
        tarname_protocol_string = (*(options.ProgramOptions))["short-tar-name"].toString();
        size_t length = tarname_protocol_string.size();
        tarname_protocol_string.prepend("://");
        tarname_protocol_string.prepend(QString::number(length));
        tarname_protocol_string.prepend("tar7z:");
        tarname_protocol_string.append("/");
        this->m_output_texture = tarname_protocol_string + OutputOptions::tar7zCompatibleName(this->m_output_texture);
    }

    for(size_t i = 0; i < m_entries.size(); i++)
    {
        Texture * t = m_textures.get(m_entries[i].InputTextureName.value());
        if (t)
        {
            m_entries[i].TextureRectangle.setValue(t->TextureRectangle);
            if (added_one_pixel)
            {
                QRectF oldrect = m_entries[i].TextureRectangle.value();
                QRectF newrect(oldrect.left() + 1.0, oldrect.top() + 1.0, oldrect.width(), oldrect.height());
                m_entries[i].TextureRectangle.setValue(newrect);
            }
            if (m_entries[i].Size.exists() == false)
            {
                m_entries[i].Size.setValue(t->size());
            }
            QString name = m_output_texture;
            if (tarname_protocol_string.size())
            {
                name = OutputOptions::tar7zCompatibleName(name);
                name.prepend(tarname_protocol_string);
            }
            m_entries[i].OutputTextureName.setValue(name);
        }
    }
}

void Atlas::setOutputName(const QString& name)
{
    if ((m_output_name.length() == 0) || !m_take_first) { 
        m_output_name = name;
    }
}

const QString& Atlas::outputName() const
{
    return m_output_name;
}

void Atlas::setOutputTexture(const QString& name, bool ignore_settings)
{
    if ((m_output_texture.length() == 0) || (!m_take_first) || ignore_settings) {
        m_output_texture = name;
    }
}

const QString& Atlas::outputTexture() const
{
    return m_output_texture;
}

void Atlas::setResourceName(const QString& name)
{
    if ((m_texture_resource_name.exists() == false) || !m_take_first) {
        m_texture_resource_name.setValue(name);    
    }
}

const QString& Atlas::resourceName() const
{
    return m_texture_resource_name.value();
}

bool Atlas::hasResourceName() const
{
    return m_texture_resource_name.exists();
}

bool Atlas::hasTexture(const QString& name) const
{
    return m_textures.contains(name);
}

void Atlas::pushTexture(Texture* t)
{
    m_textures << t;
}

Texture* Atlas::getTexture(const QString& name) const
{
    return m_textures.get(name);
}


void Atlas::toggleFlagForChangingOutputPropertiesOnlyOnce(bool flag)
{
    m_take_first = flag;
}

