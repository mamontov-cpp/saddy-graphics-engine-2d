/*! \file events.h
    

    This file contains implementations for all window events, that are performed in engine.
    Events can contain useful data about cursor position, key part and other.
 */
#pragma once
#include "../keycodes.h"
#include "../sadpoint.h"
#include "../sadsize.h"
#include "../sadstring.h"
#include "../maybe.h"

namespace sad
{

namespace input
{

/*! Determines emitted event type
 */
enum class EventType: int
{
    /*! A type for quit event
     */
    ET_Quit       = 0,
    /*! A type for window event, when window is activated
     */
    ET_Activate         = 1,
    /*! A type for window event, when window is deactivated
     */
    ET_Deactivate       = 2,
    /*! A type for mouse enter event
     */
    ET_MouseEnter       = 3,
    /*! A type for mouse leave event
     */
    ET_MouseLeave       = 4,
    /*! A type for key press event
     */
    ET_KeyPress         = 5,
    /*! A type for key release event
     */
    ET_KeyRelease       = 6,
    /*! A type for mouse move event
     */
    ET_MouseMove        =  7,
    /*! A type for mouse click event
     */
    ET_MousePress       =  8,
    /*! A type for mouse release event
     */
    ET_MouseRelease     =  9,
    /*! A type for mouse release event
     */
    ET_MouseDoubleClick =  10,
    /*! A type for wheel event
     */
    ET_MouseWheel       =  11,
    /*! A type for resize event
     */
    ET_Resize           = 12,
    /*! A type for event when console window is closed. This is special case, so we need to perform event handled synchronously
     */
    ET_EmergencyShutdown = 13,
};

/*! A count of elements in EventType
 */
#define SAD_INPUT_EVENT_TYPE_COUNT  (14)

/*! An abstract event, which is base for all events of engine.
    Can be used for marking or run-time check of events
 */
class AbstractEvent
{
public:
    /*! Kept, for purpose of inheritance
     */
    virtual ~AbstractEvent();
};

/*! This type of events raised, when sad::Renderer exits sad::Renderer::run(), or
    when  mouse cursor leaves  window area, or on window activation, or
    on window deactivation
 */
class NullEvent: public sad::input::AbstractEvent
{
public:
    /*! Constructs new empty event
     */
    NullEvent();
    /*! Kept, for purpose of inheritance
     */
    virtual ~NullEvent() override;
};

/*! A type of event, which is emitted, when user quits application
 */
class QuitEvent: public sad::input::NullEvent
{
public:
    /*! Constructs new empty event
     */
    inline QuitEvent(): sad::input::NullEvent()
    {
    }
};

class EmergencyShutdownEvent: public sad::input::NullEvent
{
public:
    /*! Constructs new empty event
     */
    inline EmergencyShutdownEvent() : sad::input::NullEvent()
    {
    }
};

/*! A type of event, which is emitted when mouse cursor leaves window area
 */
class MouseLeaveEvent: public sad::input::NullEvent
{
public:
    /*! Constructs new empty event
     */
    inline MouseLeaveEvent(): sad::input::NullEvent()
    {
    }
};
/*! A type of event, which is emitted when window is activated 
 */
class ActivateEvent: public sad::input::NullEvent
{
public:
    /*! Constructs new empty event
     */
    inline ActivateEvent(): sad::input::NullEvent()
    {
    }
};
/*! A type of event, which is emitted when window is deactivated
 */
class DeactivateEvent: public sad::input::NullEvent
{
public:
    /*! Constructs new empty event
     */
    inline DeactivateEvent(): sad::input::NullEvent()
    {
    }
};

/*! This type of events raised, when user presses or releases key
 */
class KeyEvent: public sad::input::AbstractEvent
{
public:
    sad::KeyboardKey Key;   //!< Code of key, pressed by user
    sad::Maybe<sad::String> ReadableKey; // !< A readable part of key
    bool   AltHeld;		    //!< True, if user held ALT key, when event was created
    bool   ShiftHeld;	    //!< True, if user held SHIFT key, when event was created
    bool   CtrlHeld;	    //!< True, if user held CTRL key, when event was created
    
    /*! Creates new empty key event
     */
    inline KeyEvent() 
    : Key(sad::KeyboardKey::KeyNone), 
      AltHeld(false), 
      ShiftHeld(false), 
      CtrlHeld(false) 
    {
    
    }

    inline int key() const
    {
        return static_cast<int>(Key);
    }
    /*! Converts source key to Win-1251 encoding
     *  \return Win-1251 encoding
     */
    sad::Maybe<sad::String> toWin1251Key() const;
    /*! Kept, for purpose of inheritance
     */
    virtual ~KeyEvent();
};

/*! A type of event, which is emitted, when user presses keyboard button
 */
class KeyPressEvent: public sad::input::KeyEvent
{
public:
    /*! Constructs new empty event
     */
    inline KeyPressEvent(): sad::input::KeyEvent()
    {
    }
};

/*! A type of event, which is emitted, when user releases keyboard button
 */
class KeyReleaseEvent: public sad::input::KeyEvent
{
public:
    /*! Constructs new empty event
     */
    inline KeyReleaseEvent(): sad::input::KeyEvent()
    {
    }
};


/*! A basic event for all mouse events, like sad::input::MouseEvent,
    sad::input::WheelEvent
 */ 
class MouseCursorEvent: public sad::input::AbstractEvent
{
public:
    /*! A point in 3-dimensional space, for those, who don't use orthographic projections
     */
    sad::Point3D Point3D; 
    /*! A window client-space point
     */
    sad::Point2D Point;

    inline MouseCursorEvent()
    {

    }
    /*! Kept, for purpose of inheritance
     */	
    virtual ~MouseCursorEvent();
    /*! Sets a point for event. This method is used by renderer to set coordinates of
        cursor. 

        Also it fills a 2-dimensional point field automatically
        
        \param[in] p 3-dimensional point
     */
    inline void setPoint(const sad::Point3D & p)
    {
        this->Point3D = p;
    }
    /*! Returns position of cursor in 2D space
        \return position of cursor
     */
    inline const sad::Point2D & pos2D() const
    {
        return this->Point3D;
    }

};

/*! A type of event, which is emitted, when user moves a mouse
 */
class MouseMoveEvent: public sad::input::MouseCursorEvent
{
public:
    /*! Constructs new empty event
     */
    inline MouseMoveEvent(): sad::input::MouseCursorEvent()
    {
    }
};
/*! A type of event, which is emitted when mouse cursor enters a window 
 */
class MouseEnterEvent: public sad::input::MouseCursorEvent
{
public:
    /*! Constructs new empty event
     */
    inline MouseEnterEvent(): sad::input::MouseCursorEvent()
    {
    }
};

/*! This type of events raised, when user clicks a mouse, 
    presses a mouse button, releases a mouse button or performs a double click
 */
class MouseEvent: public sad::input::MouseCursorEvent
{
public:
    sad::MouseButton  Button;  //!< A button, which is pressed or held by user

    /*! Initializes event with no mouse pressed and zero points
     */
    inline MouseEvent() : MouseCursorEvent(), Button(sad::MouseButton::MouseNone) 
    {

    }

    inline int button() const
    {
        return static_cast<int>(Button);
    }
    /*! Kept, for purpose of inheritance
     */
    virtual ~MouseEvent();
};

/*! A type of event, which is emitted, when user clicks on a mouse button
 */
class MousePressEvent: public sad::input::MouseEvent
{
public:
    /*! Constructs new empty event
     */
    inline MousePressEvent(): sad::input::MouseEvent()
    {
    }
};

/*! A type of event, which is emitted, when user releases a mouse button
 */
class MouseReleaseEvent: public sad::input::MouseEvent
{
public:
    /*! Constructs new empty event
     */
    inline MouseReleaseEvent(): sad::input::MouseEvent()
    {
    }
};

/*! A type of event, which is emitted, when user performs a double click 
    with a mouse button
 */
class MouseDoubleClickEvent: public sad::input::MouseEvent
{
public:
    /*! Constructs new empty event
     */
    inline MouseDoubleClickEvent(): sad::input::MouseEvent()
    {
    }
};

/*! This type of events raised, when user moves a wheel
 */
class MouseWheelEvent: public sad::input::MouseCursorEvent
{
public:
    /*! How much wheel has scrolled. Bigger than zero, means, that wheel was scrolled up,
        Lesser than zero, means, that wheel was scrolled down
     */
    double Delta;  

    /*! Initializes wheel event with zero delta
     */
    inline MouseWheelEvent() : MouseCursorEvent(), Delta(0)
    {

    }
    /*! Kept, for purpose of inheritance
     */
    virtual ~MouseWheelEvent();
};

/*! Event, which is raised, when window is being resized, maximized or minimized 
 */
class ResizeEvent: public sad::input::AbstractEvent
{
public:
    sad::Size2I OldSize;     //!< An old size of window
    sad::Size2I NewSize;     //!< A new size of window

    /*! A Constructs new event
     */
	inline ResizeEvent() = default;
    /*! Kept, for purpose of inheritance
     */
    virtual ~ResizeEvent();
};


/*! Provides a helper for getting enum class value for specified event type: int
 */
template<
typename _EventType
>
struct EnumValueForEventType
{

};

#define SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(TYPE)  \
template< >                                         \
struct EnumValueForEventType< TYPE >                \
{                                                   \
    static const sad::input::EventType Type;        \
};

SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(sad::input::QuitEvent)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(sad::input::ActivateEvent)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(sad::input::DeactivateEvent)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(sad::input::KeyPressEvent)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(sad::input::KeyReleaseEvent)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(sad::input::MouseMoveEvent)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(sad::input::MouseEnterEvent)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(sad::input::MouseLeaveEvent)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(sad::input::MousePressEvent)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(sad::input::MouseReleaseEvent)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(sad::input::MouseDoubleClickEvent)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(sad::input::MouseWheelEvent)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(sad::input::ResizeEvent)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(sad::input::EmergencyShutdownEvent)

#undef SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE

}

}
