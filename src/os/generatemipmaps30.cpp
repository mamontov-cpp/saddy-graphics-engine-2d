#include "os/generatemipmaps30.h"
#include "log/log.h"
#include "renderer.h"

static PFNGLGENERATEMIPMAPEXTPROC _generateMipMaps  = nullptr;

bool sad::os::generateMipMaps30(sad::Renderer * r, GLenum target)
{
    PFNGLGENERATEMIPMAPEXTPROC __generateMipMaps = _generateMipMaps;
    if (_generateMipMaps == nullptr)
    {
#ifdef WIN32
        __generateMipMaps = (PFNGLGENERATEMIPMAPEXTPROC)getProcAdress("glGenerateMipmap");
#endif
#ifdef LINUX
        __generateMipMaps = (PFNGLGENERATEMIPMAPEXTPROC)getProcAdress((const GLubyte*)("glGenerateMipmap"));
#endif
        _generateMipMaps = __generateMipMaps;
    }
    bool result = false;
    if (__generateMipMaps)
    {
        __generateMipMaps(target);
        result = true;
    }
    else
    {
        SL_COND_LOCAL_INTERNAL("Failed to obtain glGenerateMipmap", r);
    }
    return result;
}
