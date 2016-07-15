/*! \file rect2d.h
    

    A sad::Rect2D scriptable wrapper
 */
#pragma once
#include "classwrapper.h"

#include <sadrect.h>

#include "point2d.h"

#include <QScriptValue>

namespace scripting
{

/*! A sad::Point2D scriptable wrapper
 */
class Rect2D: public scripting::ClassWrapper
{
 Q_OBJECT
public:
    /*! Constructs new wrapper
     */
    Rect2D();
    /*! Constructs new wrapper
        \param[in] p point
     */
    Rect2D(const sad::Rect2D& p);
    /*! Could be inherited
     */
    virtual ~Rect2D();
    /*! Converts a point to original point
     */
    const sad::Rect2D& toRect() const;
    /*! Contains to string
        \return object to string
     */
    QString toString() const;
public slots:  
    /*! Sets point for rectangle
        \param[in] i index
        \param[in] p point
     */
    void setPoint(int i, scripting::Point2D* p);
    /*! Returns point from rectangle
        \param[in] i index
        \return point
     */
    QScriptValue point(int i) const;
    /*! Returns copy rectangle with center at specified point
        \param[in] p point
        \return rectangle
     */
    QScriptValue movedToPoint(scripting::Point2D* p);
protected:
    /*! An inner rect wrapper
     */
    sad::Rect2D m_rect;
};

}
