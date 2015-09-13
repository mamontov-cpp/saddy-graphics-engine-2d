#include "changeoverflowstrategyforlines.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../gui/labelactions.h"
#include "../../gui/scenenodeactions.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::label::ChangeOverflowStrategyForLines::ChangeOverflowStrategyForLines(
    sad::SceneNode* d,
    unsigned int oldvalue,
    unsigned int newvalue

)
: history::scenenodes::ChangeProperty<unsigned int>(
    d,
    "overflowstrategyforlines",
    oldvalue,
    newvalue
)
{

}

history::label::ChangeOverflowStrategyForLines::~ChangeOverflowStrategyForLines()
{

}

void history::label::ChangeOverflowStrategyForLines::updateUI(
    core::Editor* e,
    const unsigned int& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->cmbLabelOverflowStrategyForLines,
            &QComboBox::setCurrentIndex,
            value
        )
    );
    e->panel()->sceneNodeActions()->updateRegionForNode();
}
