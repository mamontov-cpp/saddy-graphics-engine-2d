#include "changemaximallinescount.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../gui/labelactions.h"
#include "../../gui/scenenodeactions.h"

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
            e->panel()->UI()->spbMaximalLinesCount,
            &QSpinBox::setValue,
            value
        )
    );
    e->panel()->sceneNodeActions()->updateRegionForNode();
}
