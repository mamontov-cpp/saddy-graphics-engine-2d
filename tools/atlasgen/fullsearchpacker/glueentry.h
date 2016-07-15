/*! \file glueentry.h
    

    Defines an entry for gluing textures
 */
#pragma once
#include <QtCore/QVector>

class Texture;

namespace fullsearchpacker
{

class GlueOrder;


/*! An entry size for gluing
    first element is width,
    second is height.
 */
class GlueEntry
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
    /*! Merges all entries, according specified order
        \param[in] entries a list of entries to be merged
        \param[in] order   an order, in which they should be merged
     */
    static GlueEntry merge(
        const QVector<fullsearchpacker::GlueEntry>& entries,
        fullsearchpacker::GlueOrder& order
    );
    /*! Returns element by index
        \param[in] i index
        \return value
     */
    inline double& operator[](size_t i)
    {
        return m_a[i];
    }
    /*! Returns element by index
        \param[in] i index
        \return value
     */
    inline double operator[](size_t i) const
    {
        return m_a[i];
    }
private:
    /*! Array from two elements
     */
    double m_a[2];
};

}