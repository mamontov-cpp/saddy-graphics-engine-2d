/*! \file towidechar.h
    

    A definition of simple convertor from ASCII  to Unicode, which
    responds to russian letters
 */
#pragma once

namespace sad
{

namespace freetype
{

/*! Converts char to wide char, responding to russian letters.
    This needed for Freetype to properly render russian letters
    \param[in] c character to be converted
    \return wide character
 */
wchar_t to_wide_char(unsigned char c);

}

}
