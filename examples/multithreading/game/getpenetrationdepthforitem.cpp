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
    {"ICE", 5},
    {"THUNDER", 5},
    {"BOW", 8},
    {"MACE", 8},
    {"DAGGER", 8},
    {"KEY", 8},
    {"AXE", 8},
    {"TORCH", 8},
    {"GUN", 8},
    {"THROW", 8},
    {"EYE", 5},
    {"BATWING", 8},
    {"CARROT", 5},
    {"SPEAR", 8},
    {"SWORD", 8},
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
