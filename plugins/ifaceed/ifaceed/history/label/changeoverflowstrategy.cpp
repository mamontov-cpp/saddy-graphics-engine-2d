#include "changeoverflowstrategy.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../gui/labelactions.h"
#include "../../gui/scenenodeactions.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::label::ChangeOverflowStrategy::ChangeOverflowStrategy(
    sad::SceneNode* d,
    unsigned int oldvalue,
    unsigned int newvalue

)
: history::scenenodes::ChangeProperty<unsigned int>(
    d,
    "overflowstrategy",
    oldvalue,
    newvalue
)
{

}

history::label::ChangeOverflowStrategy::~ChangeOverflowStrategy()
{

}

void history::label::ChangeOverflowStrategy::updateUI(
    core::Editor* e,
    const unsigned int& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->cmbLabelOverflowStrategy,
            &QComboBox::setCurrentIndex,
            value
        )
    );
    e->panel()->sceneNodeActions()->updateRegionForNode();
}
