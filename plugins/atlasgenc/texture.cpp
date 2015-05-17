#include "texture.h"

Texture::Texture(const QString& name) : Name(name)
{

}

bool Texture::load()
{
	bool result = this->Image.load(this->Name);
	if (result)
	{
		this->TextureRectangle = QRectF(QPointF(0, 0), this->size());
	}
	return result;
}

QSizeF Texture::size() const
{
	return QSizeF(this->Image.width(), this->Image.height());
}
