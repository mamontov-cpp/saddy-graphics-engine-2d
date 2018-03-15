/*! \file clipboard.h
    
    A libclipboard-based  clipboard  API
 */
#pragma once
#include "sadstring.h"

namespace sad
{
    
/*! A clipboard stuff
 */
class Clipboard
{
public:
    /*! Makes new clipboard
     */
    Clipboard();
    /*! Does nothing, making new clipboard
        \param[in] o clipboard
     */
    Clipboard(const sad::Clipboard& o);
    /*! Does nothing, ignoring assignment semantics
        \param[in] o clipboard
     */
    sad::Clipboard& operator=(const sad::Clipboard& o);
    /*! Frees memory from clipboard
     */
    ~Clipboard();
    /*! Sets text in clipboard
        \param[in] text a text
     */
    void set(const sad::String& text);
    /*! Gets a clipboard text
        \return text
     */
    sad::String get() const;
    /*! A clear
     */
    void clear();
protected:
    /*! A clipboard opaque pointer
     */
    void* m_clipboard;
};

}
