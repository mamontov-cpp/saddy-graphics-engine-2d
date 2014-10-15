/*! \file wayswaypointnew.h
 *  \author HiddenSeeker
 *
 * Describes a simple command for adding a waypoint
 */
#pragma once
#include "../command.h"
#include <p2d/app/way.h>

namespace history
{

namespace ways
{

/*! \class history::ways::New
 *   A simple command for adding a new way to list
 */
class WayPointNew: public history::Command
{
public:
    /*! Creates new command
        \param[in] w way
     */
    WayPointNew(sad::p2d::app::Way* w);
    /*! Could be inherited
     */
    virtual ~WayPointNew();
    /*! Adds a way point
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = NULL);
    /*! Removes way point
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = NULL);
protected:
    /*! A current selected way
     */
    sad::p2d::app::Way* m_way;
    /*! An added point
     */
    sad::Point2D m_point;
};

}

}
