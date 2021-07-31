#include "changeflipy.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>


#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uispriteblock.h"

history::Command* history::sprite2d::changeFlipY(
    core::Editor* e, 
    sad::SceneNode* d,
    bool old_value, 
    bool new_value
)
{
    return new history::scenenodes::ChangePropertyWhichLinkedToCheckbox(
        e->uiBlocks()->uiSpriteBlock()->cbFlipY,
        d,
        "flipy",
        old_value,
        new_value
    );	
}
