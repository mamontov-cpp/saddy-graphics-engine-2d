/*! \file gluemetric.h
    \author HiddenSeeker

    Describes a metric which should be used, when merging objects
 */
#pragma once
#include "glueentry.h"
#include "glueorder.h"

class GlueMetric
{
public:
    GlueMetric();

    /*! Computes a maximum between size dimensions specified by index between images, specified by order
        on an entries of entries
        \param[in] entries source array of entry
        \param[in] order source order to work with
        \param[in] index 0 - for width, 1 - for height
        \return result
     */
    static double maxMerge(const QVector<GlueEntry>& entries, GlueOrder order, int index);

    /*! Computes a sum between size dimensions specified by +index+ between images, specified by +order+
        on an entries of +entries+
        \param[in] entries source array of entry
        \param[in] order source order to work with
        \param[in] index 0 - for width, 1 - for height
        \return result
     */
    static double sumMerge(const QVector<GlueEntry>& entries, GlueOrder order, int index);
};
