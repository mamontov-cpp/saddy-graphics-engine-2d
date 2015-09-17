#include "animationschangefontlistfonts.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

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
    m_view =  e->panel()->UI()->txtFontListList;
    this->history::animations::ChangeList::updateUI(e, value);
}
