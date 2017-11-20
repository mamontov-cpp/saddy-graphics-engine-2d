#include "clipboard.h"
#include "../3rdparty/libclipboard/include/libclipboard.h"

#define VALUE (reinterpret_cast<clipboard_c*>(m_clipboard))

sad::Clipboard::Clipboard()
{
    m_clipboard = clipboard_new(NULL);
}

sad::Clipboard::Clipboard(const sad::Clipboard& o)
{
    m_clipboard = clipboard_new(NULL);
}


sad::Clipboard& sad::Clipboard::operator=(const sad::Clipboard& o)
{
    m_clipboard = clipboard_new(NULL);
    return *this;
}

sad::Clipboard::~Clipboard()
{
    clipboard_free(VALUE);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void sad::Clipboard::set(const sad::String& text)
{
    if (text.length())
    {
        clipboard_set_text(VALUE, text.c_str());
    }
    else
    {
        char t[2] = "";
        clipboard_set_text(VALUE, t);
    }
}

sad::String sad::Clipboard::get() const
{
    char* text = clipboard_text(VALUE);
    if (text)
    {
        sad::String result(text);
        free(text);
        return result;
    }
    return sad::String();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void sad::Clipboard::clear()
{
    clipboard_clear(VALUE, LCB_CLIPBOARD);
}

#include "../3rdparty/libclipboard/src/clipboard_common.c"
#include "../3rdparty/libclipboard/src/clipboard_x11.c"
#include "../3rdparty/libclipboard/src/clipboard_win32.c"
