#include "scripting.h"
#include "scenebindings.h"
#include "databasebindings.h"
#include "makeconstructor.h"
#include "scriptinglog.h"
#include "multimethod.h"
#include "makescriptingcall.h"
#include "abstractgetter.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "scenes/scenesnamesetter.h"

Q_DECLARE_METATYPE(QScriptContext*)

scripting::Scripting::Scripting(QObject* parent) : QObject(parent), m_panel(NULL)
{
    m_engine = new QScriptEngine();
    QScriptValue v = m_engine->newQObject(this, QScriptEngine::QtOwnership);
    v.setProperty("log", m_engine->newFunction(scripting::scriptinglog));  // E.log 
	
	m_engine->globalObject().setProperty("console", v, QScriptValue::ReadOnly);
    m_engine->globalObject().setProperty("E",v, QScriptValue::ReadOnly);
	
	this->initSadTypeConstructors();
	this->initDatabasePropertyBindings(v);
	this->initSceneBindings(v);
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
		"</ul>"
		"<h3>Editor-specific objects:</h3>\n"
		"<ul>"
		"	<li><b>E</b> or <b>console</b> - a basic class for all operations in editor"
		"	<ul>"
		"		<li>method <b>log</b> - logs all arguments, converting them to string</li>"
		"		<li>property <b>scenes - holds all scene-related operations</b>"
		"			<ul>"
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
		"	</ul>"
		"	</li>"
		"</ul>"
	);
	
	hboxLayout->addWidget(edit);

	dlg.exec();
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
	
	scripting::Callable* add = scripting::make_scripting_call(scripting::addProperty, this);	
	m_registered_classes << add;
	db.setProperty("add", m_engine->newObject(add)); // E.db.add

	v.setProperty("db", db); // E.db
}

void scripting::Scripting::initSceneBindings(QScriptValue& v)
{
	QScriptValue scenes = m_engine->newObject();

	// An add method
	scripting::MultiMethod* add = new scripting::MultiMethod(m_engine, "add");
	add->add(scripting::make_scripting_call(scripting::addScene, this));
	add->add(scripting::make_scripting_call(scripting::addNamelessScene, this));
	m_registered_classes << add;
	
	scenes.setProperty("add", m_engine->newObject(add));  // E.scenes.add
	
	scripting::Callable* remove = scripting::make_scripting_call(scripting::removeScene, this);	
	m_registered_classes << remove;
	scenes.setProperty("remove", m_engine->newObject(remove)); // E.scenes.remove

	scripting::Callable* moveback = scripting::make_scripting_call(scripting::moveSceneBack, this);	
	m_registered_classes << moveback;
	scenes.setProperty("moveBack", m_engine->newObject(moveback)); // E.scenes.moveBack

	scripting::Callable* movefront = scripting::make_scripting_call(scripting::moveSceneFront, this);	
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
