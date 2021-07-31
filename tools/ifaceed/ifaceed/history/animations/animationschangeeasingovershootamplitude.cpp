#include "animationschangeeasingovershootamplitude.h"
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include <QDoubleSpinBox>

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeEasingOvershootAmplitude::ChangeEasingOvershootAmplitude(
    sad::animations::Animation* d,
    double old_value,
    double new_value
) : history::animations::ChangeEasingProperty<double>(d, &sad::animations::easing::Function::setOvershootAmplitude, old_value, new_value)
{

}

history::animations::ChangeEasingOvershootAmplitude::~ChangeEasingOvershootAmplitude()
{
    
}

void history::animations::ChangeEasingOvershootAmplitude::updateUI(core::Editor* e, const double& value)
{
    gui::uiblocks::UIAnimationBlock* blk = e->uiBlocks()->uiAnimationBlock();
    e->emitClosure( blocked_bind(blk->dsbAnimationEasingOvershootAmplitude, &QDoubleSpinBox::setValue,  value) );
}