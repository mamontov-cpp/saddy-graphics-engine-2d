#include "glueentry.h"
#include "texture.h"
#include "gluemetric.h"

#include <cstdio>

GlueEntry::GlueEntry()
{
     push_back(0);
     push_back(0);
}

GlueEntry::GlueEntry(const Texture & t)
{
    push_back(t.size().width());
    push_back(t.size().height());
}

GlueEntry::GlueEntry(double x, double y)
{
    push_back(x);
    push_back(y);
}

GlueEntry::~GlueEntry()
{

}

GlueEntry GlueEntry::merge(const QVector<GlueEntry>& entries,
                           GlueOrder& order
                           )
{
    double w = 0;
    double h = 0;
    // Handle a horizontal merge
    if (order.Mode == HORIZONTAL)
    {
        w = GlueMetric::sumMerge(entries, order, 0);
        h = GlueMetric::maxMerge(entries, order, 1);
    }
    else
    {
        // Handle vertical merge
        w = GlueMetric::maxMerge(entries, order, 0);
        h = GlueMetric::sumMerge(entries, order, 1);
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
