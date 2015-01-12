#include "scripting.h"

#include "../mainpanel.h"

#include "../core/editor.h"

Q_DECLARE_METATYPE(sad::Point2D)
Q_DECLARE_METATYPE(sad::Rect2D)

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

/*! A function for making a point
    \param[in] context a context
    \param[in] engine an engine
 */
static QScriptValue makePoint2D(QScriptContext *context, QScriptEngine *engine)
{
    QScriptValue result;
    if (context->argumentCount() != 2)
    {
        context->throwError(QScriptContext::SyntaxError, "p2d() accepts only two parameters");
        return result;
    }
    QVariant v;
    v.setValue(sad::Point2D(context->argument(0).toInteger(), context->argument(1).toInteger()) );
    result = engine->newVariant(result, v);
    return result;
}

/*! A function for making a rectangle
    \param[in] context a context
    \param[in] engine an engine
 */
static QScriptValue makeRect2D(QScriptContext *context, QScriptEngine *engine)
{
    QScriptValue result;
    if (context->argumentCount() != 2 && context->argumentCount() != 4)
    {
        context->throwError(QScriptContext::SyntaxError, "r2d() accepts only two or four parameters");
        return result;
    }
    if (context->argumentCount() == 4)
    {
        if (context->argument(0).isNumber()
            && context->argument(1).isNumber()
            && context->argument(2).isNumber()
            && context->argument(3).isNumber())
        {
            QVariant v;
            v.setValue(sad::Rect2D(
                context->argument(0).toNumber(),
                context->argument(1).toNumber(),
                context->argument(2).toNumber(),
                context->argument(3).toNumber()
            ) );
            result = engine->newVariant(result, v);
        }
        else
        {
            QVariant v;
            v.setValue(sad::Rect2D(
                context->argument(0).toVariant().value<sad::Point2D>(),
                context->argument(1).toVariant().value<sad::Point2D>(),
                context->argument(2).toVariant().value<sad::Point2D>(),
                context->argument(3).toVariant().value<sad::Point2D>()
            ) );
            result = engine->newVariant(result, v);
        }
    }

    if (context->argumentCount() == 2)
    {
        QVariant v;
        v.setValue(sad::Rect2D(
            context->argument(0).toVariant().value<sad::Point2D>(),
            context->argument(1).toVariant().value<sad::Point2D>()
        ) );
        result = engine->newVariant(result, v);
    }

    return result;
}

scripting::Scripting::Scripting(QObject* parent) : QObject(parent), m_panel(NULL)
{
    m_engine = new QScriptEngine();
    QScriptValue v = m_engine->newQObject(this, QScriptEngine::QtOwnership);
    v.setProperty("log", m_engine->newFunction(logbinding));
    m_engine->globalObject().setProperty("console", v, QScriptValue::ReadOnly);
    m_engine->globalObject().setProperty("E",v, QScriptValue::ReadOnly);
    m_engine->globalObject().setProperty("p2d", m_engine->newFunction(makePoint2D));
    m_engine->globalObject().setProperty("r2d", m_engine->newFunction(makeRect2D));
}

scripting::Scripting::~Scripting()
{
    m_engine->collectGarbage();
    delete m_engine;
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
    history::BatchCommand* c = new history::BatchCommand();
    m_panel->editor()->setCurrentBatchCommand(c);

	m_panel->UI()->txtConsoleResults->setText("");
	QString text = m_panel->UI()->txtConsoleCode->toPlainText();
    QScriptValue result = m_engine->evaluate(text, "console.js");
	if (result.isError())
	{
		m_panel->UI()->txtConsoleResults->append(QString("<font color=\"red\">") + result.toString() + QString("</font>"));

        c->rollback(m_panel->editor());
        delete c;
    }
    else
    {
        if (c->count())
        {
            m_panel->editor()->history()->add(c);
        }
        else
        {
            delete c;
        }
    }
    m_panel->editor()->setCurrentBatchCommand(NULL);
}
