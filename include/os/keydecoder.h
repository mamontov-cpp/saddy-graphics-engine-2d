/*! \file keydecoder.h
    

    Defines a decoder for converting key symbols from OS to Saddy key symbols.
 */
#pragma once
#include "os/windowhandles.h"
#include "../sadhash.h"
#include "../keycodes.h"
#include "../maybe.h"
#include "../sadstring.h"

namespace sad
{

class Window;

namespace os
{

class SystemWindowEvent;

#ifdef WIN32
/*! A key, received from OS
 */
typedef WPARAM  SystemKey;

#endif

#ifdef X11
/*! A key, received from OS
 */
typedef ::KeySym SystemKey;

#endif

/*! Converts key symbols from OS to Saddy key symbols
 */ 
class KeyDecoder
{
public:
    /*! Creates new key decoder
     */
    KeyDecoder();
    /*! Decodes a system key, converting it from common key to
        current
        \param[in] e a system window event
        \return decoded key from saddy
     */
    sad::KeyboardKey decode(sad::os::SystemWindowEvent * e);
    /*! Converts system key to character string
        \param[in] e system window event
        \param[in] win a window, where other parameters should come from
        \return converted key
     */
    sad::Maybe<sad::String> convert(sad::os::SystemWindowEvent * e, sad::Window * win);
protected:
    /*! Fills table with mappings
     */
    void init();
    /*! Test, whether keyboard key is readable
     */
    bool isReadable(sad::KeyboardKey key);
#ifdef WIN32
    /*! A key state for decoding keys
     */
    unsigned char m_key_states[256];
#endif

#ifdef X11
    /*! A key symbol
     */
    ::KeySym m_key_sym;
    /*! A compose status
     */
    ::XComposeStatus m_compose_status;
#endif
    /*! A recoding table for decoder
     */
    sad::Hash<sad::os::SystemKey, sad::KeyboardKey> m_table;
};

}

}
