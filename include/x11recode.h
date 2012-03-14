/*!  \file x11recode.h
      \author HiddenSeeker
       
     Contains a recode procedure to convert key codes from X11 to sad::Input codes
*/
#pragma once

#include "renderer.h"
#include "input.h"

namespace sad
{

 /*! Recodes an event to sad::Input key code
      \param[in] xkey event
      \return code of sad::Input
 */
 int recode(XKeyEvent * xkey);

}
