#include "scenenodeschangeangle.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"

history::scenenodes::ChangeAngle::ChangeAngle(
    sad::SceneNode* d,
    float oldvalue,
    float newvalue
) : history::scenenodes::ChangeProperty<float>(d, "angle", oldvalue, newvalue)
{
    
}

history::scenenodes::ChangeAngle::~ChangeAngle()
{
    
}

void history::scenenodes::ChangeAngle::updateUI(core::Editor* e, const float& value)
{
    gui::anglewidget::AngleWidget* w = e->panel()->UI()->awSceneNodeAngle;
    e->emitClosure( blocked_bind(
        w, 
        &gui::anglewidget::AngleWidget::setValue, 
        static_cast<double>(value)
    ) );
}
