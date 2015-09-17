#include "changeflipy.h"

#include "../../mainpanel.h"

history::Command* history::sprite2d::changeFlipY(sad::SceneNode* d, bool oldvalue, bool newvalue)
{
    return new history::scenenodes::ChangePropertyWhichLinkedToCheckbox(
        &MainPanel::flipYCheckbox,
        d,
        "flipy",
        oldvalue,
        newvalue
    );	
}
