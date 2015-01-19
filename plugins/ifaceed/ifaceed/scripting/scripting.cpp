#include "scripting.h"
#include "scenebindings.h"
#include "registerconstructor.h"
#include "makeconstructor.h"
#include "scriptinglog.h"
#include "multimethod.h"
#include "point2d.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../history/scenes/sceneschangename.h"

Q_DECLARE_METATYPE(QScriptContext*)

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
    v.setProperty("log", m_engine->newFunction(scripting::scriptinglog));  // E.log 
	v.setProperty("set", m_engine->newFunction(setCustomObjectProperty)); // E.set

	QScriptValue scenes = m_engine->newObject();
	scenes.setProperty("add", m_engine->newFunction(scripting::addScene));  // E.scenes.add
	scenes.setProperty("remove", m_engine->newFunction(scripting::removeScene)); // E.scenes.remove
	scenes.setProperty("moveBack", m_engine->newFunction(scripting::sceneMoveBack)); // E.scenes.moveBack
	scenes.setProperty("moveFront", m_engine->newFunction(scripting::sceneMoveFront)); // E.scenes.moveFront

	v.setProperty("scenes", scenes); // E.scenes

	
	m_engine->globalObject().setProperty("console", v, QScriptValue::ReadOnly);
    m_engine->globalObject().setProperty("E",v, QScriptValue::ReadOnly);
    
	// A sad::Point2D constructor	
	QScriptClass* c = new scripting::ConstructorCall2<sad::Point2D, double, double>(m_engine, "Point2D");
	m_registered_classes << c;
	QScriptValue ctor = m_engine->newObject(c);
	QScriptValue metaObject = m_engine->newQMetaObject(&scripting::Point2D::staticMetaObject, ctor);
	m_engine->globalObject().setProperty("Point2D", metaObject, QScriptValue::ReadOnly);
	
	// A sad::Rect2D constructor
	scripting::MultiMethod* rect2dconstructor = new scripting::MultiMethod(m_engine, "r2d");
	rect2dconstructor->add(scripting::make_constructor<sad::Rect2D, sad::Point2D, sad::Point2D>(this));
	rect2dconstructor->add(scripting::make_constructor<sad::Rect2D, double, double, double, double>(this));
	rect2dconstructor->add(scripting::make_constructor<sad::Rect2D, sad::Point2D, sad::Point2D, sad::Point2D, sad::Point2D>(this));
	this->registerScriptClass("r2d", rect2dconstructor);

	// A sad::Color
	scripting::MultiMethod* clrconstructor = new scripting::MultiMethod(m_engine, "clr");
	clrconstructor->add(scripting::make_constructor<sad::AColor, unsigned char, unsigned char, unsigned char>(this));
	clrconstructor->add(scripting::make_constructor<sad::AColor, unsigned char, unsigned char, unsigned char, unsigned char>(this));
	this->registerScriptClass("clr", clrconstructor);   // clr
}

scripting::Scripting::~Scripting()
{
    m_engine->collectGarbage();
    delete m_engine;
	for(size_t i = 0; i < m_registered_classes.size(); i++)
	{
		delete m_registered_classes[i];
	}
}

void scripting::Scripting::setPanel(MainPanel* panel)
{
	m_panel = panel;
}

MainPanel* scripting::Scripting::panel() const
{
	return m_panel;
}

QScriptEngine* scripting::Scripting::engine() const
{
	return m_engine;
}

void scripting::Scripting::registerFunction(const QString& name, QScriptValue& v)
{
    v.setProperty("name", name);
    m_engine->globalObject().setProperty("name", v);
}

void scripting::Scripting::registerScriptClass(const QString& name, QScriptClass* c)
{
	m_engine->globalObject().setProperty(name, m_engine->newObject(c));
	if (m_registered_classes.contains(c) == false) {
		m_registered_classes << c;
	}
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
