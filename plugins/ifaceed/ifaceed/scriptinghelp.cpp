#include <scriptinghelp.h>

ScriptingHelp::ScriptingHelp(QWidget* parent) : QDialog(parent)
{
	m_ui.setupUi(this);
}


ScriptingHelp::~ScriptingHelp()
{
	
}


void ScriptingHelp::setText(const QString& text)
{
	m_ui.txtHelp->setText(text);
}
