#include "minareametric.h"

MinAreaMetric::MinAreaMetric()
{

}

MinAreaMetric::~MinAreaMetric()
{

}

double MinAreaMetric::getMetric(
   const QVector<GlueEntry>& entries,
   const GlueOrder& order
)
{
    double w = 0;
    double h = 0;
    if (order.Mode == HORIZONTAL)
    {
        w = GlueMetric::sumMerge(entries, order, 0);
        h = GlueMetric::maxMerge(entries, order, 1);
    }
    else
    {
        w = GlueMetric::maxMerge(entries, order, 0);
        h = GlueMetric::sumMerge(entries, order, 1);
    }
   return w*h;
}
