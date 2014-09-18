#include "changefontsize.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../gui/labelactions.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::label::ChangeFontSize::ChangeFontSize(
    sad::SceneNode* d,
    unsigned int oldvalue,
    unsigned int newvalue

)
: history::scenenodes::ChangeProperty<unsigned int>(
    d,
    "fontsize",
    oldvalue,
    newvalue
)
{

}

history::label::ChangeFontSize::~ChangeFontSize()
{

}

void history::label::ChangeFontSize::updateUI(
    core::Editor* e,
    const unsigned int& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->fswLabelFontSize,
            &gui::fontsizewidget::FontSizeWidget::setValue,
            value
        )
    );
    e->panel()->labelActions()->updateRegionForLabel();
}
