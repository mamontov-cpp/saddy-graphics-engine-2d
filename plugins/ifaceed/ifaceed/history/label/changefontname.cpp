#include "changefontname.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"
#include "../../closuremethodcall.h"

history::label::ChangeFontName::ChangeFontName(
	sad::SceneNode* d,
	const sad::String& oldvalue,
	const sad::String& newvalue
	
)
: history::scenenodes::ChangeProperty<sad::String>(
      d,
      "font",
      oldvalue,
      newvalue
)
{

}

history::label::ChangeFontName::~ChangeFontName()
{

}


void history::label::ChangeFontName::updateUI(core::Editor* e, const sad::String& value)
{
    e->emitClosure( bind(
            e->panel()->UI()->rtwLabelFont,
            &gui::resourcetreewidget::ResourceTreeWidget::setSelectedResourceName,
            value
        )
    );
}
