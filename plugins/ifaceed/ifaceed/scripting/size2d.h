/*! \file size2d.h
    

    A sad::Size2D scriptable wrapper
 */
#pragma once
#include "classwrapper.h"

#include <sadsize.h>

namespace scripting
{

/*! A sad::Size2D scriptable wrapper
 */
class Size2D: public scripting::ClassWrapper
{
 Q_OBJECT
 Q_PROPERTY(double width READ width WRITE setWidth)
 Q_PROPERTY(double height READ height WRITE setHeight)
public:
    /*! Constructs new wrapper
     */
    Size2D();
    /*! Constructs new wrapper
        \param[in] p point
     */
    Size2D(const sad::Size2D& p);
    /*! Could be inherited
     */
    virtual ~Size2D();
    /*! Converts a point to original point
     */
    const sad::Size2D& toSize() const;
    /*! Returns width
        \return width
     */
    double width() const;
    /*! Returns height
        \return height
     */
    double height() const;
    /*! Sets width
        \param[in] width width
     */ 
    void setWidth(double width);
    /*! Sets height
        \param[in] height height
     */
    void setHeight(double height);
    /*! Contains to string
        \return object to string
     */
    QString toString() const;
protected:
    /*! An inner point wrapper
     */
    sad::Size2D m_size;
};

}
