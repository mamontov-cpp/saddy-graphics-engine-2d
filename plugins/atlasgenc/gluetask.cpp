#include "gluetask.h"
#include <cassert>

GlueTask::GlueTask()
{

}

GlueTask::GlueTask(
        const QVector<GlueEntry>& entries,
        const QVector<GlueOrder>& orders
)
: Entries(entries), Orders(orders)
{

}


GlueTask::~GlueTask()
{

}

GlueTask GlueTask::applyOrder(const GlueOrder& order)
{
    QVector<GlueEntry> entries = this->Entries;
    QVector<GlueOrder> orders = this->Orders;
    GlueOrder local = order;
    entries << GlueEntry::merge(entries, local);
	size_t entriessize = entries.size();
	size_t index1 = order.Images[1];
	size_t index2 = order.Images[0];
	assert(index1 < entriessize);
	assert(index2 < entriessize);
    entries.remove(index1);
    entries.remove(index2);
    orders = orders << order;
    return GlueTask(entries, orders);
}

