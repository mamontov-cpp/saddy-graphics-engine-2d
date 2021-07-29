#include "input/events.h"
#include "db/dbtypename.h"



sad::input::AbstractEvent::~AbstractEvent()
{

}

sad::input::NullEvent::NullEvent()
{

}

sad::input::NullEvent::~NullEvent()
{

}

sad::input::KeyEvent::~KeyEvent()
{

}

sad::input::MouseCursorEvent::~MouseCursorEvent()
{

}

sad::input::MouseEvent::~MouseEvent()
{

}

sad::input::MouseWheelEvent::~MouseWheelEvent()
{

}

sad::input::ResizeEvent::~ResizeEvent()
{

}



#define SAD_DECLARE_ENUM_VALUE_FOR_EVENTTYPE(TYPE)             \
const sad::input::EventType sad::input::EnumValueForEventType< \
    sad::input:: TYPE ## Event >::Type                         \
    =  sad::input::EventType :: ET_ ## TYPE;

SAD_DECLARE_ENUM_VALUE_FOR_EVENTTYPE(Quit)
SAD_DECLARE_ENUM_VALUE_FOR_EVENTTYPE(Activate)
SAD_DECLARE_ENUM_VALUE_FOR_EVENTTYPE(Deactivate)
SAD_DECLARE_ENUM_VALUE_FOR_EVENTTYPE(KeyPress)
SAD_DECLARE_ENUM_VALUE_FOR_EVENTTYPE(KeyRelease)
SAD_DECLARE_ENUM_VALUE_FOR_EVENTTYPE(MouseMove)
SAD_DECLARE_ENUM_VALUE_FOR_EVENTTYPE(MouseEnter)
SAD_DECLARE_ENUM_VALUE_FOR_EVENTTYPE(MouseLeave)
SAD_DECLARE_ENUM_VALUE_FOR_EVENTTYPE(MousePress)
SAD_DECLARE_ENUM_VALUE_FOR_EVENTTYPE(MouseRelease)
SAD_DECLARE_ENUM_VALUE_FOR_EVENTTYPE(MouseDoubleClick)
SAD_DECLARE_ENUM_VALUE_FOR_EVENTTYPE(MouseWheel)
SAD_DECLARE_ENUM_VALUE_FOR_EVENTTYPE(Resize)



DECLARE_COMMON_TYPE(sad::input::QuitEvent)
DECLARE_COMMON_TYPE(sad::input::ActivateEvent)
DECLARE_COMMON_TYPE(sad::input::DeactivateEvent)
DECLARE_COMMON_TYPE(sad::input::KeyPressEvent)
DECLARE_COMMON_TYPE(sad::input::KeyReleaseEvent)
DECLARE_COMMON_TYPE(sad::input::MouseMoveEvent)
DECLARE_COMMON_TYPE(sad::input::MouseEnterEvent)
DECLARE_COMMON_TYPE(sad::input::MouseLeaveEvent)
DECLARE_COMMON_TYPE(sad::input::MousePressEvent)
DECLARE_COMMON_TYPE(sad::input::MouseReleaseEvent)
DECLARE_COMMON_TYPE(sad::input::MouseDoubleClickEvent)
DECLARE_COMMON_TYPE(sad::input::MouseWheelEvent)
DECLARE_COMMON_TYPE(sad::input::ResizeEvent)
