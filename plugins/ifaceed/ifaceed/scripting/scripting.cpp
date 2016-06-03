#include "scripting.h"
#include "constructorcall.h"
#include "makeconstructor.h"
#include "scriptinglog.h"
#include "multimethod.h"
#include "makescriptingcall.h"
#include "makefunctioncall.h"
#include "abstractgetter.h"
#include "queryresource.h"
#include "isaabb.h"
#include "point2d.h"


#include <animations/animationssimplemovement.h>

#include "../scriptinghelp.h"

#include "../core/editor.h"

#include "../gui/codeedit/codeedit.h"

#include "../gui/uiblocks/uiblocks.h"
#include "../gui/uiblocks/uiconsoleblock.h"
#include "../gui/uiblocks/uianimationblock.h"


#include "../history/scenenodes/scenenodeschangename.h"
#include "../history/scenenodes/scenenodeschangeangle.h"
#include "../history/scenenodes/scenenodeschangecolor.h"
#include "../history/scenenodes/scenenodeschangevisibility.h"

#include "../history/label/changetext.h"
#include "../history/label/changelinespacing.h"
#include "../history/label/changemaximallinewidth.h"
#include "../history/label/changebreaktext.h"
#include "../history/label/changeoverflowstrategy.h"
#include "../history/label/changetextellipsis.h"
#include "../history/label/changemaximallinescount.h"
#include "../history/label/changeoverflowstrategyforlines.h"
#include "../history/label/changetextellipsisforlines.h"

#include "../history/sprite2d/changeflipx.h"
#include "../history/sprite2d/changeflipy.h"

#include "../history/ways/wayschangename.h"
#include "../history/ways/wayschangetotaltime.h"
#include "../history/ways/wayschangeclosed.h"

#include "../history/dialogues/dialogueschangename.h"

#include "../history/animations/animationschangename.h"
#include "../history/animations/animationschangetime.h"
#include "../history/animations/animationschangelooped.h"
#include "../history/animations/animationschangeeasingfunctiontype.h"
#include "../history/animations/animationschangeeasingovershootamplitude.h"
#include "../history/animations/animationschangeeasingperiod.h"
#include "../history/animations/animationschangeblinkingfrequency.h"
#include "../history/animations/animationschangecameraoffset.h"
#include "../history/animations/animationschangeshakingfrequency.h"
#include "../history/animations/animationschangecameraangle.h"
#include "../history/animations/animationschangecamerapivot.h"
#include "../history/animations/animationschangecolorcolor.h"
#include "../history/animations/animationschangefontlistfonts.h"
#include "../history/animations/animationschangefontsizesize.h"
#include "../history/animations/animationschangelist.h"
#include "../history/animations/animationschangeresizestartingsize.h"
#include "../history/animations/animationschangeresizeendingsize.h"
#include "../history/animations/animationschangerotateangle.h"
#include "../history/animations/animationschangerect.h"
#include "../history/animations/animationschangepropertyaspoint2displayedintwospinboxes.h"


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

#include "layouts/gridbindings.h"

#include "ways/waysbindings.h"
#include "ways/wayssetter.h"

#include "dialogues/dialoguesbindings.h"
#include "dialogues/dialoguessetter.h"

#include "animations/animationsbindings.h"
#include "animations/animationspoint2dsetter.h"
#include "animations/animationssetter.h"
#include "animations/animationswidgetsetter.h"
#include "animations/animationswaysetter.h"

#include "instances/instancesbindings.h"
#include "instances/instancesnamesetter.h"
#include "instances/instancesanimationsetter.h"
#include "instances/instancesanimationdbsetter.h"
#include "instances/instancesobjectsetter.h"
#include "instances/instancesstarttimesetter.h"
#include "instances/instanceswaysetter.h"

#include "groups/groupsbindings.h"
#include "groups/groupsnamesetter.h"
#include "groups/groupsloopedsetter.h"
#include "groups/groupssequentialsetter.h"

#include <QFileDialog>
#include <QTextStream>
#include <QScriptValueIterator>

#include <animations/animationsblinking.h>
#include <animations/animationscamerashaking.h>
#include <animations/animationscamerarotation.h>
#include <animations/animationscomposite.h>
#include <animations/animationscolor.h>
#include <animations/animationsfontlist.h>
#include <animations/animationsfontsize.h>
#include <animations/animationsresize.h>
#include <animations/animationsrotate.h>
#include <animations/animationsoptionlist.h>
#include <animations/animationstexturecoordinatescontinuous.h>
#include <animations/animationstexturecoordinateslist.h>
#include <animations/animationswaymoving.h>
#include "animations/easinggetter.h"
#include "animations/easingsetter.h"

Q_DECLARE_METATYPE(QScriptContext*) //-V566

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
scripting::Scripting::Scripting(QObject* parent) : QObject(parent), m_editor(NULL)
{
    m_flags = QScriptValue::ReadOnly|QScriptValue::Undeletable;
    m_engine = new QScriptEngine();
    m_value = m_engine->newQObject(this, QScriptEngine::QtOwnership, QScriptEngine::SkipMethodsInEnumeration);
    m_value.setProperty("log", m_engine->newFunction(scripting::scripting_log), m_flags);  // E.log
    
    QScriptValue globalValue = m_engine->globalObject();
    globalValue.setProperty("console", m_value, m_flags);
    globalValue.setProperty("E",m_value,m_flags);
    globalValue.setProperty("---",m_value,m_flags);
    
    scripting::Callable* oresourcetype = scripting::make_scripting_call(scripting::resource_type, this);
    m_registered_classes << oresourcetype;
    m_value.setProperty("resourceType", m_engine->newObject(oresourcetype), m_flags);

    scripting::Callable* oresourceoptions = scripting::make_scripting_call(scripting::resource_options, this);
    m_registered_classes << oresourceoptions;
    m_value.setProperty("resourceOptions",m_engine->newObject(oresourceoptions), m_flags);

    scripting::Callable* oresourceschema = scripting::make_scripting_call(scripting::resource_schema, this);
    m_registered_classes << oresourceschema;
    m_value.setProperty("resourceSchema", m_engine->newObject(oresourceschema), m_flags);    
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

void scripting::Scripting::setEditor(core::Editor* editor)
{
    m_editor = editor;
    this->initSadTypeConstructors();
    this->initDatabasePropertyBindings(m_value);
    this->initSceneBindings(m_value);
    this->initSceneNodesBindings(m_value);
    this->initLayoutGridBindings(m_value);
    this->initWaysBindings(m_value);
    this->initDialoguesBindings(m_value);
    this->initAnimationsBindings(m_value);
    this->initAnimationInstanceBindings(m_value);
    this->initAnimationGroupBindings(m_value);
}

core::Editor* scripting::Scripting::editor() const
{
    return m_editor;
}

QScriptEngine* scripting::Scripting::engine() const
{
    return m_engine;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void scripting::Scripting::registerFunction(const QString& name, QScriptValue& v)
{
    v.setProperty("name", name);
    m_engine->globalObject().setProperty(name, v, m_flags);
}

void scripting::Scripting::registerScriptClass(const QString& name, QScriptClass* c)
{
    m_engine->globalObject().setProperty(name, m_engine->newObject(c), m_flags);
    if (m_registered_classes.contains(c) == false) {
        m_registered_classes << c;
    }
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
QSet<QString> scripting::Scripting::commonProperties()
{
    QSet<QString> result;
    const char* firstprops[] = {
        "position",
        "actorName",
        "actorPortrait",
        "text",
        "duration",
        "viewHint",
        "setPoint",
        "point",
        "width",
        "height",
        "x",
        "y",
        "sub",
        "sum",
        "mul",
        "div",
        "mid",
        "movedToPoint",
        NULL
    };
    int i = 0;
    while(firstprops[i] != NULL)
    {
        result.insert(firstprops[i]);
        ++i;
    }
    return result;
}

void scripting::Scripting::propertiesAndFunctions(
    QStringList& properties,
    QStringList& functions
)
{
    QSet<QString> propertiesset = this->commonProperties();
    QSet<QString> functionsset;

    propertiesAndFunctions(propertiesset, functionsset, m_engine->globalObject());

    properties = propertiesset.toList();
    functions = functionsset.toList();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void scripting::Scripting::propertiesAndFunctions(
    QSet<QString>& properties,
    QSet<QString>& functions,
    const QScriptValue& v
)
{
    QScriptValueIterator it(v);
    while(it.hasNext())
    {
        it.next();
        if (it.name() != "prototype" && it.name() != "__prototype__" && it.name() != "constructor")
        {
           if (it.value().isFunction() || it.value().scriptClass() != NULL)
           {
               functions.insert(it.name());
           }
           else
           {
               properties.insert(it.name());
               this->propertiesAndFunctions(properties, functions, it.value());
           }
        }
    }
}


int scripting::Scripting::screenWidth()
{
    return sad::Renderer::ref()->settings().width();
}

int scripting::Scripting::screenHeight()
{
    return sad::Renderer::ref()->settings().height();
}


void scripting::Scripting::runScript()
{
    if (m_engine->isEvaluating())
    {
        return;
    }
    history::BatchCommand* c = new history::BatchCommand();
    m_editor->setCurrentBatchCommand(c);

    gui::uiblocks::UIConsoleBlock* cblk = m_editor->uiBlocks()->uiConsoleBlock();
    cblk->txtConsoleResults->setText("");
    QString text = cblk->txtConsoleCode->toPlainText();
    
    QScriptValue globalValue = m_engine->globalObject();
    globalValue.setProperty("console", m_value, m_flags);
    globalValue.setProperty("E",m_value,m_flags);
    globalValue.setProperty("---",m_value,m_flags);
    
    scripting::Scripting::Thread poller(this);
    poller.start();
    QScriptValue result = m_engine->evaluate(text, "console.js");
    poller.forceQuit();

    if (result.isError())
    {
        cblk->txtConsoleResults->append(QString("<font color=\"red\">")
                                        + result.toString()
                                        + QString("<br/>Backtrace:<br/>")
                                        + m_engine->uncaughtExceptionBacktrace().join("<br/>")
                                        + QString("</font>")
        );
        c->rollback(m_editor);
        delete c;
    }
    else
    {
        if (c->count())
        {
            m_editor->history()->add(c);
        }
        else
        {
            delete c;
        }
    }

    m_editor->setCurrentBatchCommand(NULL);
    poller.wait();
}


// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void scripting::Scripting::showHelp()
{
    ScriptingHelp dlg;
            
    dlg.setText(
        "<h2>Scripting with IFace Editor</h2>\n"
        "<h3>Common classes:</h3>\n"
        "<ul>"
        "<li><b>sad::Point2D</b> - a basic 2D point with coordinates as double"
        "    <ul>"
        "        <li>constructor <b>p2d(), p2d(2,3)</b> - constructs a point with specified parameters ( (0,0) in first case) </li>"
        "        <li>property <b>x</b> - stores x coordinate</li>"
        "        <li>property <b>y</b> - stores y coordinate</li>"
        "        <li>method <b>sum(point)</b> - returns sum of two points</li>"
        "        <li>method <b>sub(point)</b> - returns substruction of two points</li>"
        "        <li>method <b>mid(point)</b> - returns middle point between two points</li>"
        "        <li>method <b>mul(number)</b> - returns point with coordinates, multiplicated by number</li>"
        "        <li>method <b>div(number)</b> - returns point with coordinates, divided by number</li>"
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
        "        <li>method <b>movedToPoint(point)</b> - returns copy of rectangle with center at point. Only works, if rectangle is axis-aligned.</li>"
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
        "		<li>method <b>screenWidth</b> - returns width of screen window</li>"
        "		<li>method <b>screenHeight</b> - returns height of screen window</li>"
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
        "				<li>method <b>_addLabel(name of scene, font name, font size, text, name, position as point, color)</b> - adds a label to a scene</li>"
        "				<li>method <b>addLabel(object)</b> - does the same as previous only style is different. Object must have fields:  \"font\", \"text\", \"point\", \"scene\" . Fields \"fontsize\", \"color\", \"name\" are optional.</li>"
        "				<li>method <b>_addSprite2D(name of scene, sprite options name, name, area as rectangle, color)</b> - adds a sprite to a scene</li>"
        "				<li>method <b>addSprite2D(object)</b> - does the same as previous only style is different. Object must have fields:  \"sprite\", \"area\", \"scene\" . Fields \"color\", \"name\" are optional.</li>"
        "				<li>method <b>_addCustomObject(name of scene, schema name, name, font size, text, area as rectangle, color)</b> - adds a custom object to a scene</li>"
        "				<li>method <b>addCustomObject(object)</b> - does the same as previous only style is different. Object must have fields:  \"schema\", \"fontsize\", \"scene\", \"text\", \"area\" . Fields \"fontsize\", \"color\", \"name\" are optional.</li>"
        "				<li>method <b>makeBackground(scene node)</b> - sets nodes as background. May cause labels to disappear</li>"
        "				<li>method <b>remove(22)</b>, <b>remove(\"name\")</b> - removes scene node by id or by name</li>"
        "				<li>method <b>moveBack(22)</b>, <b>moveBack(\"name\")</b> - moves scene back in list by id or by name</li>"
        "				<li>method <b>moveFront(22)</b>, <b>moveFront(\"name\")</b> - moves scene front in list by id or by name</li>"
        "				<li>method <b>spanBetweenTwoPoints(scene_node_name_or_id, first_point, second_point)</b> - makes object span between two points. Object is specified by name or id and points can be created, using p2d</li>"
        "				<li>method <b>set(\"nodename\", \"propertyname\", \"value\")</b> - sets property of scene node."
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
        "						<li><b>[Label or Custom Object]</b>property <b>\"maximallinewidth\"</b>  - a maximal line width in characters. Can be used to constrain line width for label and split it into lines.</li>"
        "						<li><b>[Label or Custom Object]</b>property <b>\"overflowstrategy\"</b>  - what we should do in case that line width exceeds a maximal value. Could have following values: E.OverflowStrategy.Visible, E.OverflowStrategy.Hidden, E.OverflowStrategy.Ellipsis.</li>"
        "						<li><b>[Label or Custom Object]</b>property <b>\"breaktext\"</b>  - whether we should break text into lines in case, when maximal line width exceeded. Can be either E.BreakText.Normal or E.BreakText.BreakWord.</li>"
        "						<li><b>[Label or Custom Object]</b>property <b>\"textellipsisposition\"</b>  - a position of suspension point in line. Can be E.TextEllipsisPosition.Begin, E.TextEllipsisPosition.Middle, E.TextEllipsisPosition.End.</li>"
        "						<li><b>[Label or Custom Object]</b>property <b>\"maximallinescount\"</b>  - a maximal lines, that should be displayed in label.</li>"
        "						<li><b>[Label or Custom Object]</b>property <b>\"overflowstrategyforlines\"</b>  - what we should do in case that  amount of lines width exceeds a maximal value. Could have following values: E.OverflowStrategy.Visible, E.OverflowStrategy.Hidden, E.OverflowStrategy.Ellipsis.</li>"
        "						<li><b>[Label or Custom Object]</b>property <b>\"textellipsispositionforlines\"</b>  - a position of suspension point in text. Can be E.TextEllipsisPosition.Begin, E.TextEllipsisPosition.Middle, E.TextEllipsisPosition.End.</li>"
        "						<li><b>[Sprite or Custom Object]</b>property <b>\"flipx\"</b>  - a flag, whether sprite is flipped by horizontal.</li>"
        "						<li><b>[Sprite or Custom Object]</b>property <b>\"flipy\"</b>  - a flag, whether sprite is flipped by vertical.</li>"
        "						<li><b>[Sprite or Custom Object]</b>property <b>\"options\"</b>  - a sprite options resource name.</li>"
        "						<li><b>[Custom Object]</b>property <b>\"schema\"</b>  - a schema resource name.</li>"
        "						<li><b>[Custom Object] properties of custom objects.</li>"
        "					</ul>"
        "				</li>"		
        "				<li>method <b>get(\"nodename\", \"propertyname\", \"value\")</b> - fetches property of scene node by it\'s name"
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
        "						<li><b>[Label or Custom Object]</b>property <b>\"maximallinewidth\"</b>  - a maximal line width in characters. Can be used to constrain line width for label and split it into lines..</li>"
        "						<li><b>[Label or Custom Object]</b>property <b>\"overflowstrategy\"</b>  - what we should do in case that line width exceeds a maximal value. Could have following values: E.OverflowStrategy.Visible, E.OverflowStrategy.Hidden, E.OverflowStrategy.Ellipsis.</li>"
        "						<li><b>[Label or Custom Object]</b>property <b>\"breaktext\"</b>  - whether we should break text into lines in case, when maximal line width exceeded. Can be either E.BreakText.Normal or E.BreakText.BreakWord.</li>"
        "						<li><b>[Label or Custom Object]</b>property <b>\"textellipsisposition\"</b>  - a position of suspension point in line. Can be E.TextEllipsisPosition.Begin, E.TextEllipsisPosition.Middle, E.TextEllipsisPosition.End.</li>"
        "						<li><b>[Label or Custom Object]</b>property <b>\"maximallinescount\"</b>  - a maximal lines, that should be displayed in label.</li>"
        "						<li><b>[Label or Custom Object]</b>property <b>\"overflowstrategyforlines\"</b>  - what we should do in case that  amount of lines width exceeds a maximal value. Could have following values: E.OverflowStrategy.Visible, E.OverflowStrategy.Hidden, E.OverflowStrategy.Ellipsis.</li>"
        "						<li><b>[Label or Custom Object]</b>property <b>\"textellipsispositionforlines\"</b>  - a position of suspension point in text. Can be E.TextEllipsisPosition.Begin, E.TextEllipsisPosition.Middle, E.TextEllipsisPosition.End.</li>"
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
        "				<li>method <b>set(\"wayname\", \"propertyname\", \"value\")</b> - sets property of way."
        "					<ul>"	
        "						<li>property <b>\"name\"</b>  - name as string</li>"
        "						<li>property <b>\"totaltime\"</b>  - a total time for a way.</li>"
        "						<li>property <b>\"closed\"</b>  - whether way is closed.</li>"		
        "					</ul>"
        "				</li>"		
        "				<li>method <b>get(\"wayname\", \"propertyname\", \"value\")</b> - fetches property of way by it\'s name"
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
        "				<li>method <b>set(\"wayname\", \"propertyname\", \"value\")</b> - sets property of dialogue."
        "					<ul>"	
        "						<li>property <b>\"name\"</b>  - name as string</li>"
        "					</ul>"
        "				</li>"		
        "				<li>method <b>get(\"wayname\", \"propertyname\", \"value\")</b> - fetches property of dialogue by it\'s name"
        "					<ul>"
        "						<li>property <b>\"name\"</b>  - name as string</li>"
        "						<li>property <b>\"majorid\"</b>  - a major id of scene in database. Useful for links.</li>"
        "						<li>property <b>\"minorid\"</b>  - a minor id of scene in database. Useful for links in your application.</li>"
        "					</ul>"
        "				</li>"
        "				<li>method <b>attr</b> - depending from number of arguments applies <b>set</b> or <b>get</b> methods respectively</li>"
        "			</ul>"
        "		</li>"
        "		<li>property <b>animations</b> - holds all operations related to animations"
        "			<ul>"
        "				<li>method <b>list()</b> - lists animations, returning array of their major ids</li>"
        "				<li>method <b>_add(\"type\", \"name\", time, looped)</b> - adds specified type (like in combo box) of animation into editor, using specified name, time and flag, whether it's looped</li>"
        "				<li>method <b>addBlinking(object)</b> - adds animation of type Blinking. Fields of object, like \"name\", \"time\", \"looped\"</li>"
        "				<li>method <b>addCameraRotation(object)</b> - adds animation of type CameraRotation. Fields of object, like \"name\", \"time\", \"looped\"</li>"
        "				<li>method <b>addCameraShaking(object)</b> - adds animation of type CameraShaking. Fields of object, like \"name\", \"time\", \"looped\"</li>"
        "				<li>method <b>addColor(object)</b> - adds animation of type Color. Fields of object, like \"name\", \"time\", \"looped\"</li>"
        "				<li>method <b>addFontList(object)</b> - adds animation of type FontList. Fields of object, like \"name\", \"time\", \"looped\"</li>"
        "				<li>method <b>addFontSize(object)</b> - adds animation of type FontSize. Fields of object, like \"name\", \"time\", \"looped\"</li>"
        "				<li>method <b>addOptionList(object)</b> - adds animation of type OptionList. Fields of object, like \"name\", \"time\", \"looped\"</li>"
        "				<li>method <b>addParallel(object)</b> - adds animation of type Parallel. Fields of object, like \"name\", \"time\", \"looped\"</li>"
        "				<li>method <b>addResize(object)</b> - adds animation of type Resize. Fields of object, like \"name\", \"time\", \"looped\"</li>"
        "				<li>method <b>addRotate(object)</b> - adds animation of type Rotate. Fields of object, like \"name\", \"time\", \"looped\"</li>"
        "				<li>method <b>addSimpleMovement(object)</b> - adds animation of type SimpleMovement. Fields of object, like \"name\", \"time\", \"looped\",\"start_point\", \"end_point\"</li>"		
        "				<li>method <b>addSequential(object)</b> - adds animation of type Sequential. Fields of object, like \"name\", \"time\", \"looped\"</li>"
        "				<li>method <b>addTextureCoordinatesList(object)</b> - adds animation of type TextureCoordinatesList. Fields of object, like \"name\", \"time\", \"looped\"</li>"
        "				<li>method <b>addTextureCoordinatesContinuous(object)</b> - adds animation of type TextureCoordinatesContinuous. Fields of object, like \"name\", \"time\", \"looped\"</li>"
        "				<li>method <b>addTyping(object)</b> - adds animation of type Typing. Fields of object, like \"name\", \"time\", \"looped\"</li>"
        "				<li>method <b>addWayMoving(object)</b> - adds animation of type WayMoving. Fields of object, like \"name\", \"time\", \"looped\"</li>"
        "				<li>method <b>remove(22)</b>, <b>remove(\"name\")</b> - removes animation by id or by name</li>"
        "				<li>method <b>addToComposite(composite animation, animation)</b> - adds animation to composite animation's list (composite is Parallel or Sequential animation). Returns true on success, false on failure</li>"
        "				<li>method <b>removeFromComposite(composite animation, animation)</b> - removes animation from composite animation's list (composite is Parallel or Sequential animation). Returns true on success, false if animation is not exists</li>"
        "				<li>method <b>compositeLength(composite animation)</b> -  returns count of animations in composite animation's list (composite is Parallel or Sequential animation).</li>"
        "				<li>method <b>getAnimation(composite animation, position)</b> -  returns animation as major id from composite animation's list, specified by pos (composite is Parallel or Sequential animation). Returns 0, if position is not valid </li>"
        "				<li>method <b>moveBackInCompositeList(composite animation, position)</b> -  moves animation back in composite animation's list. Returns true on success, false on error </li>"
        "				<li>method <b>moveFrontInCompositeList(composite animation, position)</b> -  moves animation front in composite animation's list. Returns true on success, false on error </li>"
        "				<li>method <b>set(\"animationname\", \"propertyname\", \"value\")</b> - sets property of animation. "
        "					<ul>"
        "						<li><b>[All animation types]</b> property <b>\"name\"</b>  - name as string</li>"
        "						<li><b>[All animation types]</b> property <b>\"time\"</b>  - a time, how long animation should be played.</li>"
        "						<li><b>[All animation types]</b> property <b>\"looped\"</b>  - a flag, whether animation is looped.</li>"
        "						<li><b>[All animation types]</b> property <b>\"easing_type\"</b>  - a type of easing function as numerical value. Use <b>E.animations.easing</b> function to convert between string representation and number</li>"
        "						<li><b>[All animation types]</b> property <b>\"easing_overshoot_amplitude\"</b>  - an overshoot amplitude for easing function</li>"
        "						<li><b>[All animation types]</b> property <b>\"easing_period\"</b>  - a period for easing function</li>"
        "						<li><b>[Blinking]</b> property <b>\"frequency\"</b>  - a frequency, how much times should state of visibility of object change.</li>"
        "						<li><b>[CameraShaking]</b> property <b>\"offset\"</b>  - a maximal offset of camera during animation.</li>"
        "						<li><b>[CameraShaking]</b> property <b>\"frequency\"</b>  - a maximal offset of camera during animation.</li>"
        "						<li><b>[CameraRotation]</b> property <b>\"pivot\"</b>  - a pivot point, which should camera turn around.</li>"
        "						<li><b>[CameraRotation]</b> property <b>\"min_angle\"</b>  - a starting angle for camera rotation.</li>"
        "						<li><b>[CameraRotation]</b> property <b>\"max_angle\"</b>  - an ending angle for camera rotation.</li>"
        "						<li><b>[Color]</b> property <b>\"min_color\"</b>  - a starting color (sad::AColor) for color changing animation.</li>"
        "						<li><b>[Color]</b> property <b>\"max_color\"</b>  - an ending color (sad::AColor) for color changing animation.</li>"
        "						<li><b>[FontList]</b> property <b>\"fonts\"</b>  - an array of strings, which represents links to font resources.</li>"
        "						<li><b>[FontSize]</b> property <b>\"min_size\"</b>  - a starting size for size animation.</li>"
        "						<li><b>[FontSize]</b> property <b>\"max_size\"</b>  - an  ending size for size animation.</li>"
        "						<li><b>[Resize]</b> property <b>\"start_size\"</b>  - a starting size for resize animation.</li>"
        "						<li><b>[Resize]</b> property <b>\"end_size\"</b>    - an ending size for resize animation.</li>"
        "						<li><b>[SimpleMovement]</b> property <b>\"start_point\"</b>    - a starting point for simple movement animation.</li>"
        "						<li><b>[SimpleMovement]</b> property <b>\"end_point\"</b>    - an ending point for simple movement animation.</li>"        
        "						<li><b>[Rotate]</b> property <b>\"min_angle\"</b>  - a starting angle for rotation.</li>"
        "						<li><b>[Rotate]</b> property <b>\"max_angle\"</b>  - an ending angle for rotation.</li>"
        "						<li><b>[OptionList of TextureCoordinatesList]</b> property <b>\"list\"</b>  - an array of strings, which represents links to sprite options resources.</li>"
        "						<li><b>[TextureCoordinatesContinuous]</b> property <b>\"start_rect\"</b>  - a starting rectangle (sad::Rect2D) for animation.</li>"
        "						<li><b>[TextureCoordinatesContinuous]</b> property <b>\"end_rect\"</b>  - an engine rectangle (sad::Rect2D) for animation.</li>"
        "						<li><b>[WayMoving]</b> property <b>\"way_moving\"</b>  - a major id for a way.</li>"
        "					</ul>"
        "				</li>"
        "				<li>method <b>get(\"animationname\", \"propertyname\")</b> - fetches property of animation by it\'s name"
        "					<ul>"
        "						<li><b>[All animation types]</b> property <b>\"name\"</b>  - name as string</li>"
        "						<li><b>[All animation types]</b> property <b>\"majorid\"</b>  - a major id of scene in database. Useful for links.</li>"
        "						<li><b>[All animation types]</b> property <b>\"minorid\"</b>  - a minor id of scene in database. Useful for links in your application.</li>"
        "						<li><b>[All animation types]</b> property <b>\"time\"</b>  - a time, how long animation should be played.</li>"
        "						<li><b>[All animation types]</b> property <b>\"looped\"</b>  - a flag, whether animation is looped.</li>"
        "						<li><b>[All animation types]</b> property <b>\"easing_type\"</b>  - a type of easing function as numerical value. Use <b>E.animations.easing</b> function to convert between string representation and number</li>"
        "						<li><b>[All animation types]</b> property <b>\"easing_overshoot_amplitude\"</b>  - an overshoot amplitude for easing function</li>"
        "						<li><b>[All animation types]</b> property <b>\"easing_period\"</b>  - a period for easing function</li>"
        "						<li><b>[Blinking]</b> property <b>\"frequency\"</b>  - a frequency, how much times should state of visibility of object change.</li>"
        "						<li><b>[CameraShaking]</b> property <b>\"offset\"</b>  - a maximal offset of camera during animation.</li>"
        "						<li><b>[CameraShaking]</b> property <b>\"frequency\"</b>  - a maximal offset of camera during animation.</li>"
        "						<li><b>[CameraRotation]</b> property <b>\"pivot\"</b>  - a pivot point, which should camera turn around.</li>"
        "						<li><b>[CameraRotation]</b> property <b>\"min_angle\"</b>  - a starting angle for camera rotation.</li>"
        "						<li><b>[CameraRotation]</b> property <b>\"max_angle\"</b>  - an ending angle for camera rotation.</li>"
        "						<li><b>[Color]</b> property <b>\"min_color\"</b>  - a starting color (sad::AColor) for color changing animation.</li>"
        "						<li><b>[Color]</b> property <b>\"max_color\"</b>  - an ending color (sad::AColor) for color changing animation.</li>"
        "						<li><b>[FontList]</b> property <b>\"fonts\"</b>  - an array of strings, which represents links to font resources.</li>"
        "						<li><b>[FontSize]</b> property <b>\"min_size\"</b>  - a starting size for size animation.</li>"
        "						<li><b>[FontSize]</b> property <b>\"max_size\"</b>  - an  ending size for size animation.</li>"
        "						<li><b>[Resize]</b> property <b>\"start_size\"</b>  - a starting size for resize animation for object.</li>"
        "						<li><b>[Resize]</b> property <b>\"end_size\"</b>  - an ending size for resize animation for object.</li>"       
        "						<li><b>[SimpleMovement]</b> property <b>\"start_point\"</b>    - a starting point for simple movement animation.</li>"
        "						<li><b>[SimpleMovement]</b> property <b>\"end_point\"</b>    - an ending point for simple movement animation.</li>"        
        "						<li><b>[Rotate]</b> property <b>\"min_angle\"</b>  - a starting angle for rotation.</li>"
        "						<li><b>[Rotate]</b> property <b>\"max_angle\"</b>  - an ending angle for rotation.</li>"
        "						<li><b>[OptionList of TextureCoordinatesList]</b> property <b>\"list\"</b>  - an array of strings, which represents links to sprite options resources.</li>"
        "						<li><b>[TextureCoordinatesContinuous]</b> property <b>\"start_rect\"</b>  - a starting rectangle (sad::Rect2D) for animation.</li>"
        "						<li><b>[TextureCoordinatesContinuous]</b> property <b>\"end_rect\"</b>  - an engine rectangle (sad::Rect2D) for animation.</li>"
        "						<li><b>[WayMoving]</b> property <b>\"way_moving\"</b>  - a major id for a way.</li>"
        "						<li><b>[Parallel or Sequential]</b> property <b>\"list\"</b>  - a list of major ids, which are in this animation.</li>"
        "					</ul>"
        "				</li>"
        "				<li>method <b>attr</b> - depending from number of arguments applies <b>set</b> or <b>get</b> methods respectively</li>"
        "               <li>property <b>instances</b> - holds all operations related to animation instances"
        "                   <ul>"
        "						<li>method <b>list()</b> - lists all animation instances, returning all of majorids for it</li>"
        "						<li>method <b>_addInstance(name, reference to animation from database as number, reference to animation from tree as string, reference to object, starting time)</b> - adds new instance. id of animation from database take priority if it valid.</li>"
        "						<li>method <b>addInstance(object)</b> - does the same as previous only style is different and animation fields can be used as both references. Fields \"name\",  \"animation\", \"object\", \"starttime\" are optional.</li>"
        "						<li>method <b>_addWayInstance(name, reference to way, reference to object, starting time)</b> - adds new way instance.</li>"
        "						<li>method <b>addWayInstance(object)</b> - does the same as previous only style is different. Fields \"name\",  \"way\", \"object\", \"starttime\" are optional.</li>"		
        "						<li>method <b>remove(22)</b>, <b>remove(\"name\")</b> - removes instance by id or by name</li>"		
        "						<li>method <b>set(\"animation instance name\", \"propertyname\", \"value\")</b> - sets property of animation instance."
        "							<ul>"	
        "								<li><b>[All types]</b>property <b>\"name\"</b>  - name as string</li>"
        "								<li><b>[All types]</b>property <b>\"animation\"</b>  - animation reference to an item from tree as string</li>"
        "								<li><b>[All types]</b>property <b>\"animationmajorid\"</b>  - animation reference to an item from database as number</li>"
        "								<li><b>[All types]</b>property <b>\"object\"</b>  - a reference to object as number</li>"
        "								<li><b>[All types]</b>property <b>\"starttime\"</b>  - a time, which is noted to animation, when instance starts as double</li>"
        "								<li><b>[WayInstance]</b>property <b>\"way\"</b>  - a reference to way as number</li>"		
        "							</ul>"
        "						</li>"		
        "						<li>method <b>get(\"animation instance name\", \"propertyname\", \"value\")</b> - fetches property of animation instance by it\'s name"
        "							<ul>"
        "								<li><b>[All types]</b>property <b>\"name\"</b>  - name as string</li>"
        "								<li><b>[All types]</b>property <b>\"majorid\"</b>  - a major id of animation in database. Useful for links.</li>"
        "								<li><b>[All types]</b>property <b>\"minorid\"</b>  - a minor id of animation in database. Useful for links in your application.</li>"
        "								<li><b>[All types]</b>property <b>\"name\"</b>  - name as string</li>"
        "								<li><b>[All types]</b>property <b>\"animation\"</b>  - animation reference to an item from tree as string</li>"
        "								<li><b>[All types]</b>property <b>\"animationmajorid\"</b>  - animation reference to an item from database as number</li>"
        "								<li><b>[All types]</b>property <b>\"object\"</b>  - a reference to object as number</li>"
        "								<li><b>[All types]</b>property <b>\"starttime\"</b>  - a time, which is noted to animation, when instance starts as double</li>"
        "								<li><b>[WayInstance]</b>property <b>\"way\"</b>  - a reference to way as number</li>"
        "							</ul>"
        "						</li>"
        "						<li>method <b>attr</b> - depending from number of arguments applies <b>set</b> or <b>get</b> methods respectively</li>"
        "                   </ul>"
        "               </li>"
        "               <li>property <b>groups</b> - holds all operations related to animation groups"
        "                   <ul>"
        "						<li>method <b>list()</b> - lists all animation groups, returning all of majorids for it</li>"
        "						<li>method <b>_add(name, whether it\'s looped)</b> - adds new group with specified name and flag, whether it\'s looped.</li>"
        "						<li>method <b>add(object)</b> - does the same as previous only style is different and animation fields can be used as both references. Fields \"name\",  \"looped\" are optional.</li>"
        "						<li>method <b>remove(22)</b>, <b>remove(\"name\")</b> - removes group by id or by name</li>"		
        "						<li>method <b>length(22)</b>, <b>length(\"name\")</b> - returns count of animation instances in group</li>"		
        "						<li>method <b>entry(id, position)</b>, <b>entry(\"name\", position)</b> - returns animation instace by it\'s position in group. 0 is returned if position is invalid</li>"		
        "						<li>method <b>addInstance(group, instance)</b> - adds instance to group. Returns true on success, false if instance is already in list</li>"		
        "						<li>method <b>removeInstance(group, position)</b> - removes instance, marked by position in a group from parent group. Returns true if position is valid, false if not</li>"		
        "						<li>method <b>set(\"group name\", \"propertyname\", \"value\")</b> - sets property of animation group."
        "							<ul>"	
        "								<li>property <b>\"name\"</b>  - name as string</li>"
        "								<li>property <b>\"looped\"</b>  - flag, which indicates, whether group is looped</li>"	
        "								<li>property <b>\"sequential\"</b>  - flag, which indicates, whether instances in group should be run sequentially or parallel</li>"	
        "							</ul>"
        "						</li>"		
        "						<li>method <b>get(\"group name\", \"propertyname\")</b> - fetches property of animation group by it\'s name"
        "							<ul>"
        "								<li>property <b>\"name\"</b>  - name as string</li>"
        "								<li>property <b>\"majorid\"</b>  - a major id of animation in database. Useful for links.</li>"
        "								<li>property <b>\"minorid\"</b>  - a minor id of animation in database. Useful for links in your application.</li>"
        "								<li>property <b>\"looped\"</b>  - flag, which indicates, whether group is looped</li>"	
        "								<li>property <b>\"sequential\"</b>  - flag, which indicates, whether instances in group should be run sequentially or parallel</li>"	
        "								<li>property <b>\"instances\"</b>  - list of major ids of animation instances in group</li>"	
        "							</ul>"
        "						</li>"
        "						<li>method <b>attr</b> - depending from number of arguments applies <b>set</b> or <b>get</b> methods respectively</li>"		
        "                   </ul>"
        "               </li>"
        "			</ul>"
        "		</li>"
        "	</ul>"
        "	</li>"
        "</ul>"
    );
    
    dlg.exec();
}


// ReSharper disable once CppMemberFunctionMayBeConst
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

    qScriptRegisterMetaType(m_engine, scripting::Point2D::toValue, scripting::Point2D::fromValue);

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

    scripting::Callable* screenWidth = scripting::make_function_call(scripting::Scripting::screenWidth, this);
    screenWidth->setName("screenWidth");
    m_registered_classes << screenWidth;
    m_value.setProperty("screenWidth", m_engine->newObject(screenWidth), m_flags); // E.screenWidth

    scripting::Callable* screenHeight = scripting::make_function_call(scripting::Scripting::screenHeight, this);
    screenHeight->setName("screenHeight");
    m_registered_classes << screenHeight;
    m_value.setProperty("screenHeight", m_engine->newObject(screenHeight), m_flags); // E.screenHeight
}

void scripting::Scripting::initDatabasePropertyBindings(QScriptValue& v)
{
    QScriptValue db = m_engine->newObject();
    
    db.setProperty("list", m_engine->newFunction(scripting::database::list), m_flags); // E.db.list
    
    scripting::Callable* tp = scripting::make_scripting_call(scripting::database::type, this);
    m_registered_classes << tp;
    db.setProperty("type", m_engine->newObject(tp), m_flags); // E.db.type

    db.setProperty("readableProperties", m_engine->newFunction(scripting::database::readableProperties), m_flags); // E.db.readableProperties
    db.setProperty("writableProperties", m_engine->newFunction(scripting::database::writableProperties), m_flags); // E.db.writableProperties


    scripting::Callable* add = scripting::make_scripting_call(scripting::database::addProperty, this);
    m_registered_classes << add;
    db.setProperty("add", m_engine->newObject(add), m_flags); // E.db.add
    
    scripting::Callable* remove = scripting::make_scripting_call(scripting::database::removeProperty, this);
    m_registered_classes << remove;
    db.setProperty("remove", m_engine->newObject(remove), m_flags); // E.db.remove

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
    db.setProperty("set", m_engine->newObject(set), m_flags); // E.db.set

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
    db.setProperty("get", m_engine->newObject(get), m_flags); // E.db.get
    
    v.setProperty("db", db, m_flags); // E.db

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


    scenes.setProperty("list", m_engine->newFunction(scripting::scenes::list), m_flags);  // E.scenes.list

    // An add method
    scripting::MultiMethod* add = new scripting::MultiMethod(m_engine, "add");
    add->add(scripting::make_scripting_call(scripting::scenes::add, this));
    add->add(scripting::make_scripting_call(scripting::scenes::addNameless, this));
    m_registered_classes << add;
    
    scenes.setProperty("add", m_engine->newObject(add), m_flags);  // E.scenes.add
    
    scripting::Callable* remove = scripting::make_scripting_call(scripting::scenes::remove, this);
    m_registered_classes << remove;
    scenes.setProperty("remove", m_engine->newObject(remove), m_flags); // E.scenes.remove

    scripting::Callable* moveback = scripting::make_scripting_call(scripting::scenes::moveBack, this);
    m_registered_classes << moveback;
    scenes.setProperty("moveBack", m_engine->newObject(moveback), m_flags); // E.scenes.moveBack

    scripting::Callable* movefront = scripting::make_scripting_call(scripting::scenes::moveFront, this);
    m_registered_classes << movefront;
    scenes.setProperty("moveFront", m_engine->newObject(movefront), m_flags); // E.scenes.moveFront

    scripting::MultiMethod* set = new scripting::MultiMethod(m_engine, "set");
    set->add(new scripting::scenes::NameSetter(m_engine));
    m_registered_classes << set;
    scenes.setProperty("set", m_engine->newObject(set), m_flags); // E.scenes.set

    scripting::MultiMethod* get = new scripting::MultiMethod(m_engine, "get");
    get->add(new scripting::AbstractGetter<sad::Scene*, sad::String>(m_engine, "name"));
    get->add(new scripting::AbstractGetter<sad::Scene*, unsigned int>(m_engine, "layer"));
    get->add(new scripting::AbstractGetter<sad::Scene*, unsigned long long>(m_engine, "majorid"));
    get->add(new scripting::AbstractGetter<sad::Scene*, unsigned long long>(m_engine, "minorid"));
    m_registered_classes << get;
    scenes.setProperty("get", m_engine->newObject(get), m_flags); // E.scenes.set

    v.setProperty("scenes", scenes, m_flags); // E.scenes

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

    scenenodes.setProperty("list", m_engine->newFunction(scripting::scenenodes::list), m_flags); // E.scenenodes.list

    scripting::Callable* listscene = scripting::make_scripting_call(scripting::scenenodes::listScene, this);
    m_registered_classes << listscene;
    scenenodes.setProperty("listScene", m_engine->newObject(listscene), m_flags); // E.scenenodes.listScene


    scripting::Callable* _addlabel = scripting::make_scripting_call(scripting::scenenodes::_addLabel, this);
    m_registered_classes << _addlabel;
    scenenodes.setProperty("_addLabel", m_engine->newObject(_addlabel), m_flags); // E.scenenodes._addLabel

    scripting::Callable* _addsprite2d = scripting::make_scripting_call(scripting::scenenodes::_addSprite2D, this);
    m_registered_classes << _addsprite2d;
    scenenodes.setProperty("_addSprite2D", m_engine->newObject(_addsprite2d), m_flags); // E.scenenodes._addSprite2D

    scripting::Callable* _addcustomobject = scripting::make_scripting_call(scripting::scenenodes::_addCustomObject, this);
    m_registered_classes << _addcustomobject;
    scenenodes.setProperty("_addCustomObject", m_engine->newObject(_addcustomobject), m_flags); // E.scenenodes._addCustomObject

    scripting::Callable* makeBackground = scripting::make_scripting_call(scripting::scenenodes::makeBackground, this);
    m_registered_classes << makeBackground;
    scenenodes.setProperty("makeBackground", m_engine->newObject(makeBackground), m_flags); // E.scenenodes.makeBackground

    scripting::Callable* remove = scripting::make_scripting_call(scripting::scenenodes::remove, this);
    m_registered_classes << remove;
    scenenodes.setProperty("remove", m_engine->newObject(remove)); // E.scenenodes.remove

    scripting::Callable* spanBetweenTwoPoints = scripting::make_scripting_call(scripting::scenenodes::spanBetweenTwoPoints, this);
    m_registered_classes << spanBetweenTwoPoints;
    scenenodes.setProperty("spanBetweenTwoPoints", m_engine->newObject(spanBetweenTwoPoints)); // E.scenenodes.spanBetweenTwoPoints


    scripting::MultiMethod* set = new scripting::MultiMethod(m_engine, "set");
    // All props
    set->add(new scripting::scenenodes::FlagSetter(m_editor, m_engine, "visible", history::scenenodes::changeVisibility));
    set->add(new scripting::scenenodes::Setter<sad::String, history::scenenodes::ChangeName>(m_engine, "name"));
    set->add(new scripting::scenenodes::AreaSetter(m_engine));
    set->add(new scripting::scenenodes::Setter<double, history::scenenodes::ChangeAngle>(m_engine, "angle"));
    set->add(new scripting::scenenodes::Setter<sad::AColor, history::scenenodes::ChangeColor>(m_engine, "color"));
    // sad::Label props
    set->add(new scripting::scenenodes::FontSizeSetter(m_engine));
    set->add(new scripting::scenenodes::Setter<sad::String, history::label::ChangeText>(m_engine, "text"));
    set->add(new scripting::scenenodes::Setter<float, history::label::ChangeLineSpacing>(m_engine, "linespacing"));
    set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeMaximalLineWidth>(m_engine, "maximallinewidth"));
    set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeOverflowStrategy>(m_engine, "overflowstrategy"));
    set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeBreakText>(m_engine, "breaktext"));
    set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeTextEllipsis>(m_engine, "textellipsisposition"));
    set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeMaximalLinesCount>(m_engine, "maximallinescount"));
    set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeOverflowStrategyForLines>(m_engine, "overflowstrategyforlines"));
    set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeTextEllipsisForLines>(m_engine, "textellipsispositionforlines"));

    set->add(new scripting::scenenodes::FontSetter(m_engine));
    // sad::Sprite2D props
    set->add(new scripting::scenenodes::FlagSetter(m_editor, m_engine, "flipx", history::sprite2d::changeFlipX));
    set->add(new scripting::scenenodes::FlagSetter(m_editor, m_engine, "flipy", history::sprite2d::changeFlipY));
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
    scenenodes.setProperty("set", m_engine->newObject(set), m_flags); // E.scenes.set
    
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
    get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>(m_engine, "maximallinewidth"));
    get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>(m_engine, "overflowstrategy"));
    get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>(m_engine, "breaktext"));
    get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>(m_engine, "textellipsisposition"));
    get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>(m_engine, "maximallinescount"));
    get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>(m_engine, "overflowstrategyforlines"));
    get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>(m_engine, "textellipsispositionforlines"));
    
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
    scenenodes.setProperty("get", m_engine->newObject(get), m_flags); // E.scenes.get

    v.setProperty("scenenodes", scenenodes, m_flags); // E.scenenodes

    m_engine->evaluate(
        "E.OverflowStrategy = { \"Visible\": 0, \"Hidden\": 1, \"Ellipsis\": 2 };"
        "E.BreakText = { \"Normal\": 0, \"BreakWord\": 1};"
        "E.TextEllipsisPosition = { \"Begin\": 0, \"Middle\": 1, \"End\": 2};"
    );

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

// ReSharper disable once CppMemberFunctionMayBeConst
void scripting::Scripting::initLayoutGridBindings(QScriptValue& v)
{
    QScriptValue layouts = m_engine->newObject();

    layouts.setProperty("list", m_engine->newFunction(scripting::layouts::list), m_flags); // E.layouts.list
    layouts.setProperty("query", m_engine->newFunction(scripting::layouts::query), m_flags); // E.layouts.query
    layouts.setProperty("add", m_engine->newFunction(scripting::layouts::add), m_flags); // E.layouts.add
    layouts.setProperty("remove", m_engine->newFunction(scripting::layouts::remove), m_flags); // E.layouts.remove
    layouts.setProperty("parent", m_engine->newFunction(scripting::layouts::parent), m_flags); // E.layouts.parent

    v.setProperty("layouts", layouts, m_flags); // E.layouts
}

void scripting::Scripting::initWaysBindings(QScriptValue& v)
{
    QScriptValue ways = m_engine->newObject();

    ways.setProperty("list", m_engine->newFunction(scripting::ways::list), m_flags); // E.ways.list

    scripting::Callable* _add = scripting::make_scripting_call(scripting::ways::_add, this);
    _add->setName("_add");
    m_registered_classes << _add;
    ways.setProperty("_add", m_engine->newObject(_add), m_flags); // E.ways._add

    scripting::Callable* remove = scripting::make_scripting_call(scripting::ways::remove, this);
    remove->setName("remove");
    m_registered_classes << remove;
    ways.setProperty("remove", m_engine->newObject(remove), m_flags); // E.ways.remove

    scripting::Callable* length = scripting::make_scripting_call(scripting::ways::length, this);
    length->setName("length");
    m_registered_classes << length;
    ways.setProperty("length", m_engine->newObject(length), m_flags); // E.ways.length

    scripting::Callable* addPoint = scripting::make_scripting_call(scripting::ways::addPoint, this);
    addPoint->setName("addPoint");
    m_registered_classes << addPoint;
    ways.setProperty("addPoint", m_engine->newObject(addPoint), m_flags); // E.ways.addPoint

    scripting::Callable* removePoint = scripting::make_scripting_call(scripting::ways::removePoint, this);
    removePoint->setName("removePoint");
    m_registered_classes << removePoint;
    ways.setProperty("removePoint", m_engine->newObject(removePoint), m_flags); // E.ways.removePoint

    scripting::Callable* point = scripting::make_scripting_call(scripting::ways::point, this);
    point->setName("point");
    m_registered_classes << point;
    ways.setProperty("point", m_engine->newObject(point), m_flags); // E.ways.point

    scripting::MultiMethod* set = new scripting::MultiMethod(m_engine, "set");
    set->add(new scripting::ways::Setter<sad::String, history::ways::ChangeName>(m_engine, "name"));
    set->add(new scripting::ways::Setter<double, history::ways::ChangeTotalTime>(m_engine, "totaltime"));
    set->add(new scripting::ways::Setter<bool, history::ways::ChangeClosed>(m_engine, "closed"));
    m_registered_classes << set;
    ways.setProperty("set", m_engine->newObject(set), m_flags); // E.ways.set


    scripting::MultiMethod* get = new scripting::MultiMethod(m_engine, "get");
    get->add(new scripting::AbstractGetter<sad::p2d::app::Way*, sad::String>(m_engine, "name"));
    get->add(new scripting::AbstractGetter<sad::p2d::app::Way*, unsigned long long>(m_engine, "majorid"));
    get->add(new scripting::AbstractGetter<sad::p2d::app::Way*, unsigned long long>(m_engine, "minorid"));
    get->add(new scripting::AbstractGetter<sad::p2d::app::Way*, double>(m_engine, "totaltime"));
    get->add(new scripting::AbstractGetter<sad::p2d::app::Way*, bool>(m_engine, "closed"));
    m_registered_classes << get;
    ways.setProperty("get", m_engine->newObject(get), m_flags); // E.ways.get

    v.setProperty("ways", ways, m_flags); // E.ways

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

    dialogues.setProperty("list", m_engine->newFunction(scripting::dialogues::list), m_flags); // E.dialogues.list

    scripting::Callable* _add = scripting::make_scripting_call(scripting::dialogues::_add, this);
    _add->setName("_add");
    m_registered_classes << _add;
    dialogues.setProperty("_add", m_engine->newObject(_add), m_flags); // E.dialogues._add

    scripting::Callable* remove = scripting::make_scripting_call(scripting::dialogues::remove, this);
    remove->setName("remove");
    m_registered_classes << remove;
    dialogues.setProperty("remove", m_engine->newObject(remove), m_flags); // E.dialogues.remove

    scripting::Callable* addPhrase = scripting::make_scripting_call(scripting::dialogues::addPhrase, this);
    addPhrase->setName("addPhrase");
    m_registered_classes << addPhrase;
    dialogues.setProperty("addPhrase", m_engine->newObject(addPhrase), m_flags); // E.dialogues.addPhrase

    scripting::Callable* removePhrase = scripting::make_scripting_call(scripting::dialogues::removePhrase, this);
    removePhrase->setName("removePhrase");
    m_registered_classes << removePhrase;
    dialogues.setProperty("removePhrase", m_engine->newObject(removePhrase), m_flags); // E.dialogues.removePhrase

    scripting::Callable* length = scripting::make_scripting_call(scripting::dialogues::length, this);
    length->setName("length");
    m_registered_classes << length;
    dialogues.setProperty("length", m_engine->newObject(length), m_flags); // E.dialogues.length

    scripting::Callable* phrase = scripting::make_scripting_call(scripting::dialogues::phrase, this);
    phrase->setName("phrase");
    m_registered_classes << phrase;
    dialogues.setProperty("phrase", m_engine->newObject(phrase), m_flags); // E.dialogues.point

    scripting::MultiMethod* set = new scripting::MultiMethod(m_engine, "set");
    set->add(new scripting::dialogues::Setter<sad::String, history::dialogues::ChangeName>(m_engine, "name"));
    m_registered_classes << set;
    dialogues.setProperty("set", m_engine->newObject(set), m_flags); // E.dialogues.set


    scripting::MultiMethod* get = new scripting::MultiMethod(m_engine, "get");
    get->add(new scripting::AbstractGetter<sad::dialogue::Dialogue*, sad::String>(m_engine, "name"));
    get->add(new scripting::AbstractGetter<sad::dialogue::Dialogue*, unsigned long long>(m_engine, "majorid"));
    get->add(new scripting::AbstractGetter<sad::dialogue::Dialogue*, unsigned long long>(m_engine, "minorid"));
    m_registered_classes << get;
    dialogues.setProperty("get", m_engine->newObject(get), m_flags); // E.dialogues.get


    v.setProperty("dialogues", dialogues, m_flags); // E.dialogues

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
    
    animations.setProperty("list", m_engine->newFunction(scripting::animations::list), m_flags); // E.animations.list

    scripting::Callable* _add = scripting::make_scripting_call(scripting::animations::_add, this);
    _add->setName("_add");
    m_registered_classes << _add;
    animations.setProperty("_add", m_engine->newObject(_add), m_flags); // E.animations._add

    scripting::Callable* remove = scripting::make_scripting_call(scripting::animations::remove, this);
    remove->setName("remove");
    m_registered_classes << remove;
    animations.setProperty("remove", m_engine->newObject(remove), m_flags); // E.animations.remove

    scripting::Callable* addToComposite = scripting::make_scripting_call(scripting::animations::addToComposite, this);
    addToComposite->setName("addToComposite");
    m_registered_classes << addToComposite;
    animations.setProperty("addToComposite", m_engine->newObject(addToComposite), m_flags); // E.animations.addToComposite

    scripting::Callable* removeFromComposite = scripting::make_scripting_call(scripting::animations::removeFromComposite, this);
    removeFromComposite->setName("removeFromComposite");
    m_registered_classes << removeFromComposite;
    animations.setProperty("removeFromComposite", m_engine->newObject(removeFromComposite), m_flags); // E.animations.removeFromComposite

    scripting::Callable* compositeLength = scripting::make_scripting_call(scripting::animations::compositeLength, this);
    compositeLength->setName("compositeLength");
    m_registered_classes << compositeLength;
    animations.setProperty("compositeLength", m_engine->newObject(compositeLength), m_flags); // E.animations.compositeLength

    scripting::Callable* getAnimation = scripting::make_scripting_call(scripting::animations::getAnimation, this);
    getAnimation->setName("getAnimation");
    m_registered_classes << getAnimation;
    animations.setProperty("getAnimation", m_engine->newObject(getAnimation), m_flags); // E.animations.getAnimation

    scripting::Callable* moveBackInCompositeList = scripting::make_scripting_call(scripting::animations::moveBackInCompositeList, this);
    moveBackInCompositeList->setName("moveBackInCompositeList");
    m_registered_classes << moveBackInCompositeList;
    animations.setProperty("moveBackInCompositeList", m_engine->newObject(moveBackInCompositeList), m_flags); // E.animations.moveBackInCompositeList

    scripting::Callable* moveFrontInCompositeList = scripting::make_scripting_call(scripting::animations::moveFrontInCompositeList, this);
    moveFrontInCompositeList->setName("moveFrontInCompositeList");
    m_registered_classes << moveFrontInCompositeList;
    animations.setProperty("moveFrontInCompositeList", m_engine->newObject(moveFrontInCompositeList), m_flags); // E.animations.moveFrontInCompositeList


    scripting::MultiMethod* set = new scripting::MultiMethod(m_engine, "set");
    set->add(new scripting::animations::Setter<sad::animations::Animation, sad::String, history::animations::ChangeName>(m_engine, "name"));
    set->add(new scripting::animations::Setter<sad::animations::Animation, double, history::animations::ChangeTime>(m_engine, "time"));
    set->add(new scripting::animations::Setter<sad::animations::Animation, bool, history::animations::ChangeLooped>(m_engine, "looped"));
    set->add(new scripting::animations::Setter<sad::animations::Blinking, unsigned int, history::animations::ChangeBlinkingFrequency>(m_engine, "frequency"));
    set->add(new scripting::animations::Setter<sad::animations::CameraShaking, sad::Point2D, history::animations::ChangeCameraOffset>(m_engine, "offset"));
    set->add(new scripting::animations::Setter<sad::animations::CameraShaking, int, history::animations::ChangeShakingFrequency>(m_engine, "frequency"));
    set->add(new scripting::animations::Setter<sad::animations::CameraRotation, sad::Point3D, history::animations::ChangeCameraPivot>(m_engine, "pivot"));

    gui::uiblocks::UIAnimationBlock* ablk = m_editor->uiBlocks()->uiAnimationBlock();
    set->add(new scripting::animations::WidgetSetter<
                sad::animations::CameraRotation, 
                QDoubleSpinBox*,
                double, 
                history::animations::ChangeCameraAngle
            >(m_engine,  ablk->dsbCameraRotationStartingAngle, "min_angle")
    );
    set->add(new scripting::animations::WidgetSetter<
                sad::animations::CameraRotation, 
                QDoubleSpinBox*,
                double, 
                history::animations::ChangeCameraAngle
            >(m_engine,  ablk->dsbCameraRotationEndingAngle, "max_angle")
    );
    set->add(new scripting::animations::WidgetSetter<
                sad::animations::Color, 
                gui::colorview::ColorView*,
                sad::AColor, 
                history::animations::ChangeColorColor
            >(m_engine, ablk->cwColorStartingColor, "min_color")
    );
    set->add(new scripting::animations::WidgetSetter<
                sad::animations::Color, 
                gui::colorview::ColorView*,
                sad::AColor, 
                history::animations::ChangeColorColor
            >(m_engine, ablk->cwColorEndingColor, "max_color")
    );
    set->add(new scripting::animations::Setter<sad::animations::FontList, sad::Vector<sad::String>, history::animations::ChangeFontListFonts>(m_engine, "fonts"));
    set->add(new scripting::animations::WidgetSetter<
                sad::animations::FontSize, 
                QSpinBox*,
                unsigned int, 
                history::animations::ChangeFontSizeSize
            >(m_engine, ablk->sbFontSizeStartingSize, "min_size")
    );
    set->add(new scripting::animations::WidgetSetter<
                sad::animations::FontSize, 
                QSpinBox*,
                unsigned int, 
                history::animations::ChangeFontSizeSize
            >(m_engine, ablk->sbFontSizeEndingSize, "max_size")
    );
    set->add(new scripting::animations::Setter<sad::animations::Resize, sad::Point2D, history::animations::ChangeResizeStartingSize>(m_engine, "start_size"));
    set->add(new scripting::animations::Setter<sad::animations::Resize, sad::Point2D, history::animations::ChangeResizeEndingSize>(m_engine, "end_size"));
    set->add(new scripting::animations::WidgetSetter<
                sad::animations::Rotate, 
                QDoubleSpinBox*,
                double, 
                history::animations::ChangeRotateAngle
            >(m_engine, ablk->dsbRotateStartingAngle, "min_angle")
    );
    set->add(new scripting::animations::WidgetSetter<
                sad::animations::Rotate, 
                QDoubleSpinBox*,
                double, 
                history::animations::ChangeRotateAngle
            >(m_engine, ablk->dsbRotateEndingAngle, "max_angle")
    );
    set->add(new scripting::animations::WidgetSetter<
                sad::animations::OptionList, 
                QTextEdit*,
                sad::Vector<sad::String>, 
                history::animations::ChangeList
            >(m_engine, ablk->txtOptionListList, "list")
    );
    set->add(new scripting::animations::WidgetSetter<
                sad::animations::TextureCoordinatesList, 
                QTextEdit*,
                sad::Vector<sad::String>, 
                history::animations::ChangeList
            >(m_engine, ablk->txtTextureCoordinatesList, "list")
    );
    set->add(new scripting::animations::Point2DSetter<
                sad::animations::SimpleMovement, 
                history::animations::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes
            >(m_engine, "start_point", ablk->dabSimpleMovementStartingPointX,ablk->dabSimpleMovementStartingPointY)
    );
    set->add(new scripting::animations::Point2DSetter<
                sad::animations::SimpleMovement, 
                history::animations::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes
            >(m_engine, "end_point", ablk->dabSimpleMovementEndingPointX,ablk->dabSimpleMovementEndingPointY)
    );
    
    set->add(new scripting::animations::EasingSetter<
                unsigned int, 
                history::animations::ChangeEasingFunctionType
            >(m_engine, "easing_type", &sad::animations::easing::Function::functionTypeAsUnsignedInt)
    );
    set->add(new scripting::animations::EasingSetter<
                double, 
                history::animations::ChangeEasingOvershootAmplitude
            >(m_engine, "easing_overshoot_amplitude", &sad::animations::easing::Function::overshootAmplitude)
    );
    set->add(new scripting::animations::EasingSetter<
                double, 
                history::animations::ChangeEasingPeriod
            >(m_engine, "easing_period", &sad::animations::easing::Function::period)
    );


    scripting::animations::WidgetSetter<
        sad::animations::TextureCoordinatesContinuous,
        gui::rectwidget::RectWidget*,
        sad::Rect2D,
        history::animations::ChangeRect
    >* rect1 = new scripting::animations::WidgetSetter<
        sad::animations::TextureCoordinatesContinuous,
        gui::rectwidget::RectWidget*,
        sad::Rect2D,
        history::animations::ChangeRect
    >(m_engine,  ablk->rctTCCStartingRect, "start_rect");
    rect1->addCondition(new IsAABB());
    set->add(rect1);

    scripting::animations::WidgetSetter<
        sad::animations::TextureCoordinatesContinuous,
        gui::rectwidget::RectWidget*,
        sad::Rect2D,
        history::animations::ChangeRect
    >* rect2 = new scripting::animations::WidgetSetter<
        sad::animations::TextureCoordinatesContinuous,
        gui::rectwidget::RectWidget*,
        sad::Rect2D,
        history::animations::ChangeRect
    >(m_engine,  ablk->rctTCCEndingRect, "end_rect");
    rect2->addCondition(new IsAABB());
    set->add(rect2);

    set->add(new scripting::animations::WaySetter(m_engine));
    m_registered_classes << set;
    animations.setProperty("set", m_engine->newObject(set), m_flags); // E.animations.set

    scripting::MultiMethod* get = new scripting::MultiMethod(m_engine, "get");
    get->add(new scripting::AbstractGetter<sad::animations::Animation*, sad::String>(m_engine, "name"));
    get->add(new scripting::AbstractGetter<sad::animations::Animation*, unsigned long long>(m_engine, "majorid"));
    get->add(new scripting::AbstractGetter<sad::animations::Animation*, unsigned long long>(m_engine, "minorid"));
    get->add(new scripting::AbstractGetter<sad::animations::Animation*, double>(m_engine, "time"));
    get->add(new scripting::AbstractGetter<sad::animations::Animation*, bool>(m_engine, "looped"));
    get->add(new scripting::animations::EasingGetter<unsigned int>(m_engine, "easing_type", &sad::animations::easing::Function::functionTypeAsUnsignedInt));
    get->add(new scripting::animations::EasingGetter<double>(m_engine, "easing_overshoot_amplitude", &sad::animations::easing::Function::overshootAmplitude));
    get->add(new scripting::animations::EasingGetter<double>(m_engine, "easing_period", &sad::animations::easing::Function::period));
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
    get->add(new scripting::AbstractGetter<sad::animations::Resize*, sad::Point2D >(m_engine, "start_size"));
    get->add(new scripting::AbstractGetter<sad::animations::Resize*, sad::Point2D >(m_engine, "end_size"));	
    get->add(new scripting::AbstractGetter<sad::animations::Rotate*, double >(m_engine, "min_angle"));
    get->add(new scripting::AbstractGetter<sad::animations::Rotate*, double >(m_engine, "max_angle"));	
    get->add(new scripting::AbstractGetter<sad::animations::SimpleMovement*, sad::Point2D>(m_engine, "start_point"));	
    get->add(new scripting::AbstractGetter<sad::animations::SimpleMovement*, sad::Point2D>(m_engine, "end_point"));		
    get->add(new scripting::AbstractGetter<sad::animations::OptionList*, sad::Vector<sad::String> >(m_engine, "list"));
    get->add(new scripting::AbstractGetter<sad::animations::TextureCoordinatesContinuous*, sad::Rect2D >(m_engine, "start_rect"));
    get->add(new scripting::AbstractGetter<sad::animations::TextureCoordinatesContinuous*, sad::Rect2D >(m_engine, "end_rect"));
    get->add(new scripting::AbstractGetter<sad::animations::TextureCoordinatesList*, sad::Vector<sad::String> >(m_engine, "list"));
    get->add(new scripting::AbstractGetter<sad::animations::WayMoving*, unsigned long long >(m_engine, "way"));
    get->add(new scripting::AbstractGetter<sad::animations::Composite*, sad::Vector<unsigned long long> >(m_engine, "list"));
    
    m_registered_classes << get;
    animations.setProperty("get", m_engine->newObject(get), m_flags); // E.animations.set


    v.setProperty("animations", animations, m_flags); // E.animations
    
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
       "SimpleMovement",
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

    m_engine->evaluate(
        "E.animations.easingsToNames = ["
        "\"Linear\","
        "\"InSine\","
        "\"OutSine\","
        "\"InOutSine\","
        "\"InQuad\","
        "\"OutQuad\","
        "\"InOutQuad\","
        "\"InCubic\","
        "\"OutCubic\","
        "\"InOutCubic\","
        "\"InQuart\","
        "\"OutQuart\","
        "\"InOutQuart\","
        "\"InQuint\","
        "\"OutQuint\","
        "\"InOutQuint\","
        "\"InExpo\","
        "\"OutExpo\","
        "\"InOutExpo\","
        "\"InCirc\","
        "\"OutCirc\","
        "\"InOutCirc\","
        "\"InElastic\","
        "\"OutElastic\","
        "\"InOutElastic\","
        "\"InBack\","
        "\"OutBack\","
        "\"InOutBack\","
        "\"InBounce\","
        "\"OutBounce\","
        "\"InOutBounce\""
        "];" 
        "E.animations.easing = function(a)"
        "{"
        "  if (typeof(a) == \"string\") return E.animations.easingsToNames.indexOf(a); else return  E.animations.easingsToNames[a]; "
        "};"
    );
}

void scripting::Scripting::initAnimationInstanceBindings(QScriptValue& v)
{
    QScriptValue instances = m_engine->newObject();

    instances.setProperty("list", m_engine->newFunction(scripting::instances::list), m_flags); // E.animations.instances.list

    scripting::Callable* _addInstance = scripting::make_scripting_call(scripting::instances::_addInstance, this);
    _addInstance->setName("_addInstance");
    m_registered_classes << _addInstance;
    instances.setProperty("_addInstance", m_engine->newObject(_addInstance), m_flags); // E.animations.instances._addInstance

    scripting::Callable* _addWayInstance = scripting::make_scripting_call(scripting::instances::_addWayInstance, this);
    _addWayInstance->setName("_addWayInstance");
    m_registered_classes << _addWayInstance;
    instances.setProperty("_addWayInstance", m_engine->newObject(_addWayInstance), m_flags); // E.animations.instances._addWayInstance

    scripting::Callable* remove = scripting::make_scripting_call(scripting::instances::remove, this);
    remove->setName("remove");
    m_registered_classes << remove;
    instances.setProperty("remove", m_engine->newObject(remove), m_flags); // E.animations.instances.remove

    scripting::MultiMethod* set = new scripting::MultiMethod(m_engine, "set");
    set->add(new scripting::instances::NameSetter(m_engine));
    set->add(new scripting::instances::AnimationSetter(m_engine));
    set->add(new scripting::instances::AnimationDBSetter(m_engine));
    set->add(new scripting::instances::ObjectSetter(m_engine));
    set->add(new scripting::instances::StartTimeSetter(m_engine));
    set->add(new scripting::instances::WaySetter(m_engine));
    
    m_registered_classes << set;
    instances.setProperty("set", m_engine->newObject(set), m_flags); // E.animations.instances.set


    scripting::MultiMethod* get = new scripting::MultiMethod(m_engine, "get");
    get->add(new scripting::AbstractGetter<sad::animations::Instance*, sad::String>(m_engine, "name"));
    get->add(new scripting::AbstractGetter<sad::animations::Instance*, unsigned long long>(m_engine, "majorid"));
    get->add(new scripting::AbstractGetter<sad::animations::Instance*, unsigned long long>(m_engine, "minorid"));
    get->add(new scripting::AbstractGetter<sad::animations::Instance*, sad::String>(m_engine, "animation"));
    get->add(new scripting::AbstractGetter<sad::animations::Instance*, unsigned long long>(m_engine, "animationmajorid"));
    get->add(new scripting::AbstractGetter<sad::animations::Instance*, unsigned long long>(m_engine, "object"));
    get->add(new scripting::AbstractGetter<sad::animations::Instance*, double>(m_engine, "starttime"));
    get->add(new scripting::AbstractGetter<sad::animations::WayInstance*, unsigned long long>(m_engine, "way"));
    
    m_registered_classes << get;
    instances.setProperty("get", m_engine->newObject(get), m_flags); // E.animations.instances.get

    v.property("animations").setProperty("instances", instances, m_flags);

    m_engine->evaluate(
        "E.animations.instances.addInstance = function(o) {"
        "   if (typeof o != \"object\")    "
        "   {                              "
        "      o = {};                     "
        "   }                              "
        "   if (\"animation\" in o == false)"
        "   {                              "
        "     o[\"animationid\"] = 0; o[\"animationname\"] =\"\"; "
        "   }                              "
        "   else                           "
        "   {                              "
        "        if (typeof o[\"animation\"] == \"string\") "
        "        {                         "
        "             o[\"animationid\"] = 0; o[\"animationname\"] = o[\"animation\"]; "
        "        }                         "
        "        else "
        "        {                         "
        "             o[\"animationid\"] = o[\"animation\"]; o[\"animationname\"] = \"\"; "
        "        }                         "
        "   }"
        "	if (\"name\" in o == false)    "
        "   {                              "
        "     o[\"name\"] = \"\";          "
        "   }                              "
        "	if (\"object\" in o == false)  "
        "	{                              "
        "	   o[\"object\"] = 0;          "
        "	}                              "
        "	if (\"starttime\" in o == false)    "
        "	{                              "
        "	   o[\"starttime\"] = 0;            "
        "	}                              "
        "	return E.animations.instances._addInstance(o[\"name\"], o[\"animationid\"], o[\"animationname\"], o[\"object\"], o[\"starttime\"]);"
        "};"
        "E.animations.instances.addWayInstance = function(o) {"
        "   if (typeof o != \"object\")    "
        "   {                              "
        "      o = {};                     "
        "   }                              "
        "	if (\"name\" in o == false)    "
        "   {                              "
        "     o[\"name\"] = \"\";          "
        "   }                              "
        "	if (\"way\" in o == false)     "
        "	{                              "
        "	   o[\"way\"] = 0;             "
        "	}                              "
        "	if (\"object\" in o == false)  "
        "	{                              "
        "	   o[\"object\"] = 0;          "
        "	}                              "
        "	if (\"starttime\" in o == false)    "
        "	{                              "
        "	   o[\"starttime\"] = 0;            "
        "	}                              "
        "	return E.animations.instances._addWayInstance(o[\"name\"], o[\"way\"], o[\"object\"], o[\"starttime\"]);"
        "};"
        "E.animations.instances.attr = function() {"
        "	if (arguments.length == 2)"
        "	{"
        "		return E.animations.instances.get(arguments[0], arguments[1]);"
        "	}"
        "	if (arguments.length == 3)"
        "	{"
        "		return E.animations.instances.set(arguments[0], arguments[1], arguments[2]);"
        "	}"
        "	throw new Error(\"Specify 2 or 3 arguments\");"
        "};"
    );
}


void scripting::Scripting::initAnimationGroupBindings(QScriptValue& v)
{
    QScriptValue groups = m_engine->newObject();

    groups.setProperty("list", m_engine->newFunction(scripting::groups::list), m_flags); // E.animations.groups.list

    scripting::Callable* _add = scripting::make_scripting_call(scripting::groups::_add, this);
    _add->setName("_add");
    m_registered_classes << _add;
    groups.setProperty("_add", m_engine->newObject(_add), m_flags); // E.animations.groups._add

    scripting::Callable* remove = scripting::make_scripting_call(scripting::groups::remove, this);
    remove->setName("remove");
    m_registered_classes << remove;
    groups.setProperty("remove", m_engine->newObject(remove), m_flags); // E.animations.groups.remove

    scripting::Callable* length = scripting::make_scripting_call(scripting::groups::length, this);
    length->setName("length");
    m_registered_classes << length;
    groups.setProperty("length", m_engine->newObject(length), m_flags); // E.animations.groups.length

    scripting::Callable* entry = scripting::make_scripting_call(scripting::groups::entry, this);
    entry->setName("entry");
    m_registered_classes << entry;
    groups.setProperty("entry", m_engine->newObject(entry), m_flags); // E.animations.groups.entry

    scripting::Callable* addInstance = scripting::make_scripting_call(scripting::groups::addInstance, this);
    addInstance->setName("addInstance");
    m_registered_classes << addInstance;
    groups.setProperty("addInstance", m_engine->newObject(addInstance), m_flags); // E.animations.groups.addInstance

    scripting::Callable* removeInstance = scripting::make_scripting_call(scripting::groups::removeInstance, this);
    removeInstance->setName("removeInstance");
    m_registered_classes << removeInstance;
    groups.setProperty("removeInstance", m_engine->newObject(removeInstance), m_flags); // E.animations.groups.removeInstance


    scripting::MultiMethod* set = new scripting::MultiMethod(m_engine, "set");
    set->add(new scripting::groups::NameSetter(m_engine));
    set->add(new scripting::groups::LoopedSetter(m_engine));
    set->add(new scripting::groups::SequentialSetter(m_engine));
    
    m_registered_classes << set;
    groups.setProperty("set", m_engine->newObject(set), m_flags); // E.animations.groups.set


    scripting::MultiMethod* get = new scripting::MultiMethod(m_engine, "get");
    get->add(new scripting::AbstractGetter<sad::animations::Group*, sad::String>(m_engine, "name"));
    get->add(new scripting::AbstractGetter<sad::animations::Group*, unsigned long long>(m_engine, "majorid"));
    get->add(new scripting::AbstractGetter<sad::animations::Group*, unsigned long long>(m_engine, "minorid"));
    get->add(new scripting::AbstractGetter<sad::animations::Group*, bool>(m_engine, "looped"));
    get->add(new scripting::AbstractGetter<sad::animations::Group*, bool>(m_engine, "sequential"));
    get->add(new scripting::AbstractGetter<sad::animations::Group*, sad::Vector<unsigned long long> >(m_engine, "instances"));
    
    m_registered_classes << get;
    groups.setProperty("get", m_engine->newObject(get), m_flags); // E.scenes.set

    v.property("animations").setProperty("groups", groups, m_flags);


    m_engine->evaluate(
         "E.animations.groups.add = function(o) {"
        "   if (typeof o != \"object\")    "
        "   {                              "
        "      o = {};                     "
        "   }                              "
        "	if (\"name\" in o == false)    "
        "   {                              "
        "     o[\"name\"] = \"\";          "
        "   }                              "
        "	if (\"looped\" in o == false)  "
        "	{                              "
        "	   o[\"looped\"] = false;      "
        "	}                              "
        "	return E.animations.groups._add(o[\"name\"], o[\"looped\"]);"
        "};"
        "E.animations.groups.attr = function() {"
        "	if (arguments.length == 2)"
        "	{"
        "		return E.animations.groups.get(arguments[0], arguments[1]);"
        "	}"
        "	if (arguments.length == 3)"
        "	{"
        "		return E.animations.groups.set(arguments[0], arguments[1], arguments[2]);"
        "	}"
        "	throw new Error(\"Specify 2 or 3 arguments\");"
        "};"
    );
}

// ReSharper disable once CppMemberFunctionMayBeConst
void scripting::Scripting::saveScript()
{
    QString name = QFileDialog::getSaveFileName(this->editor()->panelAsWidget(), "Enter file, where we should store source code", "", "*.js");
    if (name.length() != 0)
    {
        QFile file(name);
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&file);
            gui::uiblocks::UIConsoleBlock* cblk = m_editor->uiBlocks()->uiConsoleBlock();
            stream << cblk->txtConsoleCode->toPlainText();
        }
        else
        {
            QMessageBox::critical(this->editor()->panelAsWidget(), "Saddy Interface Editor", "Cannot open file " + name);
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void scripting::Scripting::loadScript()
{
    QString name = QFileDialog::getOpenFileName(this->editor()->panelAsWidget(), "Enter file, where code is stored", "", "*.js");
    if (name.length() != 0)
    {
        QFile file(name);
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            QString string;
            string = stream.readAll();
            gui::uiblocks::UIConsoleBlock* cblk = m_editor->uiBlocks()->uiConsoleBlock();
            cblk->txtConsoleCode->setPlainText(string);
        }
        else
        {
            QMessageBox::critical(this->editor()->panelAsWidget(), "Saddy Interface Editor", "Cannot open file " + name);
        }
    }
}

