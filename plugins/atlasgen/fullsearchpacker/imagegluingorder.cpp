#include "imagegluingorder.h"

#include "mindiffmetric.h"
#include "minareametric.h"

#include <cstdio>

fullsearchpacker::ImageGluingOrder::Result::Result() : Size(0, 0)
{

}

fullsearchpacker::ImageGluingOrder::Result::~Result()
{

}

fullsearchpacker::ImageGluingOrder::ImageGluingOrder()
{

}

fullsearchpacker::ImageGluingOrder::~ImageGluingOrder()
{

}

fullsearchpacker::ImageGluingOrder::Result fullsearchpacker::ImageGluingOrder::find(const TextureArray& images)
{
    fullsearchpacker::ImageGluingOrder::Result result;
    if (images.size())
    {
        //printf("Count of images: %d\n", images.size());
        if (images.size() == 1)
        {
            result.Size = images[0]->size();
        }
        else
        {
            fullsearchpacker::MinAreaMetric mametric;
            fullsearchpacker::MinDiffMetric mdmetric;

            QVector<fullsearchpacker::GlueEntry> entries;
            for(size_t i = 0; i < images.size(); i++)
            {
                entries << fullsearchpacker::GlueEntry(images[i]->size().width(), images[i]->size().height());
            }
            fullsearchpacker::GlueMetric::OrdersAndSize maresult = mametric.findOrder(entries);
            fullsearchpacker::GlueMetric::OrdersAndSize mdresult = mdmetric.findOrder(entries);
            QPair<MaybeNumber, fullsearchpacker::GlueMetric::OrdersAndSize> min;
            min = fullsearchpacker::GlueMetric::findMinimumOrder(min, maresult);
            min = fullsearchpacker::GlueMetric::findMinimumOrder(min, mdresult);
            result.Order = min.second.first;
            result.Size = QSizeF(min.second.second[0], min.second.second[1]);            
        }
    }
    return result;
}
