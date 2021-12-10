#include "openinbrowser.h"

#include <cstdlib>
#include <string>

int sad::openInBrowser(const char* url)
{
    if (!url)
    {
        return 0;
    }
#ifdef WIN32
    std::string data("start \"\" \"");
    data += url;
    data += "\"";
    return std::system(data.c_str());
#else
    std::string data("xdg-open \"");
    data += url;
    data += "\" &";
    return std::system(data.c_str());
#endif
}