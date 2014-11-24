#include "glueentry.h"
#include "texture.h"

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

GlueEntry GlueEntry::merge(const QVector<GlueEntry>& entries,
                           GlueOrder& order
                           )
{
    #error "Not implemented GlueEntry::merge"
    return GlueEntry();
}
