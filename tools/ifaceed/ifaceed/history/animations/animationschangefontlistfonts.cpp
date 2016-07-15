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
    const sad::Vector<sad::String>& oldvalue,
    const sad::Vector<sad::String>& newvalue

)
: history::animations::ChangeList(
      d,
      "fonts",
      NULL,
      oldvalue,
      newvalue
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
