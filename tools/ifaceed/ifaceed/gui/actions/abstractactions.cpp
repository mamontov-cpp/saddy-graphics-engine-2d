#include "abstractactions.h"

#include <db/dbobject.h>

#include ".././qstdstring.h"

#include <new>


gui::actions::AbstractActions::AbstractActions() : m_editor(nullptr)
{
    
}

void gui::actions::AbstractActions::setEditor(core::Editor* editor)
{
    m_editor = editor;
}

core::Editor* gui::actions::AbstractActions::editor() const
{
    return m_editor;
}

gui::actions::AbstractActions::~AbstractActions()
{
    
}

QString gui::actions::AbstractActions::viewableObjectName(sad::db::Object* o)
{
    std::string name = o->objectName().c_str();
    QString result = STD2QSTRING(name);
    if (result.length() == 0)
    {
        char buffer[20];
        sprintf(buffer, "%p", o);
        result = QString(buffer);
    }
    return result;
}
