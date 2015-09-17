#include "scenenodeschangevisibility.h"

#include "../../mainpanel.h"

history::Command* history::scenenodes::changeVisibility(sad::SceneNode* d, bool oldvalue, bool newvalue)
{
    return new history::scenenodes::ChangePropertyWhichLinkedToCheckbox(
        &MainPanel::visibilityCheckbox,
        d,
        "visible",
        oldvalue,
        newvalue
    );	
}
