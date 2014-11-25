/*! \file gluetask.h
    \author HiddenSeeker

    A task, which is used by metrics to find recursively an order.
    A metric must merge all +entries+ into one, extending +orders+ array
 */
#include <glueentry.h>
#include <glueorder.h>


/*! A task, which is used by metrics to find recursively an order.
    A metric must merge all +entries+ into one, extending +orders+ array
 */
class GlueTask
{
public:
    /*! Entries, being merged into one
     */
    QVector<GlueEntry> Entries;
    /*! An orders to be merged into one
     */
    QVector<GlueOrder> Orders;

    /*! Constructs empty glue task
     */
    GlueTask();
    /*! Contructs new task
        \param[in] entries an entries list
        \param[in] orders an orders list
     */
    GlueTask(const QVector<GlueEntry>& entries, const QVector<GlueOrder>& orders);
    /*! Could be inherited
     */
    virtual ~GlueTask();
    /*! Applies a new order, merging some specified in order images into one and adding it to an end
        of resulting task and adding order to a part
        \param[in] order an order to be applied
        \return new task with order, which was applied
     */
    GlueTask applyOrder(const GlueOrder& order);
};
