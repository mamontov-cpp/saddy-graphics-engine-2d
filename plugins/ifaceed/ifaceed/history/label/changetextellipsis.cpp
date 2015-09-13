#include "changetextellipsis.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../gui/labelactions.h"
#include "../../gui/scenenodeactions.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::label::ChangeTextEllipsis::ChangeTextEllipsis(
    sad::SceneNode* d,
    unsigned int oldvalue,
    unsigned int newvalue

)
: history::scenenodes::ChangeProperty<unsigned int>(
    d,
    "textellipsisposition",
    oldvalue,
    newvalue
)
{

}

history::label::ChangeTextEllipsis::~ChangeTextEllipsis()
{

}

void history::label::ChangeTextEllipsis::updateUI(
    core::Editor* e,
    const unsigned int& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->cmbLabelTextEllipsis,
            &QComboBox::setCurrentIndex,
            value
        )
    );
    e->panel()->sceneNodeActions()->updateRegionForNode();
}
