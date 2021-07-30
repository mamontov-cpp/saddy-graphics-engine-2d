#include "imagearranger.h"

#include <cstdio>

fullsearchpacker::ImageArranger::Bucket::Bucket() = default;

fullsearchpacker::ImageArranger::Bucket::Bucket(Texture *t, double pad_x, double pad_y)
{
    this->Images << t;
    this->Size = t->size();
    this->Size.setWidth(this->Size.width() + pad_x);
    this->Size.setHeight(this->Size.height() + pad_y);
}

fullsearchpacker::ImageArranger::Bucket::~Bucket() = default;

void fullsearchpacker::ImageArranger::Bucket::shift(const QPointF& p) const
{
    for(size_t i = 0; i < this->Images.size(); i++)
    {
        Texture* t = this->Images[i];
        t->TextureRectangle.moveTopLeft(
            t->TextureRectangle.topLeft() + p
        );
    }
}

fullsearchpacker::ImageArranger::Bucket fullsearchpacker::ImageArranger::Bucket::merge(
    const fullsearchpacker::ImageArranger::Bucket& bucket1,
    const fullsearchpacker::ImageArranger::Bucket& bucket2,
    const fullsearchpacker::GlueOrder& order
)
{
    fullsearchpacker::ImageArranger::Bucket result;
    result.Images = bucket1.Images;
    result.Images << bucket2.Images;

    if (order.Mode == GlueMode::HORIZONTAL)
    {
        // Handle horizontal merge
        result.Size = QSizeF(
            bucket1.Size.width() + bucket2.Size.width(), 
            std::max(bucket1.Size.height() , bucket2.Size.height())
        );
        bucket2.shift(QPointF(bucket1.Size.width(),0));
    } 
    else
    {
        // Handle vertical merge
        result.Size = QSizeF(
            std::max(bucket1.Size.width() , bucket2.Size.width()),
            bucket1.Size.height() + bucket2.Size.height()	
        );
        bucket2.shift(QPointF(0, bucket1.Size.height()));
    }
    return result;
}


fullsearchpacker::ImageArranger::ImageArranger()
{

}

fullsearchpacker::ImageArranger::~ImageArranger()
{

}

double fullsearchpacker::ImageArranger::nextPOT(double value)
{
    unsigned int  size = 1;
    while(size < value)
    {
        size =  size << 1;
    }
    return static_cast<double>(size);
}


double fullsearchpacker::ImageArranger::arrange(
    const QHash<QString, QVariant>& options,
    const TextureArray& images, 
    const QVector<fullsearchpacker::GlueOrder>& order, 
    const QSizeF& totalSize
)
{
    double padx = 0;
    double pady = 0;
    if (options["add-pixel"].toBool())
    {
        padx = 2;
        pady = 2;
    }
    if (images.size() > 0)
    {
        if (images.size() == 1)
        {
            int width = images[0]->Image.width();
            int height = images[0]->Image.height();
            images[0]->TextureRectangle = QRectF(QPointF(0, 0), QSizeF(width, height));
        }
        else
        {	
            QVector<fullsearchpacker::ImageArranger::Bucket> buckets;
            for(size_t i = 0; i < images.size(); i++)
            {
                buckets << fullsearchpacker::ImageArranger::Bucket(images[i], padx, pady);
            }
            for(size_t i = 0; i < order.size(); i++)
            {
                fullsearchpacker::GlueOrder entry = order[i];

                QVector<size_t> indexes = entry.Images;
                if (indexes[0] < indexes[1])
                {
                    std::swap(indexes[0], indexes[1]);
                }
                
                fullsearchpacker::ImageArranger::Bucket new_bucket =  fullsearchpacker::ImageArranger::Bucket::merge(buckets[entry.Images[0]], buckets[entry.Images[1]], entry);
                buckets.remove(indexes[0]);
                buckets.remove(indexes[1]);
                buckets << new_bucket;
            }
        }
    }
    double largesize = std::max(totalSize.width(), totalSize.height());
    //printf("Total size: %lf\n", largesize);
    return fullsearchpacker::ImageArranger::nextPOT(largesize);
}


