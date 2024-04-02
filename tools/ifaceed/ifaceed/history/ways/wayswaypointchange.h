/*! \file wayswaypointchange.h
 *  
 *
 * Describes a simple command for changing a waypoint
 */
#pragma once
#include "../command.h"
#include <way.h>

namespace history
{

namespace ways
{

/*! \class history::ways::WayPointChange
 *   A simple command for adding a new way to list
 */
class WayPointChange: public history::Command
{
public:
    /*! Creates new command
        \param[in] w way
        \param[in] pos a position of point in way list
        \param[in] old_value old value for point
        \param[in] new_value new value for point
     */
    WayPointChange(
        sad::Way* w, 
        int pos, 
        const sad::Point2D& old_value, 
        const sad::Point2D& new_value
    );
    /*! Could be inherited
     */
    virtual ~WayPointChange();
    /*! Sets new value for point
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Sets old value for point
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! A current selected way
     */
    sad::Way* m_way;
    /*! A position for a point in way point list
     */
    int m_position;
    /*! An old value for point
     */
    sad::Point2D m_old_point;
    /*! A new value for point
     */
    sad::Point2D m_new_point;
    /*! Tries to update UI
        \param[in] ob editor
        \param[in] p a point
     */
    void tryUpdateUI(core::Editor* ob, const sad::Point2D& p);
};

}

}
