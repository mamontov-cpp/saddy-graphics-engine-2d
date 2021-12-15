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

sad::Maybe<sad::String> sad::input::KeyEvent::toWin1251Key() const
{
    if (!ReadableKey.exists())
    {
        return sad::Maybe<sad::String>();
    }
    sad::String key = ReadableKey.value();
    if (key.length() == 2)
    {
        char num[4] = { key[1], key[0], 0, 0 };
        const int d = *reinterpret_cast<int*>(num);
        char kv[2] = { 0x0, 0x0 };
        unsigned char* pkv = reinterpret_cast<unsigned char*>(&kv[0]);
        if (d == 0xD191)
        {
            *pkv = 0xB8;
            key = kv;
        }
        if (d == 0xD081)
        {
            *pkv = 0xA8;
            key = kv;
        }
        if (d >= 0xD090 && d <= 0xD09F)
        {
            *pkv = 0xC0 + static_cast<int>(d - 0xD090);  // NOLINT(clang-diagnostic-implicit-int-conversion)
            key = kv;
        }
        if (d >= 0xD0A0 && d <= 0xD0AF)
        {
            *pkv = 0xD0 + static_cast<int>(d - 0xD0A0);  // NOLINT(clang-diagnostic-implicit-int-conversion)
            key = kv;
        }
        if (d >= 0xD0B0 && d <= 0xD0BF)
        {
            *pkv = 0xE0 + static_cast<int>(d - 0xD0B0); // NOLINT(clang-diagnostic-implicit-int-conversion)
            key = kv;
        }
        if (d >= 0xD180 && d <= 0xD18F)
        {
            *pkv = 0xF0 + static_cast<int>(d - 0xD180); // NOLINT(clang-diagnostic-implicit-int-conversion)
            key = kv;
        }
    }
    return sad::Maybe<sad::String>(key);
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
