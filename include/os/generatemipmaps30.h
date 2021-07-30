/*! \file generatemipmaps30.h
    

    Defines cross-platform generation function of mip maps in 2D, as in OpenGL 3.0+
 */
#pragma once
#include "glheaders.h"

namespace sad
{
class Renderer;

namespace os
{

/*! Generate mip maps in OpenGL 3.0+ fashion
    \param[in] r renderer, which will render texture
    \param[in] target a target texture type
    \return whether we got some pointer to a function "glGenerateMipmap" and called it
 */
bool generateMipMaps30(sad::Renderer * r, GLenum target);

}

}

