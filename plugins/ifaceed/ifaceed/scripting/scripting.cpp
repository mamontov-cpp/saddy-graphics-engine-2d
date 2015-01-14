#include "scripting.h"
#include "scenebindings.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../history/scenes/sceneschangename.h"


Q_DECLARE_METATYPE(sad::Point2D)
Q_DECLARE_METATYPE(sad::Rect2D)
Q_DECLARE_METATYPE(sad::AColor)

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

/*! A function for making a rectangle
    \param[in] context a context
    \param[in] engine an engine
 */
static QScriptValue makeColor(QScriptContext *context, QScriptEngine *engine)
{
    QScriptValue result;
    if (context->argumentCount() != 3 && context->argumentCount() != 4)
    {
        context->throwError(QScriptContext::SyntaxError, "clr() accepts only three or four parameters");
        return result;
    }
	int components[4] = {0, 0, 0, 0};
	for(size_t i = 0; i < context->argumentCount(); i++)
	{
		if (context->argument(i).isNumber() == false)
		{
			context->throwError(
				QScriptContext::SyntaxError, 
				QString("clr(): argument #") + QString::number(i) + QString(" is not a number")
			);
			return result;
		}
		
		components[i] = context->argument(i).toInt32();
		if (components[i] < 0 || components[i] > 255)
		{
			context->throwError(
				QScriptContext::SyntaxError, 
				QString("clr(): argument #") + QString::number(i) + QString(" is outside withing 0-255 range")
			);
			return result;
		}
	}

	QVariant v;
    v.setValue(sad::AColor(components[0], components[1], components[2], components[3]) );
    result = engine->newVariant(result, v);
	return result;
}
/*! A universal setting property for object
 */
static  QScriptValue setCustomObjectProperty(QScriptContext *context, QScriptEngine *engine)
{
	QScriptValue main = engine->globalObject().property("E");
	scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());

	sad::db::Object* result = NULL;
	if (context->argumentCount() != 3)
    {
        context->throwError(QScriptContext::SyntaxError, "set() accepts only three arguments");
        return main;
    }

	bool handled = false;
	//	Querying object
	if (context->argument(0).isString())
	{
		sad::Vector<sad::db::Object*> os = sad::Renderer::ref()->database("")->queryByName(context->argument(0).toString().toStdString());
		if(os.size())
		{
			result  = os[0];
		}
	}
	else
	{
		if (context->argument(0).isNumber())
		{
			result = sad::Renderer::ref()->database("")->queryByMajorId(context->argument(0).toInt32());
		}
		else
		{
			 context->throwError(QScriptContext::SyntaxError, "set() : first argument must be string or number");
			 return main;
		}
	}

	if (result == NULL)
	{
		context->throwError(QScriptContext::SyntaxError, "set() : first argument does not names an object");
		return main;
	}

	QString propertyname;
	if (context->argument(1).isString())
	{
		propertyname = context->argument(1).toString();
	}


	core::Editor* editor =  e->panel()->editor();
	if (propertyname.size() != 0)
	{
		if (result->isInstanceOf("sad::Scene"))
		{
			handled = true;
			if (context->argument(2).isString() && propertyname == "name")
			{
				sad::Scene* s = static_cast<sad::Scene*>(result);
				sad::String oldname = s->objectName();
				sad::String newname = context->argument(2).toString().toStdString();

				if (newname != oldname)
				{
					history::Command* c = new history::scenes::ChangeName(s, oldname, newname);								
					editor->currentBatchCommand()->add(c);
					c->commit(editor);
				}
			}
		}
	}

	if (handled == false)
	{
		context->throwError(QScriptContext::SyntaxError, "set(): Unknown object");
	}

	return main;
}
scripting::Scripting::Scripting(QObject* parent) : QObject(parent), m_panel(NULL)
{
    m_engine = new QScriptEngine();
    QScriptValue v = m_engine->newQObject(this, QScriptEngine::QtOwnership);
    v.setProperty("log", m_engine->newFunction(logbinding));  // E.log 
	v.setProperty("set", m_engine->newFunction(setCustomObjectProperty)); // E.set

	QScriptValue scenes = m_engine->newObject();
	scenes.setProperty("add", m_engine->newFunction(scripting::addScene));  // E.scenes.add
	scenes.setProperty("remove", m_engine->newFunction(scripting::removeScene)); // E.scenes.remove
	scenes.setProperty("moveBack", m_engine->newFunction(scripting::sceneMoveBack)); // E.scenes.moveBack
	scenes.setProperty("moveFront", m_engine->newFunction(scripting::sceneMoveFront)); // E.scenes.moveFront

	v.setProperty("scenes", scenes); // E.scenes

    m_engine->globalObject().setProperty("console", v, QScriptValue::ReadOnly);
    m_engine->globalObject().setProperty("E",v, QScriptValue::ReadOnly);
    m_engine->globalObject().setProperty("p2d", m_engine->newFunction(makePoint2D)); // p2d
    m_engine->globalObject().setProperty("r2d", m_engine->newFunction(makeRect2D));  // r2d
	m_engine->globalObject().setProperty("clr", m_engine->newFunction(makeColor));   // clr
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
        m_panel->UI()->txtConsoleResults->append(QString("<font color=\"red\">")
                                                 + result.toString()
                                                 + QString("<br/>Backtrace:<br/>")
                                                 + m_engine->uncaughtExceptionBacktrace().join("<br/>")
                                                 + QString("</font>")
        );
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
