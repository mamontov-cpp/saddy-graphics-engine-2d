#include "texture.h"

Texture::Texture(const QString& name) : Name(name)
{

}

bool Texture::load()
{
	return this->Image.load(this->Name);
}

QSizeF Texture::size() const
{
	return QSizeF(this->Image.width(), this->Image.height());
}
