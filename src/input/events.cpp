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



#define SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(TYPE)             \
const sad::input::EventType sad::input::EnumValueForEventType< \
    sad::input:: TYPE ## Event >::Type                         \
    =  sad::input::EventType :: ET_ ## TYPE;

SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(Quit)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(Activate)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(Deactivate)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(KeyPress)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(KeyRelease)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(MouseMove)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(MouseEnter)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(MouseLeave)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(MousePress)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(MouseRelease)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(MouseDoubleClick)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(MouseWheel)
SAD_DECLARE_ENUM_VALUE_FOR_EVENT_TYPE(Resize)



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
