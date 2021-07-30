#include "glueentry.h"
#include "../texture.h"
#include "gluemetric.h"


fullsearchpacker::GlueEntry::GlueEntry()
{
     m_a[0] = 0;
     m_a[1] = 0;
}

fullsearchpacker::GlueEntry::GlueEntry(double x, double y)
{
    m_a[0]  = (x);
    m_a[1] = (y);
}

fullsearchpacker::GlueEntry fullsearchpacker::GlueEntry::merge(
    const QVector<fullsearchpacker::GlueEntry>& entries,
    fullsearchpacker::GlueOrder& order
)
{
    double w = 0;
    double h = 0;
    // Handle a horizontal merge
    if (order.Mode == GlueMode::HORIZONTAL)
    {
        w = fullsearchpacker::GlueMetric::sumMerge(entries, order, 0);
        h = fullsearchpacker::GlueMetric::maxMerge(entries, order, 1);
    }
    else
    {
        // Handle vertical merge
        w = fullsearchpacker::GlueMetric::maxMerge(entries, order, 0);
        h = fullsearchpacker::GlueMetric::sumMerge(entries, order, 1);
    }
    /*
    printf("Entries: (%lf, %lf) +(%d) (%lf, %lf)  = (%lf,%lf)\n",
           entries[order.Images[0]][0], entries[order.Images[0]][1],
           order.Mode,
           entries[order.Images[1]][0], entries[order.Images[1]][1],
           w, h
          );
    */
    return GlueEntry(w, h);
}
