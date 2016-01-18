/*! \file minareametric.h
    

    Describes a metric which, that minimizes area of resulting texture
 */
#pragma once
#include "gluemetric.h"

/*! Computes a metric, that minimizes area of resulting texture
 */
class MinAreaMetric: public GlueMetric
{
public:
    /*! Defines a default metric
     */
    MinAreaMetric();
    /*! Can be inherited
     */
    virtual ~MinAreaMetric();
    /*! Computes a metric, that minimizes area of resulting texture
        \param[in] entries an entries
        \param[in] order an order, how they should be merged
        \return metric value
     */
    virtual double getMetric(
            const QVector<GlueEntry>& entries,
            const GlueOrder& order
    );
};

