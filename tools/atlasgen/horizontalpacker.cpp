#include "horizontalpacker.h"
#include "fullsearchpacker/imagearranger.h"
#include <functional>
#include <algorithm>

void HorizontalPacker::pack(Atlas& atlas, QImage*& image)
{
    double pad_x = 0;
    if ((*m_options)["add-pixel"].toBool())
    {
        pad_x = 2;
    }
    double width = 0.0, height = 0.0;
    TextureArray& images = atlas.textures();
    if (!images.empty())
    {
        if (images.size() == 1)
        {
            width = images[0]->Image.width();
            height = images[0]->Image.height();
            images[0]->TextureRectangle = QRectF(QPointF(0, 0), QSizeF(width, height));
        }
        else
        {
            const std::function<bool(Texture*, Texture*)> greater = [](Texture* t1, Texture* t2) -> bool {
                return t2->Image.height() < t1->Image.height();
            };
            std::sort(images.begin(), images.end(), greater);
            height = images[0]->Image.height();
            double x = 0;
            for (auto* current_image : images)
            {
                const double im_width = current_image->Image.width();
                const double im_height = current_image->Image.height();

                current_image->TextureRectangle = QRectF(QPointF(x, 0), QSizeF(im_width, im_height));
                x += current_image->Image.width() + pad_x;
            }
            width = x;
        }
    }
    const double max_size = std::max(width, height);

    const int width_height = static_cast<int>(fullsearchpacker::ImageArranger::nextPOT(max_size));

    image = new QImage(width_height, width_height, QImage::Format_ARGB32);
    image->fill(QColor(255, 255, 255, 0));

    this->copyImages(atlas, image);
}