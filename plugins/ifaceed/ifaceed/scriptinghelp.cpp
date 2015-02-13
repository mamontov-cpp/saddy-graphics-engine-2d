#include <scriptinghelp.h>

ScriptingHelp::ScriptingHelp(QWidget* parent) : QDialog(parent)
{
	m_ui.setupUi(this);

	connect(m_ui.txtRequest, SIGNAL(returnPressed()), this,SLOT(applySearch()));
	connect(m_ui.btnCancelSearch, SIGNAL(clicked()), this,SLOT(cancelSearch()));
}

ScriptingHelp::~ScriptingHelp()
{
	
}

void ScriptingHelp::applySearch()
{
	
}

void ScriptingHelp::cancelSearch()
{
	
}

void ScriptingHelp::setText(const QString& text)
{
	m_ui.txtHelp->setText(text);
}
