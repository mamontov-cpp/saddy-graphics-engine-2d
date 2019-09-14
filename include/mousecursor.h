/*! \file   mousecursor.h
    

    Defines a mouse cursor and methods for working with it, like getting
    position, hiding it, showing sprite instead of it.
 */
#pragma once
#include "sadpoint.h"
#include "renderer.h"
#include "refcountable.h"
#include "input/handlers.h"
#include "input/events.h"


namespace sad
{
class Sprite2D;

/*! Defines a basic class, that can be used for image for cursor
 */
class MouseCursorImage
{
public:
    /*! Sets a position for cursor image.
        \param[in] p point 
     */
    virtual void setPos(const sad::Point2D & p) = 0;
    /*! There inherited class should render image at center, defined by setPos call
     *  \param[in] r renderer
     */
    virtual void render(sad::Renderer* r) = 0;
    /*! This is abstract class, that should be implemented by user
     */
    virtual ~MouseCursorImage();
};


/*! Defines a sprite as a mouse cursor. Note that sprite will be destroyed with destruction
    of object
 */
class MouseCursorSprite: public MouseCursorImage
{
public:
    /*! Creates a new sprite
        \param[in] a sprite adapter
     */
    MouseCursorSprite(Sprite2D * a);
    /*! Sets a position for cursor image.
        \param[in] p point 
     */
    virtual void setPos(const sad::Point2D & p);
    /*! There inherited class should render image at center, defined by setPos call
     *  \param[in] r renderer
     */
    virtual void render(sad::Renderer* r);
    /*! This is abstract class, that should be implemented by user
     */
    virtual ~MouseCursorSprite();
protected:
    /*! A sprite, which should be rendered
     */
    sad::Sprite2D * m_a;
};

/*! A mouse cursor is responsible for handling getting a position of cursor or showing sprite, 
    if needed
 */
class MouseCursor: public RefCountable
{
public:
    friend class sad::Renderer;
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
    /*! Shows mouse cursor in client area
     */
    virtual void show();
    /*! Hides mouse cursor in client area
     */
    virtual void hide();
    /*! Returns current cursor image
        \return cursor image
     */
    virtual sad::MouseCursorImage * cursorImage() const;
    /*! Sets cursor image
        \param[in] image
     */
    virtual void setImage(sad::MouseCursorImage * image);
    /*! Sets image as 2D sprite
     */
    void setImage(Sprite2D * a);
    /*! Switches cursor image into default cursor image
     */
    void clearCursorImage();
    /*! Inserts handlers for handling sprite mouse cursor if needed to 
     */
    virtual void insertHandlersIfNeeded();
    /*! Removes handlers for handling sprite mouse cursor if needed to
     */
    virtual void removeHandlersIfNeeded();
protected:
    /*! Applies required cursor transfomration for rendering 
        custom cursor
     */
    virtual void applyCursorTransformations();
    /*! Shows default cursor
     */
    virtual void showDefaultCursor();	
    /*! Hides default cursor
     */
    virtual void hideDefaultCursor();
    /*! Renders cursor if need custom cursor flag is set and cursor is hidden
     */
    virtual void renderCursorIfNeedTo();
    /*! Shows custom cursor if need to
        \param[in] e event
     */
    virtual void showCustomCursorIfNeedTo(const sad::input::MouseEnterEvent & e);
    /*! Hides custom cursor if need to
     */ 
    virtual void hideCustomCursorIfNeedTo();
    /*! Moves custom cursor if need to
        \param[in] e event of mouse movement
     */
    virtual void moveCustomCursor(const sad::input::MouseMoveEvent & e);
    /*! Whether we should show custom cursor when we are entering here
     */
    bool       m_usecustomcursor;
    /*! Whether we should hide custom cursor
     */ 
    bool       m_hidecustomcursor;
    /*! A flag, determines whether mouse cursor is hidden
     */
    bool       m_hidden;
    /*! A cursor image, which should be shown at position
     */
    MouseCursorImage * m_cursor;
    /*! A renderer, attached to cursor
     */
    sad::Renderer * m_renderer;
    /*! A handler, which hides cursor when it enters windows's client area
     */ 
    sad::input::AbstractHandler * m_enter_handler;
    /*! A handler, which shows cursor when it leaves windows's client area
     */ 
    sad::input::AbstractHandler * m_leave_handler;
    /*! A handler, which moves sprite accordingly
     */
    sad::input::AbstractHandler * m_move_handler;
};

}
