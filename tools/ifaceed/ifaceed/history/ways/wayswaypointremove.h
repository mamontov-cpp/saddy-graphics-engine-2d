/*! \file wayswaypointremove.h
 *  
 *
 * Describes a simple command for removing a way point
 */
#pragma once
#include "../command.h"

#include <way.h>

namespace history
{

namespace ways
{

/*! \class history::ways::WayPointRemove
 *   A simple command for removing a way
 */
class WayPointRemove: public history::Command
{
public:
    /*! Creates new command
        \param[in] w way
        \param[in] position a position of point
     */
    WayPointRemove(sad::Way* w, int position);
    /*! Could be inherited
     */
    virtual ~WayPointRemove();
    /*! Removes a way point
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Adds a way point
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! A current selected way
     */
    sad::Way* m_way;
    /*! A stored point
     */
    sad::Point2D m_point;
    /*! An index of point in way
     */
    int m_position;
};

}

}
