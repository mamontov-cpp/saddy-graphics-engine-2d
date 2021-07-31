#include "scenenodeschangeangle.h"

#include "../../core/editor.h"

#include "../../core/typeconverters/sadrect2dtoqrectf.h"


// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uiscenenodeblock.h"

#include "../../gui/anglewidget/anglewidget.h"

#include "../../blockedclosuremethodcall.h"

history::scenenodes::ChangeAngle::ChangeAngle(
    sad::SceneNode* d,
    float old_value,
    float new_value
) : history::scenenodes::ChangeProperty<float>(d, "angle", old_value, new_value)
{
    m_affects_parent_grid = true;
}

history::scenenodes::ChangeAngle::~ChangeAngle()
{
    
}

void history::scenenodes::ChangeAngle::updateUI(core::Editor* e, const float& value)
{
    gui::anglewidget::AngleWidget* w = e->uiBlocks()->uiSceneNodeBlock()->awSceneNodeAngle;
    e->emitClosure( blocked_bind(
        w, 
        &gui::anglewidget::AngleWidget::setValue, 
        static_cast<double>(value)
    ) );
}