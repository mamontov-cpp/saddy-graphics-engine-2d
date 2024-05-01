#include "openinbrowser.h"

#include <cstdlib>
#include <string>
#include "opticksupport.h"

int sad::openInBrowser(const char* url)
{
    PROFILER_EVENT;
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
