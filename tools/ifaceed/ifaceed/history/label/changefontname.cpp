#include "changefontname.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

#include "../../closuremethodcall.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uilabelblock.h"

#include "../../gui/resourcetreewidget/resourcetreewidget.h"

history::label::ChangeFontName::ChangeFontName(
    sad::SceneNode* d,
    const sad::String& old_value,
    const sad::String& new_value
    
)
: history::scenenodes::ChangeProperty<sad::String>(
      d,
      "font",
      old_value,
      new_value
)
{
    m_affects_parent_grid = true;
}

history::label::ChangeFontName::~ChangeFontName()
{

}


void history::label::ChangeFontName::updateUI(core::Editor* e, const sad::String& value)
{
    e->emitClosure( bind(
            e->uiBlocks()->uiLabelBlock()->rtwLabelFont,
            &gui::resourcetreewidget::ResourceTreeWidget::setSelectedResourceName,
            value
        )
    );
}
