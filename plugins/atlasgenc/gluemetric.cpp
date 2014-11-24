#include "gluemetric.h"

GlueMetric::GlueMetric()
{

}

double GlueMetric::maxMerge(const QVector<GlueEntry>& entries, GlueOrder order, int index)
{
    bool maxexists = false;
    double max = 0;
    for(size_t i = 0; i < order.Images.size(); i++)
    {
        double maxcandidate = entries[i][index];
        if (maxexists == false || maxcandidate > max) {
            max = maxcandidate;
            maxexists = true;
        }
    }
    return max;
}

double GlueMetric::sumMerge(const QVector<GlueEntry>& entries, GlueOrder order, int index)
{
    double result = 0;
    for(size_t i = 0; i < order.Images.size(); i++)
    {
        result += entries[i][index];

    }
    return result;
}
