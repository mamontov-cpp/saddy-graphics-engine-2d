#include "animationschangeeasingperiod.h"
// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include <QDoubleSpinBox>

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeEasingPeriod::ChangeEasingPeriod(
    sad::animations::Animation* d,
    double old_value,
    double new_value
) : history::animations::ChangeEasingProperty<double>(d, &sad::animations::easing::Function::setPeriod, old_value, new_value)
{

}

history::animations::ChangeEasingPeriod::~ChangeEasingPeriod()
{
    
}

void history::animations::ChangeEasingPeriod::updateUI(core::Editor* e, const double& value)
{
    gui::uiblocks::UIAnimationBlock* blk = e->uiBlocks()->uiAnimationBlock();
    e->emitClosure( blocked_bind(blk->dsbAnimationEasingPeriod, &QDoubleSpinBox::setValue,  value) );
}