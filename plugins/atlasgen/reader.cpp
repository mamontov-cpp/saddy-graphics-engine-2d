#include "reader.h"

Reader::Reader() : m_preserve_unique_textures(true), m_ok(true)
{

}

Reader::~Reader()
{

}

bool Reader::shouldPreserveUniqueTextures() const
{
    return m_preserve_unique_textures;
}

void Reader::toggleShouldPreserveUniqueTextures(bool flag)
{
    m_preserve_unique_textures = flag;
}


QVector<QString>& Reader::errors()
{
    return m_errors;
}

const QVector<QString>& Reader::errors() const
{
    return m_errors;
}

bool Reader::ok() const
{
    return m_ok;
}
