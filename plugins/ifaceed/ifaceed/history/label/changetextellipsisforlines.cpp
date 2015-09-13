#include "changetextellipsisforlines.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../gui/labelactions.h"
#include "../../gui/scenenodeactions.h"

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
            e->panel()->UI()->cmbLabelTextEllipsisForLines,
            &QComboBox::setCurrentIndex,
            value
        )
    );
    e->panel()->sceneNodeActions()->updateRegionForNode();
}
