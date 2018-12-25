#include "getpenetrationdepthforitem.h"

/*! Struct for storing mapping from item to depth
 */
struct ItemNameToDepth {
    const char* ItemName; //!< Name of item
    int Depth;            //!< Penetration depth
};
/*! An array of related to depth data
 */
static ItemNameToDepth itemname_to_penetration_depth[] = {
    {"SWORD021", 3},
    {"ICE", 3},
    {"THUNDER", 3},
    {"BOW", 4},
    {"MACE", 4},
    {"DAGGER", 4},
    {"KEY", 4},
    {"AXE", 4},
    {"TORCH", 4},
    {"GUN", 4},
    {"THROW", 4},
    {"EYE", 3},
    {"BATWING", 4},
    {"CARROT", 3},
    {"SPEAR", 4},
    {"SWORD", 4},
    {NULL, 0}
};

int game::getPenetationDepthForItem(const sad::String& string)
{
    sad::String copy = string;
    copy.toUpper();
    size_t current = 0;
    while (itemname_to_penetration_depth[current].ItemName) {
        if (copy.find(itemname_to_penetration_depth[current].ItemName) != std::string::npos) {
            return itemname_to_penetration_depth[current].Depth;
        }
        ++current;
    }
    return 0;
}
