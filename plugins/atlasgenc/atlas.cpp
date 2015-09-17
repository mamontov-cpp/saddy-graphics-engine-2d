#include "atlas.h"

Atlas::Atlas()
{

}

bool Atlas::hasEntry(const QString& name, const sad::Maybe<int>& index)
{
    for(size_t i = 0; i < Entries.size(); i++)
    {
        bool ok  = name == Entries[i].Name.value();
        if (ok && index.exists() == Entries[i].Index.exists())
        {
            if (index.exists())
            {
                if (Entries[i].Index.value() == index.value())
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
    Entries << entry;
}

const TextureArray& Atlas::textures() const
{
    return Textures;
}

const QVector<AtlasEntry>& Atlas::entries() const
{
    return Entries;
}

const QVector<QString>& Atlas::errors() const
{
    return Errors;
}

bool Atlas::loadTexture(const QString& filename)
{
    if (Textures.get(filename))
    {
        return true;
    }

    Texture* t = new Texture(filename);
    if (t->load() == false)
    {
        delete t;
        Errors << (QString("Can\'t load texture from file \"") + filename + QString("\""));
        return false;
    }

    Textures << t;
    return true;
}

void Atlas::prepareForOutput(const QString& outputTextureName)
{
    for(size_t i = 0; i < Entries.size(); i++)
    {
        Texture * t = Textures.get(Entries[i].InputTextureName.value());
        if (t)
        {
            Entries[i].TextureRectangle.setValue(t->TextureRectangle);
            if (Entries[i].Size.exists() == false)
            {
                Entries[i].Size.setValue(t->size());
            }
            Entries[i].OutputTextureName.setValue(outputTextureName);
        }
    }
}
