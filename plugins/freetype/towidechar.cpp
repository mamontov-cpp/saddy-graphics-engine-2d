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
	if (c == '¸')              
	{ 
		result = 0x0451; 
	}
	if (c == '¨')              
	{ 
		result = 0x0401; 
	}
	if (c >= 'À' && c <= 'ß')   
	{ 
		result = 0x0410 + (c - 'À'); 
	}
	if (c >= 'à' && c <= 'ÿ')   
	{ 
		result = 0x0430 + (c - 'à'); 
	}
	return result;
}
