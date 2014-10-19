/*! \file wayswaypointchange.h
 *  \author HiddenSeeker
 *
 * Describes a simple command for changing a waypoint
 */
#pragma once
#include "../command.h"
#include <p2d/app/way.h>

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
		\param[in] oldv old value for point
		\param[in] newv new value for point
     */
	WayPointChange(
		sad::p2d::app::Way* w, 
		int pos, 
		const sad::Point2D& oldv, 
		const sad::Point2D& newv
	);
    /*! Could be inherited
     */
    virtual ~WayPointChange();
    /*! Sets new value for point
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = NULL);
    /*! Sets old value for point
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = NULL);
protected:
    /*! A current selected way
     */
    sad::p2d::app::Way* m_way;
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
