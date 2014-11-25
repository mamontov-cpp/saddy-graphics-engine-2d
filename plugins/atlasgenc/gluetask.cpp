#include "gluetask.h"


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
    entries.remove(order.Images[1]);
    entries.remove(order.Images[0]);
    orders = orders << order;
    return GlueTask(entries, orders);
}

