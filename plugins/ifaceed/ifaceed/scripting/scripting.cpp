#include "scripting.h"
#include "mainpanel.h"

/*! A function for logging capabilities
	\param[in] context a context
	\param[in] engine an engine
 */
static QScriptValue logbinding(QScriptContext *context, QScriptEngine *engine)
{
	QScriptValue main = engine->globalObject().property("E");
	scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
	QTextEdit* edit = e->panel()->UI()->txtConsoleResults;
	for(size_t i = 0; i < context->argumentCount(); ++i)
	{
		edit->append(context->argument(i).toString());
	}
	return QScriptValue();
}

scripting::Scripting::Scripting(QObject* parent) : QObject(parent), m_panel(NULL)
{
	QScriptValue v = m_engine.newQObject(this, QScriptEngine::QtOwnership);
	v.setProperty("log", m_engine.newFunction(logbinding));
	m_engine.globalObject().setProperty("console", v, QScriptValue::ReadOnly);
	m_engine.globalObject().setProperty("E",v, QScriptValue::ReadOnly);
}

scripting::Scripting::~Scripting()
{
	
}

void scripting::Scripting::setPanel(MainPanel* panel)
{
	m_panel = panel;
}

MainPanel* scripting::Scripting::panel() const
{
	return m_panel;
}

void scripting::Scripting::runScript()
{
	m_panel->UI()->txtConsoleResults->setText("");
	QString text = m_panel->UI()->txtConsoleCode->toPlainText();
	QScriptValue result = m_engine.evaluate(text, "console.js");
	if (result.isError())
	{
		m_panel->UI()->txtConsoleResults->append(QString("<font color=\"red\">") + result.toString() + QString("</font>"));
	}
}
