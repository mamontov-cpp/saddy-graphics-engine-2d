#include "animationschangelist.h"

#include "../../core/editor.h"

#include "../../mainpanel.h"

#include "../../blockedclosuremethodcall.h"

#include "../../qstdstring.h"

#include <QStringList>

history::animations::ChangeList::ChangeList(
    sad::animations::Animation* d,
    const sad::String& propertyname,
    QTextEdit* view,
    const sad::Vector<sad::String>& oldvalue,
    const sad::Vector<sad::String>& newvalue

)
: history::animations::ChangeProperty<sad::Vector<sad::String> >(
      d,
      propertyname,
      oldvalue,
      newvalue
), m_view(view)
{
}

history::animations::ChangeList::~ChangeList()
{

}

void history::animations::ChangeList::updateUI(core::Editor* e, const sad::Vector<sad::String>& value)
{
    QStringList list;
    for(size_t i = 0; i < value.size(); i++)
    {
        list << STD2QSTRING(value[i]);
    }
    QString nvalue = list.join("\n");

    e->emitClosure( blocked_bind(
            m_view,
            &QTextEdit::setPlainText,
            nvalue
        )
    );
}
