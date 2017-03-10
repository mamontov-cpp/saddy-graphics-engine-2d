/*! \file sadqtkeytosadkey.h
    
    Declares function for converting Qt key to saddy key
 */
#pragma once
#include <QKeyEvent>
#include <input/events.h>

namespace sad
{

namespace qt
{

/*! Converts qt key to saddy key
    \param[in] key a key
    \param[in] nativeScanCode a native scan code for key
    \return a value for key
 */
sad::KeyboardKey qtKeyToSadKey(int key, quint32 nativeScanCode);

/*! Converts qt key event to saddy key event
    \param[in] ev event
    \param[out] sev Saddy's native event
 */
void qtKeyEventToSadKeyEvent(QKeyEvent* ev, sad::input::KeyEvent* sev);

}

}
