/*! \file wayspointref.h
    

    A wrapper for reference to way point
 */
#pragma once

#include <QObject>
#include <QMetaType>

#include <sadpoint.h>

#include <p2d/app/way.h>

namespace scripting
{

class Scripting;

namespace ways
{

/*! A wrapper to point reference
 */
class PointRef: public QObject
{
 Q_OBJECT
 Q_PROPERTY(double x READ x WRITE setX)
 Q_PROPERTY(double y READ y WRITE setY)
 Q_PROPERTY(unsigned int position READ position)
public:
    /*! Constructs new wrapper
     */
    PointRef();
    /*! Constructs new wrapper
        \param[in] s scripting
        \param[in] way a way, whose point is referenced
        \param[in] pos a position
     */
    PointRef(scripting::Scripting* s, sad::p2d::app::Way* way, unsigned int pos);
    /*! Could be inherited
     */
    virtual ~PointRef();
    /*! Tests, whether point ref is valid
        \return whether is valid
     */
    bool valid() const;
    /*! Converts a point to original point
     */
    const sad::Point2D& toPoint() const;
    /*! Returns x coordinate
        \return x coordinate
     */
    double x() const;
    /*! Returns y coordinate
        \return y coordinate
     */
    double y() const;
    /*! Sets x coordinate
        \param[in] x x coordinate
     */ 
    void setX(double x);
    /*! Sets y coordinate
        \param[in] y y coordinate
     */
    void setY(double y);
    /*! Returns a position to point ref
        \return position
     */
    unsigned int position() const;
public slots:
    /*! Converts object to string representation
        \return object to string
    */
    QString toString() const;
    /*! Moves point back in list
     */
    void moveBack();
    /*! Moves point front in list
     */
    void moveFront();
protected:
    /*! A scripting part for throwing exception
     */
    scripting::Scripting* m_scripting;
    /*! An inner point wrapper
     */
    sad::p2d::app::Way* m_way;
    /*! A position of reference in list
     */
    unsigned int m_pos;
};

}

}


Q_DECLARE_METATYPE(scripting::ways::PointRef**)
