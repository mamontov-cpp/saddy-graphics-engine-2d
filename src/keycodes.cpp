#include "keycodes.h"

#include "sadstring.h"
#include "input/events.h"

bool sad::isValidKeyboardKey(int key)
{
    bool result = false;
    if (key >= '0' && key <= '9')
        result = true;
    if (key >= 'A' && key <= 'Z')
        result = true;
    if (key >= sad::Esc && key <= sad::RightAlt)
        result = true;
    return result;
}

sad::KeyboardKey sad::defaultKeyIfNotValid(sad::KeyboardKey key, sad::KeyboardKey d)
{
    if (sad::isValidKeyboardKey(key))
    {
        return key;
    }
    return d;
}

static sad::KeyboardKey basic_keys[11] = {
    sad::Minus,	 
    sad::Equal,	  
    sad::BackSlash, 
    sad::Tilde, 
    sad::OpeningSquareBracket,
    sad::ClosingSquareBracket,
    sad::Semicolon,
    sad::Comma,
    sad::Period,
    sad::Slash,
    sad::Apostrophe,
};

static char basic_keys_chars[11] = {
    '-',	 
    '=',	  
    '\\', 
    '~', 
    '[',
    ']',
    ';',
    ',',
    '.',
    '/',
    '\''
};

#define STRINGIFY(X)   #X

static const char* other_keys[] = {
    STRINGIFY(Esc)           ,  //!< An Escape key
    STRINGIFY(F1)            ,  //!< F1 key,
    STRINGIFY(F2)		     ,  //!< F2 key
    STRINGIFY(F3)		     ,  //!< F3 key
    STRINGIFY(F4)    	     ,  //!< F4 key
    STRINGIFY(F5)	         ,  //!< F5 key
    STRINGIFY(F6)		     ,  //!< F6 key
    STRINGIFY(F7)		     ,  //!< F7 key
    STRINGIFY(F8)   	     ,  //!< F8 key
    STRINGIFY(F9)            ,  //!< F9 key
    STRINGIFY(F10)           ,  //!< F10 key
    STRINGIFY(F11)           ,  //!< F11 key
    STRINGIFY(F12)           ,  //!< F12 key
    STRINGIFY(Pause)         ,  //!< A pause key
    STRINGIFY(Insert)        ,  //!< An insert key
    STRINGIFY(Delete)        ,  //!< A delete key
    STRINGIFY(Backspace)     ,  //!< A backspace key
    STRINGIFY(Enter)         ,  //!< Enter key
    STRINGIFY(Space)         ,  //!< A space key
    STRINGIFY(PrintScreen)   ,  //!< A print screen key
    STRINGIFY(Home)          ,  //!< A home key
    STRINGIFY(End)           ,  //!< An end key,
    STRINGIFY(PageUp)        ,  //!< A page up key
    STRINGIFY(PageDown)      ,  //!< A page down key
    STRINGIFY(Tab)           ,  //!< A tab key
    STRINGIFY(NumLock)       ,  //!< Num lock key
    STRINGIFY(ScrollLock)    ,  //!< Scroll lock key
    STRINGIFY(CapsLock)      ,  //!< Caps lock key
    STRINGIFY(WindowsKey)    ,  //!< A windows key
    STRINGIFY(AppKey)        ,  //!< A windows app key
    STRINGIFY(KeyLeft)       ,  //!< Left key
    STRINGIFY(KeyRight)      ,  //!< Right key
    STRINGIFY(KeyUp)         ,  //!< Up key
    STRINGIFY(KeyDown)       ,  //!< Down key
    STRINGIFY(LeftShift)     ,  //!< A left shift key
    STRINGIFY(RightShift)    ,  //!< A right shift key
    STRINGIFY(LeftControl)   ,  //!< A left control key
    STRINGIFY(RightControl)  ,  //!< A right control key
    STRINGIFY(LeftAlt)       ,  //!< A left alt key
    STRINGIFY(RightAlt)      ,  //!< A right alt key
    nullptr
};

#undef STRINGIFY

sad::String sad::keyToString(sad::KeyboardKey key)
{
    if (!sad::isValidKeyboardKey(key))
        return "Unknown";
    char buf[2] = {0, 0};
    if ((key >= '0' && key <= '9') ||( key >= 'A' && key <= 'Z'))
    {
        buf[0] = key;
        return sad::String(buf);
    }
    for(int i = 0; i < 11; i++)
    {
        if (basic_keys[i] == key)
        {
            buf[0] =  basic_keys_chars[i];
            return sad::String(buf);
        }
    }
    if (key >= sad::Esc && key <= sad::RightAlt) {
        return other_keys[ static_cast<int>(key) - static_cast<int>(sad::Esc)];
    }
    return "Unknown";
}

int sad::defaultKeyIfNotValidAsInt(int key, int d)
{
    return static_cast<sad::KeyboardKey>(sad::defaultKeyIfNotValid(static_cast<sad::KeyboardKey>(key), static_cast<sad::KeyboardKey>(d)));
}

sad::String sad::keyToStringAsInt(int key)
{
    return sad::keyToString(static_cast<sad::KeyboardKey>(key));
}