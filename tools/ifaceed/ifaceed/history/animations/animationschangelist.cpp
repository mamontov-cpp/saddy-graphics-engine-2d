#include "animationschangelist.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <db/save.h>

#include "../../core/editor.h"

#include "../../closuremethodcall.h"

#include "../../qstdstring.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <QTextEdit>
// ReSharper disable once CppUnusedIncludeDirective
#include <QStringList>

history::animations::ChangeList::ChangeList(
    sad::animations::Animation* d,
    const sad::String& property_name,
    QTextEdit* view,
    const sad::Vector<sad::String>& old_value,
    const sad::Vector<sad::String>& new_value

)
: history::animations::ChangeProperty<sad::Vector<sad::String> >(
      d,
      property_name,
      old_value,
      new_value
), m_view(view), m_should_change_widget(true)
{
}

void history::animations::ChangeList::commitWithoutUpdatingUI(core::Editor* ob)
{
    m_should_change_widget = false;
    commit(ob);
    m_should_change_widget = true;
}

history::animations::ChangeList::~ChangeList()
{

}

void history::animations::ChangeList::updateUI(core::Editor* e, const sad::Vector<sad::String>& value)
{
    if (m_should_change_widget)
    {
        QStringList list;
        for(size_t i = 0; i < value.size(); i++)
        {
            list << STD2QSTRING(value[i]);
        }
        QString nvalue = list.join("\n");

        e->emitClosure( ::bind(
                this,
                &history::Command::blockedSetTextEditText,
                m_view,
                nvalue
            )
        );
    }
}
