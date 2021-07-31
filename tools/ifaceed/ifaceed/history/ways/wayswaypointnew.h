/*! \file wayswaypointnew.h
 *  
 *
 * Describes a simple command for adding a way point to way
 */
#pragma once
#include "../command.h"
#include <p2d/app/way.h>

namespace history
{

namespace ways
{

/*! \class history::ways::WayPointNew
 *   A simple command for adding a new way point to way
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
    virtual ~WayPointNew() override;
    /*! Adds a way point
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Removes way point
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
    /*! Sets point for new point
        \param[in] point a point
     */
    void setPoint(const sad::Point2D& point);
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
