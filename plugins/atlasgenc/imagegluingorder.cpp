#include "imagegluingorder.h"

#include "mindiffmetric.h"
#include "minareametric.h"

ImageGluingOrder::Result::Result() : Size(0, 0)
{

}

ImageGluingOrder::Result::~Result()
{

}

ImageGluingOrder::ImageGluingOrder()
{

}

ImageGluingOrder::~ImageGluingOrder()
{

}

ImageGluingOrder::Result ImageGluingOrder::find(const TextureArray& images)
{
    ImageGluingOrder::Result result;
    if (images.size())
    {
        if (images.size() == 1)
        {
            result.Size = images[0]->size();
        }
        else
        {
            MinAreaMetric mametric;
            MinDiffMetric mdmetric;

            QVector<GlueEntry> entries;
            for(size_t i = 0; i < images.size(); i++)
            {
                entries << GlueEntry(images[i]->size().width(), images[i]->size().height());
            }
            QVector<GlueMetric::OrdersAndSize> maresult = mametric.findOrder(entries);
            QVector<GlueMetric::OrdersAndSize> mdresult = mdmetric.findOrder(entries);
            QPair<MaybeNumber, GlueMetric::OrdersAndSize> min;
            min = GlueMetric::findMinimumOrder(min, maresult);
            min = GlueMetric::findMinimumOrder(min, mdresult);
            result.Order = min.second.first;
            result.Size = QSizeF(min.second.second[0], min.second.second[1]);
        }
    }
    return result;
}
