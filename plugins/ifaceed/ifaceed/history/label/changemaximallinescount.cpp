#include "changemaximallinescount.h"

#include <QSpinBox>

#include "../../core/editor.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uilabelblock.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/scenenodeactions.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::label::ChangeMaximalLinesCount::ChangeMaximalLinesCount(
    sad::SceneNode* d,
    unsigned int oldvalue,
    unsigned int newvalue

)
: history::scenenodes::ChangeProperty<unsigned int>(
    d,
    "maximallinescount",
    oldvalue,
    newvalue
)
{

}

history::label::ChangeMaximalLinesCount::~ChangeMaximalLinesCount()
{

}

void history::label::ChangeMaximalLinesCount::updateUI(
    core::Editor* e,
    const unsigned int& value)
{
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiLabelBlock()->spbMaximalLinesCount,
            &QSpinBox::setValue,
            value
        )
    );
    e->actions()->sceneNodeActions()->updateRegionForNode();
}
