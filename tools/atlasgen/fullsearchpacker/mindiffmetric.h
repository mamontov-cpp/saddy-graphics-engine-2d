/*! \file mindiffmetric.h
    

    Describes a metric which is described as a max(h1,h1)*(w2-w1), where h and w various parameters
 */
#pragma once
#include "gluemetric.h"

namespace fullsearchpacker
{

/*! Computes a metric which is described as a max(h1,h1)*(w2-w1), where h and w various parameters
 */
class MinDiffMetric: public fullsearchpacker::GlueMetric
{
public:
    /*! Defines a default metric
     */
    MinDiffMetric();
    /*! Can be inherited
     */
    virtual ~MinDiffMetric();
    /*! Computes a metric which is described as a max(h1,h1)*(w2-w1), where h and w various parameters
        \param[in] entries an entries
        \param[in] order an order, how they should be merged
        \return metric value
     */
    virtual double getMetric(
        const QVector<fullsearchpacker::GlueEntry>& entries,
        const fullsearchpacker::GlueOrder& order
    ) override;
};

}
