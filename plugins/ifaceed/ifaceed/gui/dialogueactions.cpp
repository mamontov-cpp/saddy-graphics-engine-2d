#include "dialogueactions.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../closuremethodcall.h"
#include "../blockedclosuremethodcall.h"


Q_DECLARE_METATYPE(sad::p2d::app::Way*)

gui::DialogueActions::DialogueActions(QObject* parent) : QObject(parent)
{
	
}

gui::DialogueActions::~DialogueActions()
{
	
}

void gui::DialogueActions::setPanel(MainPanel* e)
{
	m_panel = e;
}

MainPanel* gui::DialogueActions::panel() const
{
	return m_panel;
}
