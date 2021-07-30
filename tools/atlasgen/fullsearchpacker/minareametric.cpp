#include "minareametric.h"

fullsearchpacker::MinAreaMetric::MinAreaMetric()
{

}

fullsearchpacker::MinAreaMetric::~MinAreaMetric()
{

}

double fullsearchpacker::MinAreaMetric::getMetric(
   const QVector<fullsearchpacker::GlueEntry>& entries,
   const fullsearchpacker::GlueOrder& order
)
{
    double w = 0;
    double h = 0;
    if (order.Mode == fullsearchpacker::GlueMode::HORIZONTAL)
    {
        w = fullsearchpacker::GlueMetric::sumMerge(entries, order, 0);
        h = fullsearchpacker::GlueMetric::maxMerge(entries, order, 1);
    }
    else
    {
        w = fullsearchpacker::GlueMetric::maxMerge(entries, order, 0);
        h = fullsearchpacker::GlueMetric::sumMerge(entries, order, 1);
    }
   return w*h;
}
