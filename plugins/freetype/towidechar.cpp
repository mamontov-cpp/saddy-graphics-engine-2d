#include "towidechar.h"
#include "sadwstring.h"

wchar_t sad::freetype::to_wide_char(unsigned char c)
{
    unsigned char mb[2] = {c, 0};
    wchar_t wc[2] = {c, 0};
    if (c > 127)  
    { 
        mbtowc(wc,(const char *)mb,2); 
    }
    wchar_t result = wc[0]; 
    // Convert russian locale letters
    if (c == 0xB8)
    { 
        result = 0x0451;
    }
    if (c == 0xA8)
    { 
        result = 0x0401;
    }
    if (c >= 0xC0 && c <= 0xDF)   
    { 
        result = 0x0410 + (c - 0xC0);
    }
    if (c >= 0xE0 )
    { 
        result = 0x0430 + (c - 0xE0);
    }
    return result;
}
