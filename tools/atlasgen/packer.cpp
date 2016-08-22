#include "packer.h"

#include <QPainter>

// ================================== PUBLIC METHODS ==================================

Packer::Packer()
{
    
}

void Packer::setOptions(QHash<QString, QVariant>* options)
{
    this->m_options = options;
}

Packer::~Packer()
{
    
}

// ================================== PROTECTED METHODS ==================================

void Packer::copyImages(Atlas& atlas, QImage*& image)
{
    bool add_pixel = (*m_options)["add-pixel"].toBool();

    QPainter painter(image);
    for(size_t i = 0; i < atlas.textures().size(); i++)
    {
        Texture* t = atlas.textures()[i];
        QPointF topleft = t->TextureRectangle.topLeft();
        const QImage& img = t->Image;
        if (add_pixel)
        {
            // Top part
            painter.drawImage(
                topleft.x(),
                topleft.y(),
                img,
                0,
                0,
                1,
                1
            );
            painter.drawImage(
                topleft.x() + 1,
                topleft.y(),
                img,
                0,
                0,
                img.width(),
                1
            );
            painter.drawImage(
                topleft.x() + img.width() + 1,
                topleft.y(),
                img,
                img.width() - 1,
                0,
                1,
                1
            );
            // Middle part
            painter.drawImage(
                topleft.x(),
                topleft.y() + 1,
                img,
                0,
                0,
                1,
                img.height()
            );
            painter.drawImage(
                topleft + QPointF(1, 1),
                img
            );
            painter.drawImage(
                topleft.x() + img.width() + 1,
                topleft.y() + 1,
                img,
                img.width() - 1,
                0,
                1,
                img.height()
            );
            // Bottom part
            painter.drawImage(
                topleft.x(),
                topleft.y() + img.height() + 1,
                img,
                0,
                img.height() - 1,
                1,
                1
            );
            painter.drawImage(
                topleft.x() + 1,
                topleft.y() + img.height() + 1,
                img,
                0,
                img.height() - 1,
                img.width(),
                1
            );
            painter.drawImage(
                topleft.x() + img.width() + 1,
                topleft.y() + img.height() + 1,
                img,
                img.width()  - 1,
                img.height() - 1,
                1,
                1
            );
        }
        else
        {
            painter.drawImage(
                topleft,
                img
            );
        }
    }
    painter.end();
}
