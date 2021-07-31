/*! \file keytovector.h

    Describes a conversion from key to vector
 */
#pragma once
#include <sadpoint.h>
#include <input/events.h>

inline sad::Point2D keyToVector(sad::KeyboardKey key)
{
    sad::Point2D p(0, 0);
    switch (key)
    {
        case sad::KeyboardKey::KeyLeft:  {p.setX(-1); break;}
        case sad::KeyboardKey::KeyRight:  {p.setX(1); break;}
        case sad::KeyboardKey::KeyUp:  {p.setY(1); break;}
        case sad::KeyboardKey::KeyDown:  {p.setY(-1); break;}
        default: break;
    };
    return p;
}
