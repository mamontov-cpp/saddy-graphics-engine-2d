#include "changetext.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

#include "../../gui/labelactions.h"

history::label::ChangeText::ChangeText(
    sad::SceneNode* d,
    const sad::String& oldvalue,
    const sad::String& newvalue

) : history::scenenodes::PropertyChanged<sad::String>(
    d,
    "text",
    oldvalue,
    newvalue
)
{

}

history::label::ChangeText::~ChangeText()
{

}

void history::label::ChangeText::updateUI(core::Editor* e, const sad::String& value)
{
    e->emitClosure( blocked_bind(
            e->panel()->UI()->txtLabelText,
            &QPlainTextEdit::setPlainText,
            QString(value.c_str())
        )
    );
    e->panel()->labelActions()->updateRegionForLabel();
}
