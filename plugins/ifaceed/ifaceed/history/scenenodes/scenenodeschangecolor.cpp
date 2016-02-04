#include "scenenodeschangecolor.h"

#include "../../core/editor.h"

#include "../../core/typeconverters/sadacolortoqcolor.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiscenenodeblock.h"

#include "../../gui/qt-palette-color-picker/colorpicker.h"

#include "../../blockedclosuremethodcall.h"


history::scenenodes::ChangeColor::ChangeColor(
    sad::SceneNode* d,
    const sad::AColor& oldvalue,
    const sad::AColor& newvalue

)
: history::scenenodes::ChangeProperty<sad::AColor>(
      d,
      "color",
      oldvalue,
      newvalue
)
{
}

history::scenenodes::ChangeColor::~ChangeColor()
{

}

void history::scenenodes::ChangeColor::updateUI(core::Editor* e, const sad::AColor& value)
{
    QColor result;
    core::typeconverters::SadAColorToQColor::convert(value, result);
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiSceneNodeBlock()->clpSceneNodeColor,
            &gui::colorpicker::ColorPicker::setSelectedColor,
            result
        )
    );
}

