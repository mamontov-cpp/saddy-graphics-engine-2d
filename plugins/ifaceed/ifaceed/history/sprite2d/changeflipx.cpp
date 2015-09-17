#include "changeflipx.h"

#include "../../mainpanel.h"

history::Command* history::sprite2d::changeFlipX(sad::SceneNode* d, bool oldvalue, bool newvalue)
{
    return new history::scenenodes::ChangePropertyWhichLinkedToCheckbox(
        &MainPanel::flipXCheckbox,
        d,
        "flipx",
        oldvalue,
        newvalue
    );	
}
