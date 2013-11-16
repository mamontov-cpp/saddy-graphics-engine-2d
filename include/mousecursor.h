/*! \file   mousecursor.h
	\author HiddenSeeker

	Defines a mouse cursor and methods for working with it, like getting
	position, hiding it, showing sprite instead of it.
 */
#pragma once
#include "sadpoint.h"
#include "renderer.h"

namespace sad
{

class Sprite2D;
class Renderer;
/*! A mouse cursor is responsible for hetting a position or showing sprite, 
	if needed
 */
class MouseCursor
{
public:
	/*! Creates default mouse cursor
	 */
	MouseCursor();
	/*! Removes cursor image, if associated with any
	 */  
	virtual ~MouseCursor();
	/*! Sets a renderer, associated with cursor
		\param[in] r renderer
	 */
	virtual void setRenderer(sad::Renderer * r);
	/*! Returns a renderer, associated with cursor
		\return renderer, associated with cursor
	 */ 
	virtual sad::Renderer * renderer() const;
	/*! Returns a position of cursor
		\return current position of cursor if it's inside of window
	 */
	virtual sad::MaybePoint3D  position() const;
	/*! Sets a position for a cursor
		\param[in] p coordinates for cursor in window client's area coordinates
	 */
	virtual void setPosition(const sad::Point2D & p);
protected:
	/*! Whether we should show custom cursor when we are entering here
	 */
	bool       m_showcustomcursor;
	/*! A cursor image, which should be shown at position
	 */
	Sprite2D * m_cursor;
	/*! A renderer, attached to cursor
	 */
	sad::Renderer * m_renderer;
};


}
