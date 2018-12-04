/*! \file getpenetrationdepthforitem.h
 *
 *  Gets penetration depth for item by it's sprite option depth
 */
#pragma once
#include <sadstring.h>

namespace game
{

/*! Returns penetration depth for item
 *  \param[in] string a string option
 *  \return penetration depth for item
 */
int getPenetationDepthForItem(const sad::String& string);

}
