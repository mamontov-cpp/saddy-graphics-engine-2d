#include "scenenodeschangevisibility.h"
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiscenenodeblock.h"

history::Command* history::scenenodes::changeVisibility(
    core::Editor* blk, 
    sad::SceneNode* d, 
    bool old_value, 
    bool new_value
)
{
    return new history::scenenodes::ChangePropertyWhichLinkedToCheckbox(
        blk->uiBlocks()->uiSceneNodeBlock()->cbSceneNodeVisible,
        d,
        "visible",
        old_value,
        new_value
    );	
}
