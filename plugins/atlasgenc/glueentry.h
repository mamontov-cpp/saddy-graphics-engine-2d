/*! \file glueentry.h
    \author HiddenSeeker

    Defines an entry for gluing textures
 */
#pragma once

#include <QtCore/QVector>
#include <QtCore/QString>

class Texture;
class GlueOrder;


/*! An entry size for gluing
    first element is width,
    second is height.
 */
class GlueEntry: public QVector<double>
{
public:
    /*! Constructs default entry
     */
    GlueEntry();
    /*! Constructs an entry from texture
        \param[in] t texture
     */
    GlueEntry(const Texture & t);
    /*! Constructs an entry from two coordinates
        \param[in] x first coordinate
        \param[in] y second coordinate
     */
    GlueEntry(double x, double y);
    /*! Can be inherited
     */
    virtual ~GlueEntry();
    /*! Merges all entries, according specified order
        \param[in] entries a list of entries to be merged
        \param[in] order   an order, in which they should be merged
     */
    static GlueEntry merge(const QVector<GlueEntry>& entries,
                           GlueOrder& order
                           );
};
