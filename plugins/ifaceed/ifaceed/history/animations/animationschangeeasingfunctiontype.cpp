#include "animationschangeeasingfunctiontype.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include <QComboBox>

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeEasingFunctionType::ChangeEasingFunctionType(
    sad::animations::Animation* d,
    unsigned int oldvalue,
    unsigned int newvalue
) : history::animations::ChangeEasingProperty<unsigned int>(d, &sad::animations::easing::Function::setFunctionTypeAsUnsignedInt, oldvalue, newvalue)
{

}

history::animations::ChangeEasingFunctionType::~ChangeEasingFunctionType()
{
	
}

void history::animations::ChangeEasingFunctionType::updateUI(core::Editor* e, const unsigned int& value)
{
	gui::uiblocks::UIAnimationBlock* blk = e->uiBlocks()->uiAnimationBlock();
	e->emitClosure( blocked_bind(blk->cmbAnimationEasingType, &QComboBox::setCurrentIndex,  value) );
}