/*! \file point3d.h
    

    A sad::Point3D scriptable wrapper
 */
#pragma once
#include "classwrapper.h"

#include <sadpoint.h>

namespace scripting
{

/*! A sad::Point3D scriptable wrapper
 */
class Point3D: public scripting::ClassWrapper
{
 Q_OBJECT
 Q_PROPERTY(double x READ x WRITE setX)
 Q_PROPERTY(double y READ y WRITE setY)
 Q_PROPERTY(double z READ z WRITE setZ)
public:
    /*! Constructs new wrapper
     */
    Point3D();
    /*! Constructs new wrapper
        \param[in] p point
     */
    Point3D(const sad::Point3D& p);
    /*! Could be inherited
     */
    virtual ~Point3D();
    /*! Converts a point to original point
     */
    const sad::Point3D& toPoint() const;
    /*! Returns x coordinate
        \return x coordinate
     */
    double x() const;
    /*! Returns y coordinate
        \return y coordinate
     */
    double y() const;
    /*! Returns z coordinate
        \return z coordinate
     */
    double z() const;
    /*! Sets x coordinate
        \param[in] x x coordinate
     */ 
    void setX(double x);
    /*! Sets y coordinate
        \param[in] y y coordinate
     */
    void setY(double y);
    /*! Sets z coordinate
        \param[in] z z coordinate
     */
    void setZ(double z);
    /*! Contains to string
        \return object to string
     */
    QString toString() const;
protected:
    /*! An inner point wrapper
     */
    sad::Point3D m_point;
};

}
