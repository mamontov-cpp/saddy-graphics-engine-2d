#include "sadsleep.h"

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void sad::sleep(unsigned int milliseconds)
{
#ifdef WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds);
#endif

}

