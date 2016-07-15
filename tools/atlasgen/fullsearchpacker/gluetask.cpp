#include "gluetask.h"
#include <cassert>

fullsearchpacker::GlueTask::GlueTask()
{

}

fullsearchpacker::GlueTask::GlueTask(
    const QVector<fullsearchpacker::GlueEntry>& entries,
    const QVector<fullsearchpacker::GlueOrder>& orders
)
: Entries(entries), Orders(orders)
{

}


fullsearchpacker::GlueTask::~GlueTask()
{

}

fullsearchpacker::GlueTask fullsearchpacker::GlueTask::applyOrder(const fullsearchpacker::GlueOrder& order)
{
    QVector<fullsearchpacker::GlueEntry> entries = this->Entries;
    QVector<fullsearchpacker::GlueOrder> orders = this->Orders;
    fullsearchpacker::GlueOrder local = order;
    fullsearchpacker::GlueEntry data = fullsearchpacker::GlueEntry::merge(entries, local);
    size_t entriessize = entries.size();
    size_t index1 = order.Images[1];
    size_t index2 = order.Images[0];
    assert(index1 < entriessize);
    assert(index2 < entriessize);
    entries.remove(index1);
    entries.remove(index2);
    entries << data;
    orders << order;
    return fullsearchpacker::GlueTask(entries, orders);
}

