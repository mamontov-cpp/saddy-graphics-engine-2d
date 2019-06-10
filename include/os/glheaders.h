/*! \file glheaders.h
    

    A summarizing header, which contains all of needed OpenGL headers
 */
#pragma once

#ifdef WIN32
    #ifndef NOMINMAX
    #define NOMINMAX 
    #endif
    #include <windows.h>
    #include <gl/gl.h>
    #include <gl/glu.h>
#endif

#ifdef X11
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glx.h>
#endif

#include <3rdparty/glext/glext.h>

// Placing get proc adress here to make sure it could be accessible

#ifdef WIN32
    #define getProcAdress wglGetProcAddress
#endif

#ifdef X11

#ifdef GLX_ARB_get_proc_adress
    #define getProcAdress(X) glXGetProcAddressARB((const GLubyte *)(X))
#else
    #define getProcAdress(X) glXGetProcAddress((const GLubyte *)(X))
#endif

#endif
