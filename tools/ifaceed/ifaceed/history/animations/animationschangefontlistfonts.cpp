#include "animationschangefontlistfonts.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <QPlainTextEdit>

// ReSharper disable once CppUnusedIncludeDirective
#include "../../gui/textedit/textedit.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"

history::animations::ChangeFontListFonts::ChangeFontListFonts(
    sad::animations::Animation* d,
    const sad::Vector<sad::String>& old_value,
    const sad::Vector<sad::String>& new_value

)
: history::animations::ChangeList(
      d,
      "fonts",
      nullptr,
      old_value,
      new_value
)
{
}

history::animations::ChangeFontListFonts::~ChangeFontListFonts()
{

}

void history::animations::ChangeFontListFonts::updateUI(core::Editor* e, const sad::Vector<sad::String>& value)
{
    m_view =  e->uiBlocks()->uiAnimationBlock()->txtFontListList;
    this->history::animations::ChangeList::updateUI(e, value);
}
