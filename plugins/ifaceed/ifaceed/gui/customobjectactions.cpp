#include "customobjectactions.h"

#include "../blockedclosuremethodcall.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../core/typeconverters/qcolortosadacolor.h"

#include "../history/scenenodes/scenenodesnew.h"

#include "../history/sprite2d/changeoptions.h"
#include "../history/sprite2d/makebackground.h"
#include "../history/sprite2d/changeflipx.h"
#include "../history/sprite2d/changeflipy.h"

#include "../gui/scenenodeactions.h"

#include <sprite2d.h>
#include <geometry2d.h>

#include <QDebug>

// ===============================  PUBLIC METHODS ===============================

gui::CustomObjectActions::CustomObjectActions(QObject* parent) : QObject(parent), m_panel(NULL)
{

}

gui::CustomObjectActions::~CustomObjectActions()
{
	
}

void gui::CustomObjectActions::setPanel(MainPanel* e)
{
	m_panel = e;
}

MainPanel* gui::CustomObjectActions::panel() const
{
	return m_panel;
}