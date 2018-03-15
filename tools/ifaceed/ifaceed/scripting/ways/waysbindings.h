/*! \file waysbindings.h
    

    A bindings for ways are listed here
 */
#pragma once
#include "../dukqtcontext.h"
#include <sadstring.h>

#include <QVector>

#include <p2d/app/way.h>
#include "wayspointref.h"
#include <3rdparty/dukpp-03/include/maybe.h>

namespace scripting
{

class Scripting;

namespace ways
{

/*! Returns major ids list for all ways, stored in database
    \return major ids list for all ways
 */
QVector<unsigned long long> list();

/*! Adds way. Prefixed by underscore, since it will be mapped to _add function and add
    is reserved for call, which will take object, preprocess it's fields and call _add using fields of this object.
    \param[in] scripting a scripting part
    \param[in] name a name for way
    \param[in] totaltime a total time for way
    \param[in] closed whether way is closed
    \param[in] points a list of points
 */ 
unsigned long long _add(
    scripting::Scripting* scripting, 
    sad::String name,
    double totaltime,
    bool closed,
    QVector<sad::Point2D> points
);

/*! Removes a way
    \param[in] scripting a scripting part
    \param[in] way a way to be removed
 */
void remove(scripting::Scripting* scripting, sad::p2d::app::Way* way);

/*! Returns length of way in points
    \param[in] scripting a scripting part
    \param[in] way a way to be removed
 */
unsigned int length(scripting::Scripting* scripting, sad::p2d::app::Way* way);

/*! Adds point to a way
    \param[in] scripting a scripting part
    \param[in] way a way to be removed
    \param[in] point a point to be added
 */
void addPoint(scripting::Scripting* scripting, sad::p2d::app::Way* way, sad::Point2D point);

/*! Removes point from way
    \param[in] scripting a scripting part
    \param[in] way a way to be removed
    \param[in] pos a position
 */
bool removePoint(scripting::Scripting* scripting, sad::p2d::app::Way* way, unsigned int pos);

/*! Fetches point reference from way
    \param[in] scripting a scripting part
    \param[in] way a way to be removed
    \param[in] pos a position
 */
dukpp03::Maybe<scripting::ways::PointRef*> point(scripting::Scripting* scripting, sad::p2d::app::Way* way, unsigned int pos);

/*! Initializes bindings for ways
    \param[in] s scripting object
    \param[in] e a global binding object
    \return created ways object
 */
dukpp03::qt::JSObject* init(scripting::Scripting* s, dukpp03::qt::JSObject* e);

}

}
