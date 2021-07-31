#include "getpenetrationdepthforitem.h"
#include <sadvector.h>

/*! Struct for storing mapping from item to depth
 */
struct ItemNameToDepth {
    sad::String ItemName; //!< Name of item
    int Depth;            //!< Penetration depth
};

static sad::Vector<ItemNameToDepth>  itemname_to_penetration_depth;

void game::clearItemPenetrationDepths()
{
    itemname_to_penetration_depth.clear();
}

void game::setItemPenetrationDepth(const sad::String& string, int depth)
{
    sad::String copy = string;
    copy.toUpper();
    for (size_t i = 0; i < itemname_to_penetration_depth.size(); i++)
    {
        if (itemname_to_penetration_depth[i].ItemName == copy)
        {
            itemname_to_penetration_depth[i].Depth = depth;
            return;
        }
    }
    itemname_to_penetration_depth.push_back(ItemNameToDepth{copy, depth});
}


int game::getPenetrationDepthForItem(const sad::String& string)
{
    sad::String copy = string;
    copy.toUpper();
    for (size_t i = 0; i < itemname_to_penetration_depth.size(); i++)
    {
        if (copy.find(itemname_to_penetration_depth[i].ItemName) != std::string::npos)
        {
            return itemname_to_penetration_depth[i].Depth;
        }
    }
    return 0;
}
