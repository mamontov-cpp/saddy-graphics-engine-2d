#include "changetextellipsisforlines.h"

#include <QComboBox>

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uilabelblock.h"

#include "../../gui/actions/actions.h"
#include "../../gui/actions/scenenodeactions.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::label::ChangeTextEllipsisForLines::ChangeTextEllipsisForLines(
    sad::SceneNode* d,
    unsigned int oldvalue,
    unsigned int newvalue

)
: history::scenenodes::ChangeProperty<unsigned int>(
    d,
    "textellipsispositionforlines",
    oldvalue,
    newvalue
)
{

}

history::label::ChangeTextEllipsisForLines::~ChangeTextEllipsisForLines()
{

}

void history::label::ChangeTextEllipsisForLines::updateUI(
    core::Editor* e,
    const unsigned int& value)
{
    e->emitClosure( blocked_bind(
            e->uiBlocks()->uiLabelBlock()->cmbLabelTextEllipsisForLines,
            &QComboBox::setCurrentIndex,
            value
        )
    );
    e->actions()->sceneNodeActions()->updateRegionForNode();
}
