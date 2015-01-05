#include "animationschangefontlistfonts.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"

#include <QStringList>

history::animations::ChangeFontListFonts::ChangeFontListFonts(
    sad::animations::Animation* d,
    const sad::Vector<sad::String>& oldvalue,
    const sad::Vector<sad::String>& newvalue

)
: history::animations::ChangeProperty<sad::Vector<sad::String> >(
      d,
      "fonts",
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
	QStringList list;
	for(size_t i = 0; i < value.size(); i++)
	{
		list << value[i].c_str();
	}
	QString nvalue = list.join("\n");

    e->emitClosure( blocked_bind(
            e->panel()->UI()->txtFontListList,
            &QTextEdit::setPlainText,
            nvalue
        )
    );
}
