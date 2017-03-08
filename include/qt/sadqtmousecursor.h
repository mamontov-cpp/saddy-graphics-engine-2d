/*! \file sadqtmousecursor.h
  

    Defines a mouse cursor, which can work with Qt
 */
#pragma once
#include "mousecursor.h"

namespace sad
{
	
namespace qt
{
	
/*! A mouse cursor, that can work with Qt
 */
class MouseCursor: public sad::MouseCursor
{
public:
	/*! Creates new cursor
	 */
	MouseCursor();
	/*! Can be inherited
	 */
	virtual ~MouseCursor();
	/*! Returns a position of cursor
	    \return current position of cursor if it's inside of window
	*/
	virtual sad::MaybePoint3D  position() const;
	/*! Sets a position for a cursor
	    \param[in] p coordinates for cursor in window client's area coordinates
	*/
	virtual void setPosition(const sad::Point2D & p);
protected:
	/*! Shows default cursor
	 */
	virtual void showDefaultCursor();
	/*! Hides default cursor
	 */
	virtual void hideDefaultCursor();
};


}

}

