#include "scripting.h"
#include "constructorcall.h"
#include "makeconstructor.h"
#include "scriptinglog.h"
#include "multimethod.h"
#include "makescriptingcall.h"
#include "abstractgetter.h"
#include "queryresource.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../history/scenenodes/scenenodeschangename.h"
#include "../history/scenenodes/scenenodeschangeangle.h"
#include "../history/scenenodes/scenenodeschangecolor.h"
#include "../history/scenenodes/scenenodeschangevisibility.h"

#include "../history/label/changetext.h"
#include "../history/label/changelinespacing.h"

#include "../history/sprite2d/changeflipx.h"
#include "../history/sprite2d/changeflipy.h"

#include "../history/ways/wayschangename.h"
#include "../history/ways/wayschangetotaltime.h"
#include "../history/ways/wayschangeclosed.h"

#include "../history/dialogues/dialogueschangename.h"

#include "../history/animations/animationschangename.h"
#include "../history/animations/animationschangetime.h"
#include "../history/animations/animationschangelooped.h"
#include "../history/animations/animationschangeblinkingfrequency.h"
#include "../history/animations/animationschangecameraoffset.h"
#include "../history/animations/animationschangeshakingfrequency.h"
#include "../history/animations/animationschangecameraangle.h"
#include "../history/animations/animationschangecamerapivot.h"
#include "../history/animations/animationschangecolorcolor.h"
#include "../history/animations/animationschangefontlistfonts.h"
#include "../history/animations/animationschangefontsizesize.h"
#include "../history/animations/animationschangelist.h"


#include "database/databasebindings.h"
#include "database/databasepropertysetter.h"
#include "database/databasepropertygetter.h"

#include "scenes/scenesbindings.h"
#include "scenes/scenesnamesetter.h"

#include "scenenodes/scenenodesbindings.h"
#include "scenenodes/scenenodessetter.h"
#include "scenenodes/scenenodesflagsetter.h"
#include "scenenodes/scenenodesareasetter.h"
#include "scenenodes/scenenodesfontsizesetter.h"
#include "scenenodes/scenenodesfontsetter.h"
#include "scenenodes/scenenodesoptionssetter.h"
#include "scenenodes/scenenodesschemasetter.h"
#include "scenenodes/scenenodescustomgetter.h"
#include "scenenodes/scenenodescustomsetter.h"

#include "ways/waysbindings.h"
#include "ways/wayssetter.h"

#include "dialogues/dialoguesbindings.h"
#include "dialogues/dialoguessetter.h"

#include "animations/animationsbindings.h"
#include "animations/animationssetter.h"
#include "animations/animationswidgetsetter.h"

#include <QFileDialog>
#include <QTextStream>

#include <animations/animationsblinking.h>
#include <animations/animationscamerashaking.h>
#include <animations/animationscamerarotation.h>
#include <animations/animationscolor.h>
#include <animations/animationsfontlist.h>
#include <animations/animationsfontsize.h>
#include <animations/animationsoptionlist.h>

Q_DECLARE_METATYPE(QScriptContext*)

// ================================== PUBLIC METHODS OF scripting::Scripting::Thread ==================================
int scripting::Scripting::Thread::TIMEOUT = 60000;

int scripting::Scripting::Thread::POLLINGTIME = 300;

scripting::Scripting::Thread::Thread(scripting::Scripting* me) : m_should_i_quit(false), m_s(me)
{
	
}

scripting::Scripting::Thread::~Thread()
{
	
}

void scripting::Scripting::Thread::forceQuit()
{
	m_should_i_quit = true;	
}

void scripting::Scripting::Thread::run()
{
	this->msleep(scripting::Scripting::Thread::POLLINGTIME);

	int timeout = 0;
	while (!m_should_i_quit && m_s->engine()->isEvaluating())
	{
		this->msleep(scripting::Scripting::Thread::POLLINGTIME);
		timeout += scripting::Scripting::Thread::POLLINGTIME; 
		m_should_i_quit = timeout >= scripting::Scripting::Thread::TIMEOUT;
	}
	if (m_should_i_quit && timeout >= scripting::Scripting::Thread::TIMEOUT)
	{
		m_s->cancelExecution();
	}
}

// ================================== PUBLIC METHODS OF scripting::Scripting ==================================
scripting::Scripting::Scripting(QObject* parent) : QObject(parent), m_panel(NULL)
{
    m_engine = new QScriptEngine();
    m_value = m_engine->newQObject(this, QScriptEngine::QtOwnership);
    m_value.setProperty("log", m_engine->newFunction(scripting::scripting_log));  // E.log
	
	m_engine->globalObject().setProperty("console", m_value, QScriptValue::ReadOnly);
    m_engine->globalObject().setProperty("E",m_value, QScriptValue::ReadOnly);
	
    scripting::Callable* oresourcetype = scripting::make_scripting_call(scripting::resource_type, this);
    m_registered_classes << oresourcetype;
    m_value.setProperty("resourceType", m_engine->newObject(oresourcetype), QScriptValue::ReadOnly);

    scripting::Callable* oresourceoptions = scripting::make_scripting_call(scripting::resource_options, this);
    m_registered_classes << oresourceoptions;
    m_value.setProperty("resourceOptions",m_engine->newObject(oresourceoptions), QScriptValue::ReadOnly);

    scripting::Callable* oresourceschema = scripting::make_scripting_call(scripting::resource_schema, this);
    m_registered_classes << oresourceschema;
    m_value.setProperty("resourceSchema", m_engine->newObject(oresourceschema), QScriptValue::ReadOnly);

	this->initSadTypeConstructors();
	this->initDatabasePropertyBindings(m_value);
	this->initSceneBindings(m_value);
	this->initSceneNodesBindings(m_value);
    this->initWaysBindings(m_value);
	this->initDialoguesBindings(m_value);
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
	this->initAnimationsBindings(m_value);
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
	
	scripting::Scripting::Thread poller(this);
	poller.start();
	QScriptValue result = m_engine->evaluate(text, "console.js");
	poller.forceQuit();

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
	poller.wait();
}


void scripting::Scripting::showHelp()
{
	// A size of help dialog
	QSize dialogsize(640, 480);

	QDialog dlg;
	dlg.setWindowTitle("Console Scripting Help");
	dlg.resize(dialogsize);
	dlg.setMinimumSize(dialogsize);
	dlg.setMaximumSize(dialogsize);
	
	QWidget* widget = new QWidget(&dlg);
    widget->setObjectName(QString::fromUtf8("layoutWidget"));
    widget->setGeometry(QRect(0, 0, 640, 480));
	

	QHBoxLayout* hboxLayout = new QHBoxLayout(widget);
	
	QTextEdit* edit = new QTextEdit();
	edit->setReadOnly(true);
	edit->setText(
		"<h2>Scripting with IFace Editor</h2>\n"
		"<h3>Common classes:</h3>\n"
		"<ul>"
		"<li><b>sad::Point2D</b> - a basic 2D point with coordinates as double"
		"    <ul>"
		"        <li>constructor <b>p2d(), p2d(2,3)</b> - constructs a point with specified parameters ( (0,0) in first case) </li>"
		"        <li>property <b>x</b> - stores x coordinate</li>"
		"        <li>property <b>y</b> - stores y coordinate</li>"
		"    </ul>"
		"</li>"
		"<li><b>sad::Point2I</b> - a basic 2D point with coordinates as int"
		"    <ul>"
		"        <li>constructor <b>p2i(), p2i(2,3)</b> - constructs a point with specified parameters ( (0,0) in first case) </li>"
		"        <li>property <b>x</b> - stores x coordinate</li>"
		"        <li>property <b>y</b> - stores y coordinate</li>"
		"    </ul>"
		"</li>"
		"<li><b>sad::Point3D</b> - a basic 3D point with coordinates as double"
		"    <ul>"
		"        <li>constructor <b>p3d(), p3d(2,3,4)</b> - constructs a point with specified parameters ( (0,0,0) in first case) </li>"
		"        <li>property <b>x</b> - stores x coordinate</li>"
		"        <li>property <b>y</b> - stores y coordinate</li>"
		"        <li>property <b>z</b> - stores z coordinate</li>"
		"    </ul>"
		"</li>"
		"<li><b>sad::Point3I</b> - a basic 3D point with coordinates as int"
		"    <ul>"
		"        <li>constructor <b>p3i(), p3i(2,3,4)</b> - constructs a point with specified parameters ( (0,0,0) in first case) </li>"
		"        <li>property <b>x</b> - stores x coordinate</li>"
		"        <li>property <b>y</b> - stores y coordinate</li>"
		"        <li>property <b>z</b> - stores z coordinate</li>"
		"    </ul>"
		"</li>"
		"<li><b>sad::Rect2D</b> - a rectangle, where all points are sad::Point2D"
		"    <ul>"
		"        <li>constructors <b>r2d(), r2d(p2d(0,0),p2d(0,0)), r2d(0,0,0,0), r2d(p2d(0,0),p2d(0,0),p2d(0,0),p2d(0,0))</b> - constructs a rect with specified parameters</li>"
		"        <li>method <b>setPoint(index, p2d(0,0))</b> - sets a point for rectangle, where index=[0..3]</li>"
		"        <li>method <b>point(index)</b> - returns a point for rectangle, where index=[0..3]</li>"
		"    </ul>"
		"</li>"
		"<li><b>sad::Rect2I</b> - a rectangle, where all points are sad::Point2I"
		"    <ul>"
		"        <li>constructors <b>r2i(), r2i(p2i(0,0),p2i(0,0)), r2i(0,0,0,0), r2i(p2i(0,0),p2i(0,0),p2i(0,0),p2i(0,0))</b> - constructs a rect with specified parameters</li>"
		"        <li>method <b>setPoint(index, p2i(0,0))</b> - sets a point for rectangle, where index=[0..3]</li>"
		"        <li>method <b>point(index)</b> - returns a point for rectangle, where index=[0..3]</li>"
		"    </ul>"
		"</li>"
		"<li><b>sad::Size2D</b> - a basic 2D size with coordinates as double"
		"    <ul>"
		"        <li>constructor <b>s2d(), s2d(2,3)</b> - constructs a size with specified parameters ( (0,0) in first case) </li>"
		"        <li>property <b>width</b> - stores width</li>"
		"        <li>property <b>height</b> - stores height</li>"
		"    </ul>"
		"</li>"
		"<li><b>sad::Size2I</b> - a basic 2D size with coordinates as unsigned integral value"
		"    <ul>"
		"        <li>constructor <b>s2i(), s2i(2,3)</b> - constructs a size with specified parameters ( (0,0) in first case) </li>"
		"        <li>property <b>width</b> - stores width</li>"
		"        <li>property <b>height</b> - stores height</li>"
		"    </ul>"
		"</li>"
		"<li><b>sad::Color</b> - a basic color in RGB-space"
		"    <ul>"
		"        <li>constructor <b>clr(), clr(r,g,b)</b> - constructs a color with specified parameters ( (0,0,0) in first case) </li>"
		"        <li>property <b>r</b> - red component</li>"
		"        <li>property <b>g</b> - green component</li>"
		"        <li>property <b>b</b> - blue component</li>"
		"    </ul>"
		"</li>"
		"<li><b>sad::AColor</b> - a basic color in RGBA-space"
		"    <ul>"
		"        <li>constructor <b>aclr(), aclr(r,g,b), aclr(r,g,b,a)</b> - constructs a color with specified parameters ( (0,0,0,0) in first case) </li>"
		"        <li>property <b>r</b> - red component</li>"
		"        <li>property <b>g</b> - green component</li>"
		"        <li>property <b>b</b> - blue component</li>"
		"        <li>property <b>a</b> - alpha component</li>"
		"    </ul>"
		"</li>"
		"<li><b>sad::dialogue::Phrase</b> - a phrase"
		"    <ul>"
		"        <li>constructor <b>phrase(actorName,actorPortrait,text,duration,viewHint)</b> - constructs new phrase </li>"
		"        <li>property <b>actorName</b> - a name for actor</li>"
		"        <li>property <b>actorPortrait</b> - a portrait for actor</li>"
		"        <li>property <b>text</b> - a text for phrase</li>"
		"        <li>property <b>duration</b> - a duration for phrase</li>"
		"        <li>property <b>viewHint</b> - a viewHint for phrase</li>"
		"    </ul>"
		"</li>"
		"</ul>"
		"<h3>Editor-specific objects:</h3>\n"
		"<ul>"
		"	<li><b>E</b> or <b>console</b> - a basic class for all operations in editor"
		"	<ul>"
		"		<li>method <b>log</b> - logs all arguments, converting them to string</li>"
        "		<li>method <b>resourceType(\"name\")</b> - returns type of resource in tree</li>"
        "		<li>method <b>resourceOptions(\"name\")</b> - returns immutable resource wrapper for options for sprite</li>"
        "		<li>method <b>resourceSchema(\"name\")</b> - returns immutable object schema wrapper</li>"
        "		<li>property <b>db</b> - holds all operations, related to database properties and object metaprogramming"
		"			<ul>"
		"				<li>method <b>list()</b> - lists properties from database, returning an array of names of properties</li>"
		"				<li>method <b>type(\"name or majorid\")</b> -  returns real name of type for object. Note, that it could be performed only on scenes, scene nodes, ways, dialogues, animations, instances and groups</li>"
		"				<li>method <b>readableProperties(\"name or majorid\")</b> -  returns list of readable properties for a specified object</li>"
		"				<li>method <b>writableProperties(\"name or majorid\")</b> -  returns list of writable properties for a specified object</li>"
		"				<li>method <b>add(\"type\", \"name\")</b> - adds new property to database. Returns true on success</li>"		
		"				<li>method <b>remove(\"name\")</b> - removes a property for database</li>"		
		"				<li>method <b>set(\"name\", value)</b> - sets a property value for database</li>"		
		"				<li>method <b>get(\"name\", value)</b> - returns a property value for database</li>"		
		"				<li>method <b>attr</b> - depending from number of arguments applies <b>set</b> or <b>get</b> methods respectively</li>"
		"			</ul>"
		"		</li>"
		"		<li>property <b>scenes</b> - holds all scene-related operations"
		"			<ul>"
		"				<li>method <b>list()</b> - lists scenes, returning all of majorids for it</li>"
		"				<li>method <b>add()</b>, <b>add(\"name\")</b> - adds scene, named or anonymous</li>"
		"				<li>method <b>remove(22)</b>, <b>remove(\"name\")</b> - removes scene by id or by name</li>"
		"				<li>method <b>moveBack(22)</b>, <b>moveBack(\"name\")</b> - moves scene back in list by id or by name</li>"
		"				<li>method <b>moveFront(22)</b>, <b>moveFront(\"name\")</b> - moves scene front in list by id or by name</li>"
		"				<li>method <b>set(\"scenename\", \"propertyname\", \"value\")</b> - sets property of scene. You can fetch only name by specifying \"name\" as second parameter</li>"
		"				<li>method <b>get(\"scenename\", \"propertyname\", \"value\")</b> - fetched property of scene by it\'s name"
		"					<ul>"
		"						<li>property <b>\"name\"</b>  - name as string</li>"
		"						<li>property <b>\"layer\"</b>  - a layer of scene in list. The less it is, the more early scene is drawn</li>"
		"						<li>property <b>\"majorid\"</b>  - a major id of scene in database. Useful for links.</li>"
		"						<li>property <b>\"minorid\"</b>  - a minor id of scene in database. Useful for links in your application.</li>"
		"					</ul>"
		"				</li>"
		"				<li>method <b>attr</b> - depending from number of arguments applies <b>set</b> or <b>get</b> methods respectively</li>"
		"			</ul>"
		"		</li>"
		"		<li>property <b>scenenodes</b> - holds all operations related scene nodes"
		"			<ul>"
		"				<li>method <b>list()</b> - lists all scenenodes from all scenes, returning all of majorids for it</li>"
		"				<li>method <b>listScene(\"scene\")</b> - lists scene nodes from scene, named by name or major id</li>"
		"				<li>method <b>_addLabel(name of scene, font name, font size, text, name, position as point, color) - adds a label to a scene</li>"
		"				<li>method <b>addLabel(object) - does the same as previous only style is different. Object must have fields:  \"font\", \"text\", \"point\", \"scene\" . Fields \"fontsize\", \"color\", \"name\" are optional.</li>"
		"				<li>method <b>_addSprite2D(name of scene, sprite options name, name, area as rectangle, color) - adds a sprite to a scene</li>"
		"				<li>method <b>addSprite2D(object) - does the same as previous only style is different. Object must have fields:  \"sprite\", \"area\", \"scene\" . Fields \"color\", \"name\" are optional.</li>"
		"				<li>method <b>_addCustomObject(name of scene, schema name, name, font size, text, area as rectangle, color) - adds a custom object to a scene</li>"
		"				<li>method <b>addCustomObject(object) - does the same as previous only style is different. Object must have fields:  \"schema\", \"fontsize\", \"scene\", \"text\", \"area\" . Fields \"fontsize\", \"color\", \"name\" are optional.</li>"
		"				<li>method <b>makeBackground(scene node) - sets nodes as background. May cause labels to disappear</li>"
		"				<li>method <b>remove(22)</b>, <b>remove(\"name\")</b> - removes scene node by id or by name</li>"
		"				<li>method <b>moveBack(22)</b>, <b>moveBack(\"name\")</b> - moves scene back in list by id or by name</li>"
		"				<li>method <b>moveFront(22)</b>, <b>moveFront(\"name\")</b> - moves scene front in list by id or by name</li>"
		"				<li>method <b>set(\"nodename\", \"propertyname\", \"value\")</b> - sets property of scene."
		"					<ul>"	
		"						<li><b>[All node types]</b>property <b>\"name\"</b>  - name of node as string.</li>"
		"						<li><b>[All node types]</b>property <b>\"visible\"</b>  - visibility of node as bool.</li>"
		"						<li><b>[All node types]</b>property <b>\"area\"</b>  - area of node as sad::Rect2D.</li>"
		"						<li><b>[All node types]</b>property <b>\"angle\"</b>  - angle of node as double.</li>"
		"						<li><b>[All node types]</b>property <b>\"color\"</b>  - color of node as sad::AColor.</li>"
		"						<li><b>[Label or Custom Object]</b>property <b>\"fontsize\"</b>  - font size.</li>"
		"						<li><b>[Label or Custom Object]</b>property <b>\"text\"</b>  - text of label.</li>"
		"						<li><b>[Label or Custom Object]</b>property <b>\"linespacing\"</b>  - line spacing for label.</li>"
		"						<li><b>[Label or Custom Object]</b>property <b>\"font\"</b>  - a font resource name.</li>"
		"						<li><b>[Sprite or Custom Object]</b>property <b>\"flipx\"</b>  - a flag, whether sprite is flipped by horizontal.</li>"
		"						<li><b>[Sprite or Custom Object]</b>property <b>\"flipy\"</b>  - a flag, whether sprite is flipped by vertical.</li>"
		"						<li><b>[Sprite or Custom Object]</b>property <b>\"options\"</b>  - a sprite options resource name.</li>"
		"						<li><b>[Custom Object]</b>property <b>\"schema\"</b>  - a schema resource name.</li>"
		"						<li><b>[Custom Object] properties of custom objects.</li>"
		"					</ul>"
		"				</li>"		
		"				<li>method <b>get(\"nodename\", \"propertyname\", \"value\")</b> - fetched property of scene by it\'s name"
		"					<ul>"
		"						<li><b>[All node types]</b>property <b>\"name\"</b>  - name as string</li>"
		"						<li><b>[All node types]</b>property <b>\"layer\"</b>  - a layer of scene in list. The less it is, the more early scene is drawn</li>"
		"						<li><b>[All node types]</b>property <b>\"majorid\"</b>  - a major id of scene in database. Useful for links.</li>"
		"						<li><b>[All node types]</b>property <b>\"minorid\"</b>  - a minor id of scene in database. Useful for links in your application.</li>"
		"						<li><b>[All node types]</b>property <b>\"visible\"</b>  - visibility of node as bool.</li>"
		"						<li><b>[All node types]</b>property <b>\"area\"</b>  - area of node as sad::Rect2D.</li>"
		"						<li><b>[All node types]</b>property <b>\"angle\"</b>  - angle of node as double.</li>"
		"						<li><b>[All node types]</b>property <b>\"color\"</b>  - color of node as sad::AColor.</li>"
		"						<li><b>[Label or Custom Object]</b>property <b>\"fontsize\"</b>  - font size.</li>"
		"						<li><b>[Label or Custom Object]</b>property <b>\"text\"</b>  - text of label.</li>"
		"						<li><b>[Label or Custom Object]</b>property <b>\"linespacing\"</b>  - line spacing for label.</li>"
		"						<li><b>[Label or Custom Object]</b>property <b>\"font\"</b>  - a font resource name.</li>"
		"						<li><b>[Sprite or Custom Object]</b>property <b>\"flipx\"</b>  - a flag, whether sprite is flipped by horizontal.</li>"
		"						<li><b>[Sprite or Custom Object]</b>property <b>\"flipy\"</b>  - a flag, whether sprite is flipped by vertical.</li>"
		"						<li><b>[Sprite or Custom Object]</b>property <b>\"options\"</b>  - a sprite options resource name.</li>"
		"						<li><b>[Custom Object]</b>property <b>\"schema\"</b>  - a schema resource name.</li>"
		"						<li><b>[Custom Object] properties of custom objects.</li>"
		"					</ul>"
		"				</li>"
		"				<li>method <b>attr</b> - depending from number of arguments applies <b>set</b> or <b>get</b> methods respectively</li>"
		"			</ul>"
		"		</li>"
		"		<li>property <b>ways</b> - holds all operations related to ways"
		"			<ul>"
		"				<li>method <b>list()</b> - lists all ways, returning all of majorids for it</li>"
		"				<li>method <b>_add(name, totaltime as double, closed as bool, array of points)</b> - adds new way to a db</li>"
		"				<li>method <b>add(object)</b> - does the same as previous only style is different. Fields \"name\", \"totaltime\", \"closed\", \"points\" are optional.</li>"
		"				<li>method <b>remove(22)</b>, <b>remove(\"name\")</b> - removes way by id or by name</li>"
		"				<li>method <b>addPoint(way, point)</b> - adds new point to a way.</li>"
		"				<li>method <b>removePoint(way, position of point)</b> - removes a point from a way.</li>"
		"				<li>method <b>point(way, position of point)</b> - returns a reference of point from a way, which can be casted to common point. Throws exception on invalid positions. Properties of point:"
		"					<ul>"
		"						<li>property <b>x</b> - x coordinate of point</li>"
		"						<li>property <b>y</b> - y coordinate of point</li>"
		"						<li>property <b>position</b> - read-only position of point</li>"
		"						<li>method <b>moveBack</b> - moves point back in way list (does nothing, if point cannot be moved in such way)</li>"
		"						<li>method <b>moveFront</b> - moves point front in way list (does nothing, if point cannot be moved in such way)</li>"
		"					</ul>"
		"				</li>"
		"				<li>method <b>set(\"wayname\", \"propertyname\", \"value\")</b> - sets property of scene."
		"					<ul>"	
		"						<li>property <b>\"name\"</b>  - name as string</li>"
		"						<li>property <b>\"totaltime\"</b>  - a total time for a way.</li>"
		"						<li>property <b>\"closed\"</b>  - whether way is closed.</li>"		
		"					</ul>"
		"				</li>"		
		"				<li>method <b>get(\"wayname\", \"propertyname\", \"value\")</b> - fetched property of scene by it\'s name"
		"					<ul>"
		"						<li>property <b>\"name\"</b>  - name as string</li>"
		"						<li>property <b>\"majorid\"</b>  - a major id of scene in database. Useful for links.</li>"
		"						<li>property <b>\"minorid\"</b>  - a minor id of scene in database. Useful for links in your application.</li>"
		"						<li>property <b>\"totaltime\"</b>  - a total time for a way.</li>"
		"						<li>property <b>\"closed\"</b>  - whether way is closed.</li>"		
		"					</ul>"
		"				</li>"
		"				<li>method <b>attr</b> - depending from number of arguments applies <b>set</b> or <b>get</b> methods respectively</li>"
		"			</ul>"
		"		</li>"
		"		<li>property <b>dialogues</b> - holds all operations related to dialogues"
		"			<ul>"
		"				<li>method <b>list()</b> - lists all dialogues, returning all of majorids for it</li>"
		"				<li>method <b>_add(name, array of phrases)</b> - adds new way to a db</li>"
		"				<li>method <b>add(object)</b> - does the same as previous only style is different. Fields \"name\",  \"points\" are optional.</li>"
		"				<li>method <b>remove(22)</b>, <b>remove(\"name\")</b> - removes dialogue by id or by name</li>"
		"				<li>method <b>addPhrase(dialogue, phrase)</b> - adds new phrase to a dialogue.</li>"
		"				<li>method <b>removePhrase(dialogue, position of phrase)</b> - removes a phrase from a dialogue.</li>"
		"				<li>method <b>phrase(dialogue, position of phrase)</b> - returns a reference of phrase from a dialogues, which can be casted to common phrase. Throws exception on invalid positions. Properties of reference to a phrase:"
		"					<ul>"
		"						<li>property <b>actorName</b> - an actor name</li>"
		"						<li>property <b>actorPortrait</b> - an actor portrait as string</li>"
		"						<li>property <b>text</b> - a text</li>"
		"						<li>property <b>duration</b> - a duration of phrase</li>"
		"						<li>property <b>viewHint</b> - a view hint</li>"
		"						<li>property <b>position</b> - read-only position of phrase</li>"
		"						<li>method <b>moveBack</b> - moves phrase back in way list (does nothing, if phrase cannot be moved in such way)</li>"
		"						<li>method <b>moveFront</b> - moves phrase front in way list (does nothing, if phrase cannot be moved in such way)</li>"
		"					</ul>"
		"				</li>"
		"				<li>method <b>set(\"wayname\", \"propertyname\", \"value\")</b> - sets property of scene."
		"					<ul>"	
		"						<li>property <b>\"name\"</b>  - name as string</li>"
		"					</ul>"
		"				</li>"		
		"				<li>method <b>get(\"wayname\", \"propertyname\", \"value\")</b> - fetched property of scene by it\'s name"
		"					<ul>"
		"						<li>property <b>\"name\"</b>  - name as string</li>"
		"						<li>property <b>\"majorid\"</b>  - a major id of scene in database. Useful for links.</li>"
		"						<li>property <b>\"minorid\"</b>  - a minor id of scene in database. Useful for links in your application.</li>"
		"					</ul>"
		"				</li>"
		"				<li>method <b>attr</b> - depending from number of arguments applies <b>set</b> or <b>get</b> methods respectively</li>"
		"			</ul>"
		"		</li>"
		"	</ul>"
		"	</li>"
		"</ul>"
	);
	
	hboxLayout->addWidget(edit);

	dlg.exec();
}


void scripting::Scripting::cancelExecution()
{
	if (m_engine->isEvaluating())
	{
		m_engine->abortEvaluation(m_engine->currentContext()->throwError(QScriptContext::SyntaxError, "Aborted due timeout"));
	}
}

void scripting::Scripting::initSadTypeConstructors()
{
	
	// A sad::Point2D constructor	
	scripting::MultiMethod* point2dconstructor = new scripting::MultiMethod(m_engine, "p2d");
	point2dconstructor->add(scripting::make_constructor<sad::Point2D>(this));
	point2dconstructor->add(scripting::make_constructor<sad::Point2D, double, double>(this));
	this->registerScriptClass("p2d", point2dconstructor);

	// A sad::Point2I constructor	
	scripting::MultiMethod* point2iconstructor = new scripting::MultiMethod(m_engine, "p2i");
	point2iconstructor->add(scripting::make_constructor<sad::Point2I>(this));
	point2iconstructor->add(scripting::make_constructor<sad::Point2I, int, int>(this));
	this->registerScriptClass("p2i", point2iconstructor);

	// A sad::Point3D constructor	
	scripting::MultiMethod* point3dconstructor = new scripting::MultiMethod(m_engine, "p3d");
	point3dconstructor->add(scripting::make_constructor<sad::Point3D>(this));
	point3dconstructor->add(scripting::make_constructor<sad::Point3D, sad::Point2D>(this));
	point3dconstructor->add(scripting::make_constructor<sad::Point3D, double, double>(this));
	point3dconstructor->add(scripting::make_constructor<sad::Point3D, double, double, double>(this));
	this->registerScriptClass("p3d", point3dconstructor);

	// A sad::Point3I constructor	
	scripting::MultiMethod* point3iconstructor = new scripting::MultiMethod(m_engine, "p3i");
	point3iconstructor->add(scripting::make_constructor<sad::Point3I>(this));
	point3iconstructor->add(scripting::make_constructor<sad::Point3I, sad::Point2I>(this));
	point3iconstructor->add(scripting::make_constructor<sad::Point3I, int, int>(this));
	point3iconstructor->add(scripting::make_constructor<sad::Point3I, int, int, int>(this));
	this->registerScriptClass("p3i", point3iconstructor);

	// A sad::Rect2D constructor
	scripting::MultiMethod* rect2dconstructor = new scripting::MultiMethod(m_engine, "r2d");
	rect2dconstructor->add(scripting::make_constructor<sad::Rect2D>(this));
	rect2dconstructor->add(scripting::make_constructor<sad::Rect2D, sad::Point2D, sad::Point2D>(this));
	rect2dconstructor->add(scripting::make_constructor<sad::Rect2D, double, double, double, double>(this));
	rect2dconstructor->add(scripting::make_constructor<sad::Rect2D, sad::Point2D, sad::Point2D, sad::Point2D, sad::Point2D>(this));
	this->registerScriptClass("r2d", rect2dconstructor);

	// A sad::Rect2I constructor
	scripting::MultiMethod* rect2iconstructor = new scripting::MultiMethod(m_engine, "r2i");
	rect2iconstructor->add(scripting::make_constructor<sad::Rect2I>(this));
	rect2iconstructor->add(scripting::make_constructor<sad::Rect2I, sad::Point2I, sad::Point2I>(this));
	rect2iconstructor->add(scripting::make_constructor<sad::Rect2I, int, int, int, int>(this));
	rect2iconstructor->add(scripting::make_constructor<sad::Rect2I, sad::Point2I, sad::Point2I, sad::Point2I, sad::Point2I>(this));
	this->registerScriptClass("r2i", rect2iconstructor);

	// A sad::Size2D constructor	
	scripting::MultiMethod* size2dconstructor = new scripting::MultiMethod(m_engine, "s2d");
	size2dconstructor->add(scripting::make_constructor<sad::Size2D>(this));
	size2dconstructor->add(scripting::make_constructor<sad::Size2D, double, double>(this));
	this->registerScriptClass("s2d", size2dconstructor);

	// A sad::Size2I constructor	
	scripting::MultiMethod* size2iconstructor = new scripting::MultiMethod(m_engine, "s2i");
	size2iconstructor->add(scripting::make_constructor<sad::Size2I>(this));
	size2iconstructor->add(scripting::make_constructor<sad::Size2I, unsigned int, unsigned int>(this));
	this->registerScriptClass("s2i", size2iconstructor);

	// A sad::Color
	scripting::MultiMethod* clrconstructor = new scripting::MultiMethod(m_engine, "clr");
	clrconstructor->add(scripting::make_constructor<sad::Color>(this));
	clrconstructor->add(scripting::make_constructor<sad::Color, unsigned char, unsigned char, unsigned char>(this));
	this->registerScriptClass("clr", clrconstructor);

	// A sad::AColor
	scripting::MultiMethod* aclrconstructor = new scripting::MultiMethod(m_engine, "aclr");
	aclrconstructor->add(scripting::make_constructor<sad::AColor>(this));
	aclrconstructor->add(scripting::make_constructor<sad::AColor, unsigned char, unsigned char, unsigned char>(this));
	aclrconstructor->add(scripting::make_constructor<sad::AColor, unsigned char, unsigned char, unsigned char, unsigned char>(this));
	this->registerScriptClass("aclr", aclrconstructor);   
}

void scripting::Scripting::initDatabasePropertyBindings(QScriptValue& v)
{
	QScriptValue db = m_engine->newObject();
	
    db.setProperty("list", m_engine->newFunction(scripting::database::list)); // E.db.list
	
    scripting::Callable* tp = scripting::make_scripting_call(scripting::database::type, this);
	m_registered_classes << tp;
	db.setProperty("type", m_engine->newObject(tp)); // E.db.type

    db.setProperty("readableProperties", m_engine->newFunction(scripting::database::readableProperties)); // E.db.readableProperties
    db.setProperty("writableProperties", m_engine->newFunction(scripting::database::writableProperties)); // E.db.writableProperties


    scripting::Callable* add = scripting::make_scripting_call(scripting::database::addProperty, this);
	m_registered_classes << add;
	db.setProperty("add", m_engine->newObject(add)); // E.db.add
	
    scripting::Callable* remove = scripting::make_scripting_call(scripting::database::removeProperty, this);
	m_registered_classes << remove;
	db.setProperty("remove", m_engine->newObject(remove)); // E.db.remove

	scripting::MultiMethod* set = new scripting::MultiMethod(m_engine, "set");
#define PUSH_SETTER(TYPE) set->add(new scripting::database::PropertySetter< TYPE >(m_engine));
	PUSH_SETTER( double )
	PUSH_SETTER( float )
	PUSH_SETTER( int )
	PUSH_SETTER( long )
	PUSH_SETTER( long long )
	PUSH_SETTER( sad::AColor )
	PUSH_SETTER( sad::Color )
	PUSH_SETTER( sad::Point2D )
	PUSH_SETTER( sad::Point2I )
	PUSH_SETTER( sad::Point3D )
	PUSH_SETTER( sad::Point3I )
	PUSH_SETTER( sad::Size2D )
	PUSH_SETTER( sad::Size2I )
	PUSH_SETTER( sad::Rect2D )
	PUSH_SETTER( sad::Rect2I )
	PUSH_SETTER( sad::String )
	PUSH_SETTER( std::string )
	PUSH_SETTER( QString )
	PUSH_SETTER( short )
	PUSH_SETTER( bool )
	PUSH_SETTER( char )
	PUSH_SETTER( signed char )
	PUSH_SETTER( unsigned char )
	PUSH_SETTER( unsigned int )
	PUSH_SETTER( unsigned long )
	PUSH_SETTER( unsigned long long )
	PUSH_SETTER( unsigned short )
#undef PUSH_SETTER
	m_registered_classes << set;
	db.setProperty("set", m_engine->newObject(set)); // E.db.set

	scripting::MultiMethod* get = new scripting::MultiMethod(m_engine, "get");
#define PUSH_GETTER(TYPE) get->add(new scripting::database::PropertyGetter< TYPE >(m_engine));
	PUSH_GETTER( double )
	PUSH_GETTER( float )
	PUSH_GETTER( int )
	PUSH_GETTER( long )
	PUSH_GETTER( long long )
	PUSH_GETTER( sad::AColor )
	PUSH_GETTER( sad::Color )
	PUSH_GETTER( sad::Point2D )
	PUSH_GETTER( sad::Point2I )
	PUSH_GETTER( sad::Point3D )
	PUSH_GETTER( sad::Point3I )
	PUSH_GETTER( sad::Size2D )
	PUSH_GETTER( sad::Size2I )
	PUSH_GETTER( sad::Rect2D )
	PUSH_GETTER( sad::Rect2I )
	PUSH_GETTER( sad::String )
	PUSH_GETTER( std::string )
	PUSH_GETTER( QString )
	PUSH_GETTER( short )
	PUSH_GETTER( bool )
	PUSH_GETTER( char )
	PUSH_GETTER( signed char )
	PUSH_GETTER( unsigned char )
	PUSH_GETTER( unsigned int )
	PUSH_GETTER( unsigned long )
	PUSH_GETTER( unsigned long long )
	PUSH_GETTER( unsigned short )
#undef PUSH_GETTER
	m_registered_classes << get;
	db.setProperty("get", m_engine->newObject(get)); // E.db.get
	
	v.setProperty("db", db); // E.db

	m_engine->evaluate(
		"E.db.attr = function() {"  
		"	if (arguments.length == 1)"
		"	{"
        "		return E.db.get(arguments[0]);"
		"	}"
		"	if (arguments.length == 2)"
		"	{"
        "		return E.scenes.set(arguments[0], arguments[1]);"
		"	}"
		"	throw new Error(\"Specify 1 or 2 arguments\");"
		"};"
	);
}

void scripting::Scripting::initSceneBindings(QScriptValue& v)
{
	QScriptValue scenes = m_engine->newObject();


    scenes.setProperty("list", m_engine->newFunction(scripting::scenes::list));  // E.scenes.list

	// An add method
	scripting::MultiMethod* add = new scripting::MultiMethod(m_engine, "add");
    add->add(scripting::make_scripting_call(scripting::scenes::add, this));
    add->add(scripting::make_scripting_call(scripting::scenes::addNameless, this));
	m_registered_classes << add;
	
	scenes.setProperty("add", m_engine->newObject(add));  // E.scenes.add
	
    scripting::Callable* remove = scripting::make_scripting_call(scripting::scenes::remove, this);
	m_registered_classes << remove;
	scenes.setProperty("remove", m_engine->newObject(remove)); // E.scenes.remove

    scripting::Callable* moveback = scripting::make_scripting_call(scripting::scenes::moveBack, this);
	m_registered_classes << moveback;
	scenes.setProperty("moveBack", m_engine->newObject(moveback)); // E.scenes.moveBack

    scripting::Callable* movefront = scripting::make_scripting_call(scripting::scenes::moveFront, this);
	m_registered_classes << movefront;
	scenes.setProperty("moveFront", m_engine->newObject(movefront)); // E.scenes.moveFront

	scripting::MultiMethod* set = new scripting::MultiMethod(m_engine, "set");
	set->add(new scripting::scenes::NameSetter(m_engine));
	m_registered_classes << set;
	scenes.setProperty("set", m_engine->newObject(set)); // E.scenes.set

	scripting::MultiMethod* get = new scripting::MultiMethod(m_engine, "get");
	get->add(new scripting::AbstractGetter<sad::Scene*, sad::String>(m_engine, "name"));
	get->add(new scripting::AbstractGetter<sad::Scene*, unsigned int>(m_engine, "layer"));
	get->add(new scripting::AbstractGetter<sad::Scene*, unsigned long long>(m_engine, "majorid"));
	get->add(new scripting::AbstractGetter<sad::Scene*, unsigned long long>(m_engine, "minorid"));
	m_registered_classes << get;
	scenes.setProperty("get", m_engine->newObject(get)); // E.scenes.set

	v.setProperty("scenes", scenes); // E.scenes

	m_engine->evaluate(
		"E.scenes.attr = function() {"  
		"	if (arguments.length == 2)"
		"	{"
        "		return E.scenes.get(arguments[0], arguments[1]);"
		"	}"
		"	if (arguments.length == 3)"
		"	{"
        "		return E.scenes.set(arguments[0], arguments[1], arguments[2]);"
		"	}"
		"	throw new Error(\"Specify 2 or 3 arguments\");"
		"};"
	);
}


void scripting::Scripting::initSceneNodesBindings(QScriptValue& v)
{
	QScriptValue scenenodes = m_engine->newObject();

    scenenodes.setProperty("list", m_engine->newFunction(scripting::scenenodes::list)); // E.scenenodes.list

    scripting::Callable* listscene = scripting::make_scripting_call(scripting::scenenodes::listScene, this);
    m_registered_classes << listscene;
    scenenodes.setProperty("listScene", m_engine->newObject(listscene)); // E.scenenodes.listScene


	scripting::Callable* _addlabel = scripting::make_scripting_call(scripting::scenenodes::_addLabel, this);
	m_registered_classes << _addlabel;
	scenenodes.setProperty("_addLabel", m_engine->newObject(_addlabel)); // E.scenenodes._addLabel

    scripting::Callable* _addsprite2d = scripting::make_scripting_call(scripting::scenenodes::_addSprite2D, this);
    m_registered_classes << _addsprite2d;
    scenenodes.setProperty("_addSprite2D", m_engine->newObject(_addsprite2d)); // E.scenenodes._addSprite2D

    scripting::Callable* _addcustomobject = scripting::make_scripting_call(scripting::scenenodes::_addCustomObject, this);
    m_registered_classes << _addcustomobject;
    scenenodes.setProperty("_addCustomObject", m_engine->newObject(_addcustomobject)); // E.scenenodes._addCustomObject

	scripting::Callable* makeBackground = scripting::make_scripting_call(scripting::scenenodes::makeBackground, this);
    m_registered_classes << makeBackground;
    scenenodes.setProperty("makeBackground", m_engine->newObject(makeBackground)); // E.scenenodes.makeBackground

	scripting::Callable* remove = scripting::make_scripting_call(scripting::scenenodes::remove, this);
    m_registered_classes << remove;
    scenenodes.setProperty("remove", m_engine->newObject(remove)); // E.scenenodes.remove

	scripting::MultiMethod* set = new scripting::MultiMethod(m_engine, "set");
    // All props
    set->add(new scripting::scenenodes::FlagSetter(m_engine, "visible", history::scenenodes::changeVisibility));
	set->add(new scripting::scenenodes::Setter<sad::String, history::scenenodes::ChangeName>(m_engine, "name"));
    set->add(new scripting::scenenodes::AreaSetter(m_engine));
    set->add(new scripting::scenenodes::Setter<double, history::scenenodes::ChangeAngle>(m_engine, "angle"));
    set->add(new scripting::scenenodes::Setter<sad::AColor, history::scenenodes::ChangeColor>(m_engine, "color"));
    // sad::Label props
    set->add(new scripting::scenenodes::FontSizeSetter(m_engine));
    set->add(new scripting::scenenodes::Setter<sad::String, history::label::ChangeText>(m_engine, "text"));
    set->add(new scripting::scenenodes::Setter<float, history::label::ChangeLineSpacing>(m_engine, "linespacing"));
    set->add(new scripting::scenenodes::FontSetter(m_engine));
	// sad::Sprite2D props
    set->add(new scripting::scenenodes::FlagSetter(m_engine, "flipx", history::sprite2d::changeFlipX));
    set->add(new scripting::scenenodes::FlagSetter(m_engine, "flipy", history::sprite2d::changeFlipY));
    set->add(new scripting::scenenodes::OptionsSetter(m_engine));
	// sad::db::CustomObject props
	set->add(new scripting::scenenodes::SchemaSetter(m_engine));
#define PUSH_SETTER(TYPE) set->add(new scripting::scenenodes::CustomSetter< TYPE >(m_engine));
	PUSH_SETTER( double )
	PUSH_SETTER( float )
	PUSH_SETTER( int )
	PUSH_SETTER( long )
	PUSH_SETTER( long long )
	PUSH_SETTER( sad::AColor )
	PUSH_SETTER( sad::Color )
	PUSH_SETTER( sad::Point2D )
	PUSH_SETTER( sad::Point2I )
	PUSH_SETTER( sad::Point3D )
	PUSH_SETTER( sad::Point3I )
	PUSH_SETTER( sad::Size2D )
	PUSH_SETTER( sad::Size2I )
	PUSH_SETTER( sad::Rect2D )
	PUSH_SETTER( sad::Rect2I )
	PUSH_SETTER( sad::String )
	PUSH_SETTER( std::string )
	PUSH_SETTER( QString )
	PUSH_SETTER( short )
	PUSH_SETTER( bool )
	PUSH_SETTER( char )
	PUSH_SETTER( signed char )
	PUSH_SETTER( unsigned char )
	PUSH_SETTER( unsigned int )
	PUSH_SETTER( unsigned long )
	PUSH_SETTER( unsigned long long )
	PUSH_SETTER( unsigned short )
#undef PUSH_SETTER
	m_registered_classes << set;
	scenenodes.setProperty("set", m_engine->newObject(set)); // E.scenes.set
	
	scripting::MultiMethod* get = new scripting::MultiMethod(m_engine, "get");
    // All
    get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::String>(m_engine, "name"));
	get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>(m_engine, "layer"));
	get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned long long>(m_engine, "majorid"));
	get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned long long>(m_engine, "minorid"));
	get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned long long>(m_engine, "scene"));
	get->add(new scripting::AbstractGetter<sad::SceneNode*, bool>(m_engine, "visible"));	
    get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::Rect2D>(m_engine, "area"));
    get->add(new scripting::AbstractGetter<sad::SceneNode*, double>(m_engine, "angle"));
    get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::AColor>(m_engine, "color"));    
    // sad::Label props
    get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>(m_engine, "fontsize"));
    get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::String>(m_engine, "text"));
    get->add(new scripting::AbstractGetter<sad::SceneNode*, float>(m_engine, "linespacing"));
    get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::String>(m_engine, "font"));
	// sad::Sprite2D props
    get->add(new scripting::AbstractGetter<sad::SceneNode*, bool>(m_engine, "flipx"));
    get->add(new scripting::AbstractGetter<sad::SceneNode*, bool>(m_engine, "flipy"));
	get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::String>(m_engine, "options"));
	// sad::db::CustomObject props
	get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::String>(m_engine, "schema"));
#define PUSH_GETTER(TYPE) get->add(scripting::scenenodes::custom_getter< TYPE >(m_engine));
	PUSH_GETTER( double )
	PUSH_GETTER( float )
	PUSH_GETTER( int )
	PUSH_GETTER( long )
	PUSH_GETTER( long long )
	PUSH_GETTER( sad::AColor )
	PUSH_GETTER( sad::Color )
	PUSH_GETTER( sad::Point2D )
	PUSH_GETTER( sad::Point2I )
	PUSH_GETTER( sad::Point3D )
	PUSH_GETTER( sad::Point3I )
	PUSH_GETTER( sad::Size2D )
	PUSH_GETTER( sad::Size2I )
	PUSH_GETTER( sad::Rect2D )
	PUSH_GETTER( sad::Rect2I )
	PUSH_GETTER( sad::String )
	PUSH_GETTER( std::string )
	PUSH_GETTER( QString )
	PUSH_GETTER( short )
	PUSH_GETTER( bool )
	PUSH_GETTER( char )
	PUSH_GETTER( signed char )
	PUSH_GETTER( unsigned char )
	PUSH_GETTER( unsigned int )
	PUSH_GETTER( unsigned long )
	PUSH_GETTER( unsigned long long )
	PUSH_GETTER( unsigned short )
#undef PUSH_GETTER
    m_registered_classes << get;
	scenenodes.setProperty("get", m_engine->newObject(get)); // E.scenes.get

	v.setProperty("scenenodes", scenenodes); // E.scenenodes

	m_engine->evaluate(
		"E.scenenodes.addLabel = function(o) {"  
		"	if (\"fontsize\" in o == false)"
		"   {                              "
		"     o[\"fontsize\"] = 16;        "
		"   }                              "
		"	if (\"color\" in o == false)   "
		"	{"
		"	   o[\"color\"] = aclr(255, 255, 255, 0);"
		"	}"
		"	if (\"name\" in o == false)   "
		"	{"
		"	   o[\"name\"] = \"\";"
		"	}"
		"	return E.scenenodes._addLabel(o[\"scene\"], o[\"font\"], o[\"fontsize\"], o[\"text\"], o[\"name\"], o[\"point\"], o[\"color\"]);"
		"};"
        "E.scenenodes.addSprite2D = function(o) {"
        "	if (\"color\" in o == false)   "
        "	{"
        "	   o[\"color\"] = aclr(255, 255, 255, 0);"
        "	}"
        "	if (\"name\" in o == false)   "
        "	{"
        "	   o[\"name\"] = \"\";"
        "	}"
        "	return E.scenenodes._addSprite2D(o[\"scene\"], o[\"sprite\"], o[\"name\"], o[\"area\"], o[\"color\"]);"
        "};"
        "E.scenenodes.addCustomObject = function(o) {"
        "	if (\"fontsize\" in o == false)"
        "   {                              "
        "     o[\"fontsize\"] = 16;        "
        "   }                              "
        "	if (\"color\" in o == false)   "
        "	{"
        "	   o[\"color\"] = aclr(255, 255, 255, 0);"
        "	}"
        "	if (\"name\" in o == false)   "
        "	{"
        "	   o[\"name\"] = \"\";"
        "	}"
        "	return E.scenenodes._addCustomObject(o[\"scene\"], o[\"schema\"], o[\"name\"], o[\"fontsize\"], o[\"text\"],  o[\"area\"], o[\"color\"]);"
        "};"
		"E.scenenodes.attr = function() {"  
		"	if (arguments.length == 2)"
		"	{"
        "		return E.scenenodes.get(arguments[0], arguments[1]);"
		"	}"
		"	if (arguments.length == 3)"
		"	{"
        "		return E.scenenodes.set(arguments[0], arguments[1], arguments[2]);"
		"	}"
		"	throw new Error(\"Specify 2 or 3 arguments\");"
		"};"
	);
}

void scripting::Scripting::initWaysBindings(QScriptValue& v)
{
    QScriptValue ways = m_engine->newObject();

    ways.setProperty("list", m_engine->newFunction(scripting::ways::list)); // E.ways.list

	scripting::Callable* _add = scripting::make_scripting_call(scripting::ways::_add, this);
	_add->setName("_add");
	m_registered_classes << _add;
	ways.setProperty("_add", m_engine->newObject(_add)); // E.ways._add

	scripting::Callable* remove = scripting::make_scripting_call(scripting::ways::remove, this);
	remove->setName("remove");
	m_registered_classes << remove;
	ways.setProperty("remove", m_engine->newObject(remove)); // E.ways.remove

	scripting::Callable* length = scripting::make_scripting_call(scripting::ways::length, this);
	length->setName("length");
	m_registered_classes << length;
	ways.setProperty("length", m_engine->newObject(length)); // E.ways.length

	scripting::Callable* addPoint = scripting::make_scripting_call(scripting::ways::addPoint, this);
	addPoint->setName("addPoint");
	m_registered_classes << addPoint;
	ways.setProperty("addPoint", m_engine->newObject(addPoint)); // E.ways.addPoint

	scripting::Callable* removePoint = scripting::make_scripting_call(scripting::ways::removePoint, this);
	removePoint->setName("removePoint");
	m_registered_classes << removePoint;
	ways.setProperty("removePoint", m_engine->newObject(removePoint)); // E.ways.removePoint

	scripting::Callable* point = scripting::make_scripting_call(scripting::ways::point, this);
	point->setName("point");
	m_registered_classes << point;
	ways.setProperty("point", m_engine->newObject(point)); // E.ways.point

	scripting::MultiMethod* set = new scripting::MultiMethod(m_engine, "set");
    set->add(new scripting::ways::Setter<sad::String, history::ways::ChangeName>(m_engine, "name"));
	set->add(new scripting::ways::Setter<double, history::ways::ChangeTotalTime>(m_engine, "totaltime"));
	set->add(new scripting::ways::Setter<bool, history::ways::ChangeClosed>(m_engine, "closed"));
    m_registered_classes << set;
	ways.setProperty("set", m_engine->newObject(set)); // E.ways.set


	scripting::MultiMethod* get = new scripting::MultiMethod(m_engine, "get");
    get->add(new scripting::AbstractGetter<sad::p2d::app::Way*, sad::String>(m_engine, "name"));
	get->add(new scripting::AbstractGetter<sad::p2d::app::Way*, unsigned long long>(m_engine, "majorid"));
	get->add(new scripting::AbstractGetter<sad::p2d::app::Way*, unsigned long long>(m_engine, "minorid"));
	get->add(new scripting::AbstractGetter<sad::p2d::app::Way*, double>(m_engine, "totaltime"));
	get->add(new scripting::AbstractGetter<sad::p2d::app::Way*, bool>(m_engine, "closed"));
	m_registered_classes << get;
	ways.setProperty("get", m_engine->newObject(get)); // E.ways.get

    v.setProperty("ways", ways); // E.ways

	m_engine->evaluate(
		"E.ways.add = function(o) {"  
		"   if (typeof o != \"object\")    "
		"   {                              "
		"      o = {};                     "
		"   }                              "
		"	if (\"name\" in o == false)    "
		"   {                              "
		"     o[\"name\"] = \"\";          "
		"   }                              "
		"	if (\"totaltime\" in o == false)   "
		"	{"
		"	   o[\"totaltime\"] = 0;       "
		"	}"
		"	if (\"closed\" in o == false)  "
		"	{"
		"	   o[\"closed\"] = false;      "
		"	}"
		"	if (\"points\" in o == false)  "
		"	{"
		"	   o[\"points\"] = [];         "
		"	}"
		"	return E.ways._add(o[\"name\"], o[\"totaltime\"], o[\"closed\"], o[\"points\"]);"
		"};"
		"E.ways.attr = function() {"  
		"	if (arguments.length == 2)"
		"	{"
        "		return E.ways.get(arguments[0], arguments[1]);"
		"	}"
		"	if (arguments.length == 3)"
		"	{"
        "		return E.ways.set(arguments[0], arguments[1], arguments[2]);"
		"	}"
		"	throw new Error(\"Specify 2 or 3 arguments\");"
		"};"
	);
}


void scripting::Scripting::initDialoguesBindings(QScriptValue& v)
{
	QScriptValue dialogues = m_engine->newObject();

	dialogues.setProperty("list", m_engine->newFunction(scripting::dialogues::list)); // E.dialogues.list

	scripting::Callable* _add = scripting::make_scripting_call(scripting::dialogues::_add, this);
	_add->setName("_add");
	m_registered_classes << _add;
	dialogues.setProperty("_add", m_engine->newObject(_add)); // E.dialogues._add

	scripting::Callable* remove = scripting::make_scripting_call(scripting::dialogues::remove, this);
	remove->setName("remove");
	m_registered_classes << remove;
	dialogues.setProperty("remove", m_engine->newObject(remove)); // E.dialogues.remove

	scripting::Callable* addPhrase = scripting::make_scripting_call(scripting::dialogues::addPhrase, this);
	addPhrase->setName("addPhrase");
	m_registered_classes << addPhrase;
	dialogues.setProperty("addPhrase", m_engine->newObject(addPhrase)); // E.dialogues.addPhrase

	scripting::Callable* removePhrase = scripting::make_scripting_call(scripting::dialogues::removePhrase, this);
	removePhrase->setName("removePhrase");
	m_registered_classes << removePhrase;
	dialogues.setProperty("removePhrase", m_engine->newObject(removePhrase)); // E.dialogues.removePhrase

	scripting::Callable* length = scripting::make_scripting_call(scripting::dialogues::length, this);
	length->setName("length");
	m_registered_classes << length;
	dialogues.setProperty("length", m_engine->newObject(length)); // E.dialogues.length

	scripting::Callable* phrase = scripting::make_scripting_call(scripting::dialogues::phrase, this);
	phrase->setName("phrase");
	m_registered_classes << phrase;
	dialogues.setProperty("phrase", m_engine->newObject(phrase)); // E.dialogues.point

	scripting::MultiMethod* set = new scripting::MultiMethod(m_engine, "set");
    set->add(new scripting::dialogues::Setter<sad::String, history::dialogues::ChangeName>(m_engine, "name"));
	m_registered_classes << set;
	dialogues.setProperty("set", m_engine->newObject(set)); // E.dialogues.set


	scripting::MultiMethod* get = new scripting::MultiMethod(m_engine, "get");
    get->add(new scripting::AbstractGetter<sad::dialogue::Dialogue*, sad::String>(m_engine, "name"));
	get->add(new scripting::AbstractGetter<sad::dialogue::Dialogue*, unsigned long long>(m_engine, "majorid"));
	get->add(new scripting::AbstractGetter<sad::dialogue::Dialogue*, unsigned long long>(m_engine, "minorid"));
	m_registered_classes << get;
	dialogues.setProperty("get", m_engine->newObject(get)); // E.dialogues.get


	v.setProperty("dialogues", dialogues); // E.dialogues

	m_engine->evaluate(
		"var phrase = function(actorName, actorPortrait, text, duration, viewHint) {"  
		"	return {\"actorName\" : actorName, \"actorPortrait\" : actorPortrait, \"text\": text, \"duration\": duration, \"viewHint\" : viewHint};"
		"};"
		"E.dialogues.add = function(o) {"  
		"   if (typeof o != \"object\")    "
		"   {                              "
		"      o = {};                     "
		"   }                              "
		"	if (\"name\" in o == false)    "
		"   {                              "
		"     o[\"name\"] = \"\";          "
		"   }                              "
		"	if (\"phrases\" in o == false) "
		"	{                              "
		"	   o[\"phrases\"] = [];        "
		"	}                              "
		"	return E.dialogues._add(o[\"name\"], o[\"phrases\"]);"
		"};"
		"E.dialogues.attr = function() {"  
		"	if (arguments.length == 2)"
		"	{"
        "		return E.dialogues.get(arguments[0], arguments[1]);"
		"	}"
		"	if (arguments.length == 3)"
		"	{"
        "		return E.dialogues.set(arguments[0], arguments[1], arguments[2]);"
		"	}"
		"	throw new Error(\"Specify 2 or 3 arguments\");"
		"};"
	);
}


void scripting::Scripting::initAnimationsBindings(QScriptValue& v)
{
	QScriptValue animations = m_engine->newObject();
	
	animations.setProperty("list", m_engine->newFunction(scripting::animations::list)); // E.animations.list

	scripting::Callable* _add = scripting::make_scripting_call(scripting::animations::_add, this);
	_add->setName("_add");
	m_registered_classes << _add;
	animations.setProperty("_add", m_engine->newObject(_add)); // E.animations._add

	scripting::Callable* remove = scripting::make_scripting_call(scripting::animations::remove, this);
	remove->setName("remove");
	m_registered_classes << remove;
	animations.setProperty("remove", m_engine->newObject(remove)); // E.animations.remove


	scripting::MultiMethod* set = new scripting::MultiMethod(m_engine, "set");
	set->add(new scripting::animations::Setter<sad::animations::Animation, sad::String, history::animations::ChangeName>(m_engine, "name"));
	set->add(new scripting::animations::Setter<sad::animations::Animation, double, history::animations::ChangeTime>(m_engine, "time"));
	set->add(new scripting::animations::Setter<sad::animations::Animation, bool, history::animations::ChangeLooped>(m_engine, "looped"));
	set->add(new scripting::animations::Setter<sad::animations::Blinking, unsigned int, history::animations::ChangeBlinkingFrequency>(m_engine, "frequency"));
	set->add(new scripting::animations::Setter<sad::animations::CameraShaking, sad::Point2D, history::animations::ChangeCameraOffset>(m_engine, "offset"));
	set->add(new scripting::animations::Setter<sad::animations::CameraShaking, int, history::animations::ChangeShakingFrequency>(m_engine, "frequency"));
	set->add(new scripting::animations::Setter<sad::animations::CameraRotation, sad::Point3D, history::animations::ChangeCameraPivot>(m_engine, "pivot"));
	set->add(new scripting::animations::WidgetSetter<
				sad::animations::CameraRotation, 
				QDoubleSpinBox*,
				double, 
				history::animations::ChangeCameraAngle
			>(m_engine,  m_panel->UI()->dsbCameraRotationStartingAngle, "min_angle")
	);
	set->add(new scripting::animations::WidgetSetter<
				sad::animations::CameraRotation, 
				QDoubleSpinBox*,
				double, 
				history::animations::ChangeCameraAngle
			>(m_engine,  m_panel->UI()->dsbCameraRotationEndingAngle, "max_angle")
	);
	set->add(new scripting::animations::WidgetSetter<
				sad::animations::Color, 
				gui::colorview::ColorView*,
				sad::AColor, 
				history::animations::ChangeColorColor
			>(m_engine, m_panel->UI()->cwColorStartingColor, "min_color")
	);
	set->add(new scripting::animations::WidgetSetter<
				sad::animations::Color, 
				gui::colorview::ColorView*,
				sad::AColor, 
				history::animations::ChangeColorColor
			>(m_engine, m_panel->UI()->cwColorEndingColor, "max_color")
	);
	set->add(new scripting::animations::Setter<sad::animations::FontList, sad::Vector<sad::String>, history::animations::ChangeFontListFonts>(m_engine, "fonts"));
	set->add(new scripting::animations::WidgetSetter<
				sad::animations::FontSize, 
				QSpinBox*,
				unsigned int, 
				history::animations::ChangeFontSizeSize
			>(m_engine, m_panel->UI()->sbFontSizeStartingSize, "min_size")
	);
	set->add(new scripting::animations::WidgetSetter<
				sad::animations::FontSize, 
				QSpinBox*,
				unsigned int, 
				history::animations::ChangeFontSizeSize
			>(m_engine, m_panel->UI()->sbFontSizeEndingSize, "max_size")
	);
	set->add(new scripting::animations::WidgetSetter<
				sad::animations::OptionList, 
				QTextEdit*,
				sad::Vector<sad::String>, 
				history::animations::ChangeList
			>(m_engine, m_panel->UI()->txtOptionListList, "list")
	);
	m_registered_classes << set;
	animations.setProperty("set", m_engine->newObject(set)); // E.scenes.set

	scripting::MultiMethod* get = new scripting::MultiMethod(m_engine, "get");
	get->add(new scripting::AbstractGetter<sad::animations::Animation*, sad::String>(m_engine, "name"));
	get->add(new scripting::AbstractGetter<sad::animations::Animation*, unsigned long long>(m_engine, "majorid"));
	get->add(new scripting::AbstractGetter<sad::animations::Animation*, unsigned long long>(m_engine, "minorid"));
	get->add(new scripting::AbstractGetter<sad::animations::Animation*, double>(m_engine, "time"));
	get->add(new scripting::AbstractGetter<sad::animations::Animation*, bool>(m_engine, "looped"));
	get->add(new scripting::AbstractGetter<sad::animations::Blinking*, unsigned int>(m_engine, "frequency"));	
	get->add(new scripting::AbstractGetter<sad::animations::CameraShaking*, sad::Point2D>(m_engine, "offset"));
	get->add(new scripting::AbstractGetter<sad::animations::CameraShaking*, int>(m_engine, "frequency"));
	get->add(new scripting::AbstractGetter<sad::animations::CameraRotation*, sad::Point3D>(m_engine, "pivot"));
	get->add(new scripting::AbstractGetter<sad::animations::CameraRotation*, double>(m_engine, "min_angle"));
	get->add(new scripting::AbstractGetter<sad::animations::CameraRotation*, double>(m_engine, "max_angle"));	
	get->add(new scripting::AbstractGetter<sad::animations::Color*, sad::AColor>(m_engine, "min_color"));
	get->add(new scripting::AbstractGetter<sad::animations::Color*, sad::AColor>(m_engine, "max_color"));	
	get->add(new scripting::AbstractGetter<sad::animations::FontList*, sad::Vector<sad::String> >(m_engine, "fonts"));	
	get->add(new scripting::AbstractGetter<sad::animations::FontSize*, unsigned int >(m_engine, "min_size"));	
	get->add(new scripting::AbstractGetter<sad::animations::FontSize*, unsigned int >(m_engine, "max_size"));
	get->add(new scripting::AbstractGetter<sad::animations::OptionList*, sad::Vector<sad::String> >(m_engine, "list"));
	
	m_registered_classes << get;
	animations.setProperty("get", m_engine->newObject(get)); // E.scenes.set


	v.setProperty("animations", animations); // E.animations
	
	QString templateanimationadd(
		"E.animations.add{CLASSNAME} = function(o) {"
		"   if (typeof o != \"object\")    "
		"   {                              "
		"      o = {};                     "
		"   }                              "
		"	if (\"name\" in o == false)    "
		"   {                              "
		"     o[\"name\"] = \"\";          "
		"   }                              "
		"	if (\"time\" in o == false)    "
		"	{                              "
		"	   o[\"time\"] = 0;            "
		"	}                              "
		"	if (\"looped\" in o == false)  "
		"	{                              "
		"	   o[\"looped\"] = false;      "
		"	}                              "
		"	return E.animations._add(\"{CLASSNAME}\", o[\"name\"], o[\"time\"], o[\"looped\"]);"
		"};"
	);

	const char* names[] = {
       "Blinking",
       "CameraRotation",
       "CameraShaking",
       "Color",
       "FontList",
       "FontSize",
       "OptionList",
       "Parallel",
       "Resize",
       "Rotate",
       "Sequential",
       "TextureCoordinatesList",
       "TextureCoordinatesContinuous",
       "Typing",
       "WayMoving",
	   NULL
	};
	int i =0;
	while(names[i] != 0) {
		QString kadd = templateanimationadd;
		kadd.replace("{CLASSNAME}",names[i]);
		m_engine->evaluate(kadd);
		++i;
	}
		
	m_engine->evaluate(
		"E.animations.attr = function() {"  
		"	if (arguments.length == 2)"
		"	{"
        "		return E.animations.get(arguments[0], arguments[1]);"
		"	}"
		"	if (arguments.length == 3)"
		"	{"
        "		return E.animations.set(arguments[0], arguments[1], arguments[2]);"
		"	}"
		"	throw new Error(\"Specify 2 or 3 arguments\");"
		"};"
	);
}
void scripting::Scripting::saveScript()
{
	QString name = QFileDialog::getSaveFileName(this->panel(), "Enter file, where we should store source code", "", "*.js");
	if (name.length() != 0)
	{
		QFile file(name);
		if (file.open(QIODevice::WriteOnly))
		{
			QTextStream stream(&file);
			stream << this->panel()->UI()->txtConsoleCode->toPlainText();
		}
		else
		{
			QMessageBox::critical(this->panel(), "Saddy Interface Editor", "Cannot open file " + name);
		}
	}
}

void scripting::Scripting::loadScript()
{
	QString name = QFileDialog::getOpenFileName(this->panel(), "Enter file, where code is stored", "", "*.js");
	if (name.length() != 0)
	{
		QFile file(name);
		if (file.open(QIODevice::ReadOnly))
		{
			QTextStream stream(&file);
			QString string;
			string = stream.readAll();
			this->panel()->UI()->txtConsoleCode->setPlainText(string);
		}
		else
		{
			QMessageBox::critical(this->panel(), "Saddy Interface Editor", "Cannot open file " + name);
		}
	}
}
