#include "texturearray.h"

#include <stdexcept>

TextureArray::TextureArray()
{

}

TextureArray::~TextureArray()
{
    for(size_t i = 0; i < this->size(); i++)
    {
        delete (*this)[i];
    }
}

bool TextureArray::pushUnique(Texture* t)
{
    if (!t)
    {
        throw std::runtime_error("TextureArray::pushUnique: Texture must be not nullptr");
    }
    if (contains(t->Name))
    {
        return false;
    }
    (*this) << t;
    return true;
}

bool TextureArray::contains(const QString& name) const
{
    for(size_t i = 0; i < this->size(); i++)
    {
        if ((*this)[i]->Name == name)
        {
            return true;
        }
    }
    return false;
}

Texture* TextureArray::get(const QString& name) const
{
    for(size_t i = 0; i < this->size(); i++)
    {
        if ((*this)[i]->Name == name)
        {
            return (*this)[i];
        }
    }
    return nullptr;
}
