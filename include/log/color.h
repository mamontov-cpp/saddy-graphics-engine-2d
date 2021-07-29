/*! \file color.h
    

    Defines a color, used in coloring schemes for console targets
 */
#pragma once

namespace sad
{

namespace log
{

/*! Defines a color, for color scheme, which text will be colored, when log message
    is represented on screen
 */
enum class Color: int
{
    NONE =  0,
    LIGHT_RED = 1,
    LIGHT_GREEN = 2,
    LIGHT_BLUE = 3,
    DARK_RED = 4,
    DARK_GREEN = 5,
    DARK_BLUE = 6,
    WHITE = 7,
    LIGHT_GREY = 8,
    DARK_GREY = 9,
    BLACK = 10,
    LIGHT_YELLOW = 11,
    DARK_YELLOW = 12,
    LIGHT_MAGENTA = 13,
    DARK_MAGENTA = 14,
    LIGHT_CYAN = 15,
    DARK_CYAN = 16
};

}

}
