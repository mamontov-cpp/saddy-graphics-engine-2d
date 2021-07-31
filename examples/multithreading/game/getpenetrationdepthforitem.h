/*! \file getpenetrationdepthforitem.h
 *
 *  Gets penetration depth for item by it's sprite option depth
 */
#pragma once
#include <sadstring.h>

namespace game
{

/*! Clears penetration depths for all of items
 */
void clearItemPenetrationDepths();

/*! Sets penetration depth for item
    \param[in] string a string of item's icon options
    \param[in] depth a depth for item
 */
void setItemPenetrationDepth(const sad::String& string, int depth);

/*! Returns penetration depth for item
 *  \param[in] string a string option
 *  \return penetration depth for item
 */
int getPenetrationDepthForItem(const sad::String& string);

}
