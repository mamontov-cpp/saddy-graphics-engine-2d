#include "scenenodeschangevisibility.h"
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiscenenodeblock.h"

history::Command* history::scenenodes::changeVisibility(
    core::Editor* blk, 
    sad::SceneNode* d, 
    bool oldvalue, 
    bool newvalue
)
{
    return new history::scenenodes::ChangePropertyWhichLinkedToCheckbox(
        blk->uiBlocks()->uiSceneNodeBlock()->cbSceneNodeVisible,
        d,
        "visible",
        oldvalue,
        newvalue
    );	
}
