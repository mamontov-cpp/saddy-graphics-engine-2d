#include "groupschangesequential.h"

#include "../../core/editor.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../blockedclosuremethodcall.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationsgroupblock.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <QRadioButton>

history::groups::ChangeSequential::ChangeSequential(
    sad::animations::Group* d,
    bool oldvalue,
    bool newvalue
)
: history::groups::ChangeProperty<bool>(
      d,
      "sequential",
      oldvalue,
      newvalue
)
{
}

history::groups::ChangeSequential::~ChangeSequential()
{

}

void history::groups::ChangeSequential::updateUI(core::Editor* e, const bool& value)
{
	gui::uiblocks::UIAnimationsGroupBlock* blk = e->uiBlocks()->uiAnimationsGroupBlock();    
	e->emitClosure( blocked_bind(
            blk->rbAnimationsGroupParallel,
            &QRadioButton::setChecked,
            !value
        )
    );
	e->emitClosure( blocked_bind(
            blk->rbAnimationsGroupSequential,
            &QRadioButton::setChecked,
            value
        )
    );
}
