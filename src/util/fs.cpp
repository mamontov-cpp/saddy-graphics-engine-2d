#include "util/fs.h"

sad::String util::pathDelimiter()
{
#ifdef WIN32
	return "\\";
#endif

#ifdef LINUX
	return "/";
#endif
}
