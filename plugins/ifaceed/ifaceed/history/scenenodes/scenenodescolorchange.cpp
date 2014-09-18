#include "scenenodescolorchange.h"

#include "../../core/editor.h"

#include "../../core/typeconverters/sadacolortoqcolor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::scenenodes::ChangeColor::ChangeColor(
    sad::SceneNode* d,
    const sad::AColor& oldvalue,
    const sad::AColor& newvalue

)
: history::scenenodes::PropertyChanged<sad::AColor>(
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
            e->panel()->UI()->clpSceneNodeColor,
            &gui::colorpicker::ColorPicker::setSelectedColor,
            result
        )
    );
}

