/*! \file waysbindings.h
    

    A bindings for ways are listed here
 */
#pragma once
#include <sadstring.h>
#include <sadrect.h>

#include <QScriptEngine>
#include <QVector>

#include <p2d/app/way.h>

namespace scripting
{

class Scripting;

namespace ways
{

/*! Lists all ways
    \param[in] ctx context
    \param[in] engine an engine
    \return a ways list
 */
QScriptValue list(
    QScriptContext* ctx,
    QScriptEngine* engine
);

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
    sad::Vector<sad::p2d::app::WayPoint> points
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
QScriptValue point(scripting::Scripting* scripting, sad::p2d::app::Way* way, unsigned int pos);

}

}
