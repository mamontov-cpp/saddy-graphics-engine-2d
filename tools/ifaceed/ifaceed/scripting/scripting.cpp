#include "scripting.h"
#include "scriptinghelptext.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "constructorcall.h"
#include "makeconstructor.h"
#include "scriptinglog.h"
#include "multimethod.h"
#include "makescriptingcall.h"
#include "makefunctioncall.h"
#include "abstractgetter.h"
#include "abstractsetter.h"
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

#include "../history/scenes/sceneschangename.h"


#include "../history/scenenodes/scenenodeschangename.h"
#include "../history/scenenodes/scenenodeschangeangle.h"
#include "../history/scenenodes/scenenodeschangecolor.h"
#include "../history/scenenodes/scenenodeschangearea.h"
#include "../history/scenenodes/scenenodeschangevisibility.h"

#include "../history/label/changefontsize.h"
#include "../history/label/changetext.h"
#include "../history/label/changefontname.h"
#include "../history/label/changelinespacing.h"
#include "../history/label/changemaximallinewidth.h"
#include "../history/label/changebreaktext.h"
#include "../history/label/changeoverflowstrategy.h"
#include "../history/label/changetextellipsis.h"
#include "../history/label/changemaximallinescount.h"
#include "../history/label/changeoverflowstrategyforlines.h"
#include "../history/label/changetextellipsisforlines.h"
#include "../history/label/changehasformatting.h"

#include "../history/sprite2d/changeflipx.h"
#include "../history/sprite2d/changeflipy.h"

#include "database/databasebindings.h"
#include "database/databasepropertysetter.h"
#include "database/databasepropertygetter.h"

#include "scenes/scenesbindings.h"

#include "scenenodes/scenenodesbindings.h"
#include "scenenodes/scenenodessetter.h"
#include "scenenodes/scenenodesflagsetter.h"
#include "scenenodes/scenenodesoptionssetter.h"
#include "scenenodes/scenenodesschemasetter.h"
#include "scenenodes/scenenodescustomsetter.h"
#include "scenenodes/scenenodescustomgetter.h"

#include "layouts/scriptablelengthvalue.h"
#include "layouts/scriptablegridcell.h"
#include "layouts/gridbindings.h"

#include "ways/waysbindings.h"

#include "dialogues/dialoguesbindings.h"

#include "animations/animationsbindings.h"

#include "instances/instancesbindings.h"
/*
#include "instances/instancesnamesetter.h"
#include "instances/instancesanimationsetter.h"
#include "instances/instancesanimationdbsetter.h"
#include "instances/instancesobjectsetter.h"
#include "instances/instancesstarttimesetter.h"
#include "instances/instanceswaysetter.h"
*/

#include "groups/groupsbindings.h"
/*
#include "groups/groupsnamesetter.h"
#include "groups/groupsloopedsetter.h"
#include "groups/groupssequentialsetter.h"
*/
#include <QFileDialog>
#include <QDebug>
#include <QTextStream>
#include <QScriptValueIterator>
#include <QTextEdit>

#include <scene.h>

#include "lambda.h"
#include "function.h"

Q_DECLARE_METATYPE(scripting::layouts::ScriptableGrid*)
Q_DECLARE_METATYPE(scripting::layouts::ScriptableGridCell*)
Q_DECLARE_METATYPE(sad::Vector<unsigned long long>) //-V566
Q_DECLARE_METATYPE(QScriptContext*) //-V566
Q_DUKPP03_DECLARE_METATYPE(scripting::layouts::ScriptableLengthValue)  //-V566

// ================================== Miscellaneous functions =================================================
extern const std::string __context_eval_info;
extern const std::string __context_types_info;

/*! Checks, whether object is native in context
 * \brief is_native_object whether object is native
 * \param ctx context
 * \return value
 */
static duk_ret_t is_native_object(duk_context* ctx)
{
    int count = duk_get_top(ctx);
    if (count != 1)
    {
        duk_push_error_object(ctx, DUK_ERR_TYPE_ERROR, "isNativeObject: got %d arguments, instead of 1", count);
        duk_throw(ctx);
        return 0;
    }
    duk_bool_t result = 0;
    if (duk_is_object(ctx, 0))
    {
        result = duk_has_prop_string(ctx, 0, DUKPP03_VARIANT_PROPERTY_SIGNATURE);
    }
    duk_push_boolean(ctx, result);
    return 1;
}

/*! Dumps native object from variant
    \param[in] v variant
    \return string
 */
static QString dump_native_object(const QVariant& v)
{
    if (v.canConvert<sad::db::Object*>())
    {
        sad::db::Object* object = v.value<sad::db::Object*>();
        std::stringstream ss;
        ss << object->serializableName();
        ss << "(" << object << ")";
        std::string name = ss.str();
        return name.c_str();
    }
    else
    {
        std::stringstream ss;
        ss << v.typeName();
        ss << "(" << v.data() << ")";
        std::string name = ss.str();
        return name.c_str();
    }
}

// ================================== PUBLIC METHODS OF scripting::Scripting ==================================


scripting::Scripting::Scripting(QObject* parent) : QObject(parent), m_editor(NULL), m_ctx(NULL), m_evaluating(false)
{
    dukpp03::qt::registerMetaType<sad::db::Object*>();
    dukpp03::qt::registerMetaType<sad::db::Object**>();
    dukpp03::qt::registerMetaType<sad::String>();
    dukpp03::qt::registerMetaType<sad::Rect2D>();

    m_ctx = new dukpp03::qt::Context();
    // Initialize isNativeObject
    m_ctx->registerNativeFunction("isNativeObject", is_native_object, 1);
    m_ctx->registerCallable("dumpNativeObject", dukpp03::qt::make_function::from(dump_native_object));


    scripting::Scripting* me = this;
    std::function<void(QString c)> output_string = [me](QString c) {
        gui::uiblocks::UIConsoleBlock* cblk = me->editor()->uiBlocks()->uiConsoleBlock();
        cblk->txtConsoleResults->append(c);
    };
    m_ctx->registerCallable("outputString", dukpp03::make_lambda<dukpp03::qt::BasicContext>::from(output_string));
    bool b = m_ctx->eval("internal = {}; internal.isNativeObject = isNativeObject; internal.dumpNativeObject = dumpNativeObject; internal.outputString = outputString;");
    assert(b);

    std::string error;
    b = m_ctx->eval(__context_eval_info, true, &error);
    assert(b);

    dukpp03::qt::JSObject* obj = new dukpp03::qt::JSObject();
    obj->setProperty("resourceType", dukpp03::qt::make_function::from(scripting::resource_type));
    obj->setProperty("resourceOptions", dukpp03::qt::curried1::from(this, scripting::resource_options));
    obj->setProperty("resourceSchema", dukpp03::qt::curried1::from(this, scripting::resource_schema));

    obj->registerAsGlobalVariable(m_ctx, "E");
    m_global_value = obj;
    m_global_value->addRef();

    b = m_ctx->eval("E.log = internal.log; E.dump = internal.dump; console = E;", true, &error);
    assert(b);
    

    // TODO: Remove this
    m_flags = QScriptValue::ReadOnly | QScriptValue::Undeletable;
    m_engine = new QScriptEngine();
    m_value = m_engine->newQObject(this, QScriptEngine::QtOwnership, QScriptEngine::SkipMethodsInEnumeration);
    //m_value.setProperty("log", m_engine->newFunction(scripting::scripting_log), m_flags);  // E.log

    QScriptValue globalValue = m_engine->globalObject();
    globalValue.setProperty("console", m_value, m_flags);
    globalValue.setProperty("E", m_value, m_flags);
    globalValue.setProperty("---", m_value, m_flags);


}

scripting::Scripting::~Scripting()
{
    delete m_ctx;
    m_global_value->delRef();

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
    this->initDatabasePropertyBindings();
    this->initSceneBindings();
    this->initSceneNodesBindings();
    this->initLayoutGridBindings();
    this->initWaysBindings();
    this->initDialoguesBindings();
    this->initAnimationsBindings();
    this->initAnimationInstanceBindings(m_value);
    this->initAnimationGroupBindings(m_value);

    // Don't forget to call after object is initialized
    copyProperties(scripting::Scripting::SSC_CPD_FROM_GLOBAL_TO_HEAP);
}

core::Editor* scripting::Scripting::editor() const
{
    return m_editor;
}

dukpp03::qt::Context* scripting::Scripting::context() const
{
    return m_ctx;
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
    QSet<QString> propertiesset;
    QSet<QString> functionsset;

    this->propertiesAndFunctions(propertiesset, functionsset);

    properties = propertiesset.toList();
    functions = functionsset.toList();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void scripting::Scripting::propertiesAndFunctions(
    QSet<QString>& properties,
    QSet<QString>& functions,
    bool get_global
)
{
    duk_context* c = m_ctx->context();
    if (get_global)
    {
        properties = this->commonProperties();
        duk_push_global_object(c);
    }
    duk_enum(c, -1, DUK_ENUM_OWN_PROPERTIES_ONLY);
    while(duk_next(c, -1, 1))
    {
        QString name = duk_get_string(c, -2);
        if (name != "prototype" && name != "__prototype__" && name != "constructor")
        {
           if (duk_is_function(c, -1))
           {
               functions.insert(name);
           }
           else
           {
               properties.insert(name);
               if (duk_is_object(c, -1))
               {
                   this->propertiesAndFunctions(properties, functions, false);
               }
           }
        }
        duk_pop_2(c);
    }
    duk_pop(c);
    if (get_global)
    {
        duk_pop(c);
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
    if (m_evaluating)
    {
        return;
    }
    m_evaluating = true;
    // Restore old properties, that can be destroyed by user's actions
    copyProperties(scripting::Scripting::SSC_CPD_FROM_HEAP_TO_GLOBAL);
    // Set maximum execution time to 30 000 seconds
    m_ctx->setMaximumExecutionTime(30000);

    history::BatchCommand* c = new history::BatchCommand();
    m_editor->setCurrentBatchCommand(c);

    gui::uiblocks::UIConsoleBlock* cblk = m_editor->uiBlocks()->uiConsoleBlock();
    cblk->txtConsoleResults->setText("");
    QString text = cblk->txtConsoleCode->toPlainText();
    std::string error;
    bool success = m_ctx->eval(text.toStdString(), false, &error);
    if (!success)
    {
        QString qerror = error.c_str();
        qerror.replace("\n", "<br />");
        cblk->txtConsoleResults->append(QString("<font color=\"red\">")
                                        + qerror
                                        + QString("</font>")
        );
        c->rollback(m_editor);
        delete c;
    }
    else
    {
        if (m_ctx->getTop() > 0)
        {
            if (!duk_is_undefined(m_ctx->context(), -1))
            {
                duk_eval_string(m_ctx->context(), "internal.log");
                duk_swap(m_ctx->context(), -1, -2);
                duk_call(m_ctx->context(), 1);
            }
        }
        m_ctx->cleanStack();
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
    m_evaluating = false;
}


// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void scripting::Scripting::showHelp()
{
    ScriptingHelp dlg;
            
    dlg.setText(scripting_help);
    
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
    {
        dukpp03::qt::ClassBinding* c = new dukpp03::qt::ClassBinding();
        c->addConstructor<sad::Point2D>("SadPoint2D");
        c->addConstructor<sad::Point2D, double, double>("SadPoint2D");
        c->addMethod("x", dukpp03::qt::bind_method::from(&sad::Point2D::x));
        c->addMethod("y", dukpp03::qt::bind_method::from(&sad::Point2D::y));
        c->addMethod("setX", dukpp03::qt::bind_method::from(&sad::Point2D::setX));
        c->addMethod("setY", dukpp03::qt::bind_method::from(&sad::Point2D::setY));
        c->addMethod("distance", dukpp03::qt::bind_method::from(&sad::Point2D::distance));
        c->setPrototypeFunction("SadPoint2D");

        m_ctx->addClassBinding("sad::Point2D", c);
        bool b = m_ctx->eval("p2d = SadPoint2D;");
        assert(b);
    }

    {
        dukpp03::qt::ClassBinding* c = new dukpp03::qt::ClassBinding();
        c->addConstructor<sad::Point3D>("SadPoint3D");
        c->addConstructor<sad::Point3D, double, double>("SadPoint3D");
        c->addConstructor<sad::Point3D, double, double, double>("SadPoint3D");
        c->addMethod("x", dukpp03::qt::rebind_method::to<sad::Point3D>::from(&sad::Point3D::x));
        c->addMethod("y", dukpp03::qt::rebind_method::to<sad::Point3D>::from(&sad::Point3D::y));
        c->addMethod("z", dukpp03::qt::bind_method::from(&sad::Point3D::z));
        c->addMethod("setX", dukpp03::qt::rebind_method::to<sad::Point3D>::from(&sad::Point3D::setX));
        c->addMethod("setY", dukpp03::qt::rebind_method::to<sad::Point3D>::from(&sad::Point3D::setY));
        c->addMethod("setZ", dukpp03::qt::bind_method::from(&sad::Point3D::setZ));
        c->setPrototypeFunction("SadPoint3D");

        m_ctx->addClassBinding("sad::Point3D", c);
        bool b = m_ctx->eval("p3d = SadPoint3D;");
        assert(b);
    }

    {
        dukpp03::qt::ClassBinding* c = new dukpp03::qt::ClassBinding();
        c->addConstructor<sad::Point2I>("SadPoint2I");
        c->addConstructor<sad::Point2I, int, int>("SadPoint2I");
        c->addMethod("x", dukpp03::qt::bind_method::from(&sad::Point2I::x));
        c->addMethod("y", dukpp03::qt::bind_method::from(&sad::Point2I::y));
        c->addMethod("setX", dukpp03::qt::bind_method::from(&sad::Point2I::setX));
        c->addMethod("setY", dukpp03::qt::bind_method::from(&sad::Point2I::setY));
        c->addMethod("distance", dukpp03::qt::bind_method::from(&sad::Point2I::distance));
        c->setPrototypeFunction("SadPoint2I");

        m_ctx->addClassBinding("sad::Point2I", c);
        bool b = m_ctx->eval("p2i = SadPoint2I;");
        assert(b);
    }

    {
        dukpp03::qt::ClassBinding* c = new dukpp03::qt::ClassBinding();
        c->addConstructor<sad::Point3I>("SadPoint3I");
        c->addConstructor<sad::Point3I, int, int>("SadPoint3I");
        c->addConstructor<sad::Point3I, int, int, int>("SadPoint3I");
        c->addMethod("x", dukpp03::qt::rebind_method::to<sad::Point3I>::from(&sad::Point3I::x));
        c->addMethod("y", dukpp03::qt::rebind_method::to<sad::Point3I>::from(&sad::Point3I::y));
        c->addMethod("z", dukpp03::qt::bind_method::from(&sad::Point3I::z));
        c->addMethod("setX", dukpp03::qt::rebind_method::to<sad::Point3I>::from(&sad::Point3I::setX));
        c->addMethod("setY", dukpp03::qt::rebind_method::to<sad::Point3I>::from(&sad::Point3I::setY));
        c->addMethod("setZ", dukpp03::qt::bind_method::from(&sad::Point3I::setZ));
        c->setPrototypeFunction("SadPoint3I");

        m_ctx->addClassBinding("sad::Point3I", c);
        bool b = m_ctx->eval("p3i = SadPoint3I;");
        assert(b);
    }

    {
        dukpp03::qt::ClassBinding* c = new dukpp03::qt::ClassBinding();
        c->addConstructor<sad::Size2D>("SadSize2D");
        c->addConstructor<sad::Size2D, double, double>("SadSize2D");
        c->addAccessor("Width", dukpp03::qt::getter::from(&sad::Size2D::Width), dukpp03::qt::setter::from(&sad::Size2D::Width));
        c->addAccessor("Height", dukpp03::qt::getter::from(&sad::Size2D::Height), dukpp03::qt::setter::from(&sad::Size2D::Height));
        c->setPrototypeFunction("SadSize2D");

        m_ctx->addClassBinding("sad::Size2D", c);
        bool b = m_ctx->eval("s2d = SadSize2D;");
        assert(b);
    }

    {
        dukpp03::qt::ClassBinding* c = new dukpp03::qt::ClassBinding();
        c->addConstructor<sad::Size2I>("SadSize2I");
        c->addConstructor<sad::Size2I, unsigned int, unsigned int>("SadSize2I");
        c->addAccessor("Width", dukpp03::qt::getter::from(&sad::Size2I::Width), dukpp03::qt::setter::from(&sad::Size2I::Width));
        c->addAccessor("Height", dukpp03::qt::getter::from(&sad::Size2I::Height), dukpp03::qt::setter::from(&sad::Size2I::Height));
        c->setPrototypeFunction("SadSize2I");

        m_ctx->addClassBinding("sad::Size2I", c);
        bool b = m_ctx->eval("s2i = SadSize2I;");
        assert(b);
    }

    {
        dukpp03::qt::ClassBinding* c = new dukpp03::qt::ClassBinding();
        c->addConstructor<sad::Rect2D>("SadRect2D");
        c->addConstructor<sad::Rect2D, sad::Point2D, sad::Point2D>("SadRect2D");
        c->addConstructor<sad::Rect2D, sad::Point2D, sad::Point2D, sad::Point2D, sad::Point2D>("SadRect2D");
        c->addConstructor<sad::Rect2D, double, double, double, double>("SadRect2D");
        c->addMethod("width", dukpp03::qt::bind_method::from(&sad::Rect2D::width));
        c->addMethod("height", dukpp03::qt::bind_method::from(&sad::Rect2D::height));
        c->addMethod("p0", dukpp03::qt::bind_method::from(&sad::Rect2D::p0));
        c->addMethod("p1", dukpp03::qt::bind_method::from(&sad::Rect2D::p1));
        c->addMethod("p2", dukpp03::qt::bind_method::from(&sad::Rect2D::p2));
        c->addMethod("p3", dukpp03::qt::bind_method::from(&sad::Rect2D::p3));
        std::function<dukpp03::Maybe<sad::Point2D>(sad::Rect2D*, int)> point = [](sad::Rect2D* r, int i) {
            dukpp03::Maybe<sad::Point2D> maybepoint;
            if (i >= 0 && i  < 4) {
                maybepoint.setValue((*r)[i]);
                return maybepoint;
            }
            return maybepoint;
        };
        c->addMethod("point", dukpp03::qt::bind_lambda::from(point));
        std::function<void(sad::Rect2D*, int, const sad::Point2D&)> set_point = [](sad::Rect2D* r, int i, const sad::Point2D& p) {
            if (i >= 0 && i  < 4) {
                (*r)[i] = p;
            }
        };
        c->addMethod("setPoint", dukpp03::qt::bind_lambda::from(set_point));
        std::function<sad::Rect2D(sad::Rect2D*, const sad::Point2D)> moved_to_point = [](sad::Rect2D* r, const sad::Point2D& p) {
            sad::Rect2D rect = *r;
            if (sad::isAABB(rect) == false)
            {
                throw std::logic_error("movedToPoint(): rectangle must be axis-aligned");
                return rect;
            }

            sad::Point2D  oldcenter = (rect[0] + rect[2]) / 2.0;
            sad::Point2D  newcenter =p;
            sad::Point2D newp0 = (newcenter - oldcenter) + rect[0];
            sad::Point2D newp2 = (newcenter - oldcenter) + rect[2];
            return sad::Rect2D(newp0, newp2);
        };
        c->addMethod("movedToPoint", dukpp03::qt::bind_lambda::from(moved_to_point));

        c->setPrototypeFunction("SadRect2D");

        m_ctx->addClassBinding("sad::Rect2D", c);

        bool b = m_ctx->eval("r2d = SadRect2D;");
        assert(b);
    }
    {
        dukpp03::qt::ClassBinding* c = new dukpp03::qt::ClassBinding();
        c->addConstructor<sad::Rect2I>("SadRect2I");
        c->addConstructor<sad::Rect2I, sad::Point2I, sad::Point2I>("SadRect2I");
        c->addConstructor<sad::Rect2I, sad::Point2I, sad::Point2I, sad::Point2I, sad::Point2I>("SadRect2I");
        c->addConstructor<sad::Rect2I, double, double, double, double>("SadRect2I");
        c->addMethod("width", dukpp03::qt::bind_method::from(&sad::Rect2I::width));
        c->addMethod("height", dukpp03::qt::bind_method::from(&sad::Rect2I::height));
        c->addMethod("p0", dukpp03::qt::bind_method::from(&sad::Rect2I::p0));
        c->addMethod("p1", dukpp03::qt::bind_method::from(&sad::Rect2I::p1));
        c->addMethod("p2", dukpp03::qt::bind_method::from(&sad::Rect2I::p2));
        c->addMethod("p3", dukpp03::qt::bind_method::from(&sad::Rect2I::p3));
        std::function<dukpp03::Maybe<sad::Point2I>(sad::Rect2I*, int)> point = [](sad::Rect2I* r, int i) {
            dukpp03::Maybe<sad::Point2I> maybepoint;
            if (i >= 0 && i  < 4) {
                maybepoint.setValue((*r)[i]);
                return maybepoint;
            }
            return maybepoint;
        };
        c->addMethod("point", dukpp03::qt::bind_lambda::from(point));
        std::function<void(sad::Rect2I*, int, const sad::Point2I&)> set_point = [](sad::Rect2I* r, int i, const sad::Point2I& p) {
            if (i >= 0 && i  < 4) {
                (*r)[i] = p;
            }
        };
        c->addMethod("setPoint", dukpp03::qt::bind_lambda::from(set_point));

        c->setPrototypeFunction("SadRect2I");

        m_ctx->addClassBinding("sad::Rect2I", c);

        bool b = m_ctx->eval("r2i = SadRect2I;");
        assert(b);
    }

    {
        dukpp03::qt::ClassBinding* c = new dukpp03::qt::ClassBinding();
        c->addConstructor<sad::Color>("SadColor");
        c->addConstructor<sad::Color, unsigned char, unsigned char, unsigned char>("SadColor");
        c->addMethod("r", dukpp03::qt::bind_method::from(&sad::Color::r));
        c->addMethod("g", dukpp03::qt::bind_method::from(&sad::Color::g));
        c->addMethod("b", dukpp03::qt::bind_method::from(&sad::Color::b));
        c->addMethod("setR", dukpp03::qt::bind_method::from(&sad::Color::setR));
        c->addMethod("setG", dukpp03::qt::bind_method::from(&sad::Color::setG));
        c->addMethod("setB", dukpp03::qt::bind_method::from(&sad::Color::setB));
        c->setPrototypeFunction("SadColor");

        m_ctx->addClassBinding("sad::Color", c);

        bool b = m_ctx->eval("clr = SadColor;");
        assert(b);
    }

    {
        dukpp03::qt::ClassBinding* c = new dukpp03::qt::ClassBinding();
        c->addConstructor<sad::AColor>("SadAColor");
        c->addConstructor<sad::AColor, unsigned char, unsigned char, unsigned char>("SadAColor");
        c->addConstructor<sad::AColor, unsigned char, unsigned char, unsigned char, unsigned char>("SadAColor");
        c->addMethod("r", dukpp03::qt::rebind_method::to<sad::AColor>::from(&sad::AColor::r));
        c->addMethod("g", dukpp03::qt::rebind_method::to<sad::AColor>::from(&sad::AColor::g));
        c->addMethod("b",  dukpp03::qt::rebind_method::to<sad::AColor>::from(&sad::AColor::b));
        c->addMethod("a", dukpp03::qt::bind_method::from(&sad::AColor::a));
        c->addMethod("setR", dukpp03::qt::rebind_method::to<sad::AColor>::from(&sad::AColor::setR));
        c->addMethod("setG", dukpp03::qt::rebind_method::to<sad::AColor>::from(&sad::AColor::setG));
        c->addMethod("setB", dukpp03::qt::rebind_method::to<sad::AColor>::from(&sad::AColor::setB));
        c->addMethod("setA", dukpp03::qt::bind_method::from(&sad::AColor::setA));

        c->setPrototypeFunction("SadAColor");

        m_ctx->addClassBinding("sad::AColor", c);

        bool b = m_ctx->eval("aclr = SadAColor;");
        assert(b);
    }


    std::string error;
    bool b = m_ctx->eval(__context_types_info, true, &error);
    if (!b)
    {
        qDebug() << QString(error.c_str());
    }
    assert(b);

    m_global_value->setProperty("screenWidth", dukpp03::qt::make_function::from(scripting::Scripting::screenWidth));
    m_global_value->setProperty("screenHeight", dukpp03::qt::make_function::from(scripting::Scripting::screenHeight));

    // TODO: Remove all after this point
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

void scripting::Scripting::initDatabasePropertyBindings()
{
    dukpp03::qt::JSObject* db = new dukpp03::qt::JSObject();
    db->setProperty("list", dukpp03::qt::make_function::from(scripting::database::list));
    db->setProperty("type", dukpp03::qt::make_function::from(scripting::database::type));
    db->setProperty("readableProperties", dukpp03::qt::make_function::from(scripting::database::readableProperties));
    db->setProperty("writableProperties", dukpp03::qt::make_function::from(scripting::database::writableProperties));
    db->setProperty("addProperty", dukpp03::qt::curried1::from(this, scripting::database::addProperty));
    db->setProperty("removeProperty", dukpp03::qt::curried1::from(this, scripting::database::removeProperty));

    dukpp03::qt::MultiMethod* set = new dukpp03::qt::MultiMethod();
#define PUSH_SETTER(TYPE) set->add(new scripting::database::PropertySetter< TYPE >(this));
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
    db->setProperty("set",  static_cast<dukpp03::qt::Callable*>(set)); // E.db.set


    dukpp03::qt::MultiMethod* get = new dukpp03::qt::MultiMethod();
#define PUSH_GETTER(TYPE) get->add(new scripting::database::PropertyGetter< TYPE >());
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
    db->setProperty("get", static_cast<dukpp03::qt::Callable*>(get)); // E.db.get

    m_global_value->setProperty("db", db);

    bool b = m_ctx->eval(
        "E.db.attr = function() {"  
        "   if (arguments.length == 1)"
        "   {"
        "       return E.db.get(arguments[0]);"
        "   }"
        "   if (arguments.length == 2)"
        "   {"
        "       E.db.set(arguments[0], arguments[1]); return E.db;"
        "   }"
        "   throw new Error(\"Specify 1 or 2 arguments\");"
        "};"
    );
    assert(b);
}

void scripting::Scripting::initSceneBindings()
{
    dukpp03::qt::JSObject* scenes = new dukpp03::qt::JSObject();
    scenes->setProperty("list", dukpp03::qt::make_function::from(scripting::scenes::list));

    dukpp03::qt::MultiMethod* add = new dukpp03::qt::MultiMethod();
    add->add(dukpp03::qt::curried1::from(this, scripting::scenes::add));
    add->add(dukpp03::qt::curried1::from(this, scripting::scenes::addNameless));
    scenes->setProperty("add", static_cast<dukpp03::qt::Callable*>(add)); // E.db.add

    scenes->setProperty("remove", dukpp03::qt::curried1::from(this, scripting::scenes::remove));
    scenes->setProperty("moveBack", dukpp03::qt::curried1::from(this, scripting::scenes::moveBack));
    scenes->setProperty("moveFront", dukpp03::qt::curried1::from(this, scripting::scenes::moveFront));

    dukpp03::qt::MultiMethod* set = new dukpp03::qt::MultiMethod();

    {
        scripting::AbstractSetter<sad::Scene*, sad::String>* name_setter  = scripting::setterForProperty<sad::Scene*, sad::String>(this, "name");
        std::function<
            void(scripting::Scripting*, sad::Scene*, const sad::String&, sad::String oldvalue, sad::String newvalue)
        > set_name_action = [](scripting::Scripting* s, sad::Scene* obj, const sad::String& propertyname, sad::String oldvalue, sad::String newvalue) {
            core::Editor* editor =  s->editor();

            history::Command* c = new history::scenes::ChangeName(obj, oldvalue, newvalue);
            editor->currentBatchCommand()->add(c);
            c->commit(editor);
        };
        name_setter->addAction(set_name_action);
        set->add(name_setter);
    }

    scenes->setProperty("set",  static_cast<dukpp03::qt::Callable*>(set)); // E.scenes.set

    dukpp03::qt::MultiMethod* get = new dukpp03::qt::MultiMethod();
    get->add(new scripting::AbstractGetter<sad::Scene*, sad::String>("name"));
    get->add(new scripting::AbstractGetter<sad::Scene*, unsigned int>("layer"));
    get->add(new scripting::AbstractGetter<sad::Scene*, unsigned long long>("majorid"));
    get->add(new scripting::AbstractGetter<sad::Scene*, unsigned long long>("minorid"));
    scenes->setProperty("get",  static_cast<dukpp03::qt::Callable*>(get)); // E.scenes.get

    m_global_value->setProperty("scenes", scenes);


    bool b = m_ctx->eval(
        "E.scenes.attr = function() {"
        "   if (arguments.length == 2)"
        "   {"
        "       return E.scenes.get(arguments[0], arguments[1]);"
        "   }"
        "   if (arguments.length == 3)"
        "   {"
        "       E.scenes.set(arguments[0], arguments[1], arguments[2]); return E.scenes;"
        "   }"
        "   throw new Error(\"Specify 2 or 3 arguments\");"
        "};"
    );
    assert(b);
}


void scripting::Scripting::initSceneNodesBindings()
{
    dukpp03::qt::JSObject* scenenodes = new dukpp03::qt::JSObject();
    scenenodes->setProperty("list", dukpp03::qt::make_function::from(scripting::scenenodes::list));
    scenenodes->setProperty("listScene", dukpp03::qt::make_function::from(scripting::scenenodes::listScene));
    scenenodes->setProperty("_addLabel", dukpp03::qt::curried1::from(this, scripting::scenenodes::_addLabel));
    scenenodes->setProperty("_addSprite2D", dukpp03::qt::curried1::from(this, scripting::scenenodes::_addSprite2D));
    scenenodes->setProperty("_addCustomObject", dukpp03::qt::curried1::from(this, scripting::scenenodes::_addCustomObject));
    scenenodes->setProperty("makeBackground", dukpp03::qt::curried1::from(this, scripting::scenenodes::makeBackground));
    scenenodes->setProperty("remove", dukpp03::qt::curried1::from(this, scripting::scenenodes::remove));
    scenenodes->setProperty("spanBetweenTwoPoints", dukpp03::qt::curried1::from(this, scripting::scenenodes::spanBetweenTwoPoints));

    dukpp03::qt::MultiMethod* set = new dukpp03::qt::MultiMethod();

    {
        // All props
        set->add(new scripting::scenenodes::FlagSetter(this, "visible", history::scenenodes::changeVisibility));
        set->add(new scripting::scenenodes::Setter<sad::String, history::scenenodes::ChangeName>(this, "name"));

        scripting::scenenodes::Setter<sad::Rect2D, history::scenenodes::ChangeArea>* area_setter = new scripting::scenenodes::Setter<sad::Rect2D, history::scenenodes::ChangeArea>(this, "area");
        std::function<dukpp03::Maybe<sad::String>(const sad::Rect2D&)> is_aabb = [](const sad::Rect2D& val) {
            dukpp03::Maybe<sad::String> result;
            if (sad::isAABB(val) == false)
            {
                result.setValue("Rectangle must be axis-aligned");
            }
            return result;
        };
        area_setter->addCondition(is_aabb);
        set->add(area_setter);

        set->add(new scripting::scenenodes::Setter<double, history::scenenodes::ChangeAngle>(this, "angle"));
        set->add(new scripting::scenenodes::Setter<sad::AColor, history::scenenodes::ChangeColor>(this, "color"));

        // sad::Label props
        scripting::scenenodes::Setter<unsigned int, history::label::ChangeFontSize>* font_size_setter = new scripting::scenenodes::Setter<unsigned int, history::label::ChangeFontSize>(this, "fontsize");
        std::function<dukpp03::Maybe<sad::String>(const unsigned long&)> is_greater_than_zero = [](const unsigned long& val) {
            dukpp03::Maybe<sad::String> result;
            if (val == 0)
            {
                result.setValue("Value must be greater than zero");
            }
            return result;
        };
        font_size_setter->addCondition(is_greater_than_zero);
        set->add(font_size_setter);
        set->add(new scripting::scenenodes::Setter<sad::String, history::label::ChangeText>(this, "text"));
        set->add(new scripting::scenenodes::Setter<float, history::label::ChangeLineSpacing>(this, "linespacing"));
        set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeMaximalLineWidth>(this, "maximallinewidth"));
        set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeOverflowStrategy>(this, "overflowstrategy"));
        set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeBreakText>(this, "breaktext"));
        set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeTextEllipsis>(this, "textellipsisposition"));
        set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeMaximalLinesCount>(this, "maximallinescount"));
        set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeOverflowStrategyForLines>(this, "overflowstrategyforlines"));
        set->add(new scripting::scenenodes::Setter<unsigned int, history::label::ChangeTextEllipsisForLines>(this, "textellipsispositionforlines"));
        set->add(new scripting::scenenodes::Setter<bool, history::label::ChangeHasFormatting>(this, "hasformatting"));

        scripting::scenenodes::Setter<sad::String, history::label::ChangeFontName>* font_setter = new scripting::scenenodes::Setter<sad::String, history::label::ChangeFontName>(this, "font");
        std::function<dukpp03::Maybe<sad::String>(const sad::String&)> font_exists = [](const sad::String& resource_name) {
            dukpp03::Maybe<sad::String> result;
            sad::resource::Resource* resource = sad::Renderer::ref()->tree("")->root()->resource(resource_name);
            bool valid = false;
            if (resource) {
                if (resource->metaData()->canBeCastedTo("sad::freetype::Font")
                    || resource->metaData()->canBeCastedTo("sad::TextureMappedFont")
                    || resource->metaData()->canBeCastedTo("sad::Font"))
                {
                    valid = true;
                }
            }
            if (!valid)
            {
                result.setValue(resource_name + " is not a font resource");
            }
            return result;
        };
        font_setter->addCondition(font_exists);
        set->add(font_setter);
        // sad::Sprite2D props
        set->add(new scripting::scenenodes::FlagSetter(this, "flipx", history::sprite2d::changeFlipX));
        set->add(new scripting::scenenodes::FlagSetter(this, "flipy", history::sprite2d::changeFlipY));
        set->add(new scripting::scenenodes::OptionsSetter(this));
        // sad::db::CustomObject props
        set->add(new scripting::scenenodes::SchemaSetter(this));
#define PUSH_SETTER(TYPE) set->add(new scripting::scenenodes::CustomSetter< TYPE >(this));
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
    }

    scenenodes->setProperty("set",  static_cast<dukpp03::qt::Callable*>(set)); // E.scenenodes.set
    dukpp03::qt::MultiMethod* get = new dukpp03::qt::MultiMethod();
    {
        // All
        get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::String>("name"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("layer"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned long long>("majorid"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned long long>("minorid"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned long long>("scene"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, bool>("visible"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::Rect2D>("area"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, double>("angle"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::AColor>("color"));
        // sad::Label props
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("fontsize"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::String>("text"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, float>("linespacing"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::String>("font"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("maximallinewidth"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("overflowstrategy"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("breaktext"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("textellipsisposition"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("maximallinescount"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("overflowstrategyforlines"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, unsigned int>("textellipsispositionforlines"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, bool>("hasformatting"));

        // sad::Sprite2D props
        get->add(new scripting::AbstractGetter<sad::SceneNode*, bool>("flipx"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, bool>("flipy"));
        get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::String>("options"));
        // sad::db::CustomObject props
        get->add(new scripting::AbstractGetter<sad::SceneNode*, sad::String>( "schema"));
    #define PUSH_GETTER(TYPE) get->add(scripting::scenenodes::custom_getter< TYPE >());
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
    }
    scenenodes->setProperty("get",  static_cast<dukpp03::qt::Callable*>(get)); // E.scenenodes.get

    m_global_value->setProperty("scenenodes", scenenodes);

    bool b = m_ctx->eval(
        "E.OverflowStrategy = { \"Visible\": 0, \"Hidden\": 1, \"Ellipsis\": 2 };"
        "E.BreakText = { \"Normal\": 0, \"BreakWord\": 1};"
        "E.TextEllipsisPosition = { \"Begin\": 0, \"Middle\": 1, \"End\": 2};"
    );
    assert( b );

    b = m_ctx->eval(
        "E.scenenodes.addLabel = function(o) {"
        "   if (\"fontsize\" in o == false)"
        "   {                              "
        "     o[\"fontsize\"] = 16;        "
        "   }                              "
        "   if (\"color\" in o == false)   "
        "   {"
        "      o[\"color\"] = aclr(255, 255, 255, 0);"
        "   }"
        "   if (\"name\" in o == false)   "
        "   {"
        "      o[\"name\"] = \"\";"
        "   }"
        "   return E.scenenodes._addLabel(o[\"scene\"], o[\"font\"], o[\"fontsize\"], o[\"text\"], o[\"name\"], o[\"point\"], o[\"color\"]);"
        "};"
        "E.scenenodes.addSprite2D = function(o) {"
        "   if (\"color\" in o == false)   "
        "   {"
        "      o[\"color\"] = aclr(255, 255, 255, 0);"
        "   }"
        "   if (\"name\" in o == false)   "
        "   {"
        "      o[\"name\"] = \"\";"
        "   }"
        "   return E.scenenodes._addSprite2D(o[\"scene\"], o[\"sprite\"], o[\"name\"], o[\"area\"], o[\"color\"]);"
        "};"
        "E.scenenodes.addCustomObject = function(o) {"
        "   if (\"fontsize\" in o == false)"
        "   {                              "
        "     o[\"fontsize\"] = 16;        "
        "   }                              "
        "   if (\"color\" in o == false)   "
        "   {"
        "      o[\"color\"] = aclr(255, 255, 255, 0);"
        "   }"
        "   if (\"name\" in o == false)   "
        "   {"
        "      o[\"name\"] = \"\";"
        "   }"
        "   return E.scenenodes._addCustomObject(o[\"scene\"], o[\"schema\"], o[\"name\"], o[\"fontsize\"], o[\"text\"],  o[\"area\"], o[\"color\"]);"
        "};"
        "E.scenenodes.attr = function() {"
        "   if (arguments.length == 2)"
        "   {"
        "       return E.scenenodes.get(arguments[0], arguments[1]);"
        "   }"
        "   if (arguments.length == 3)"
        "   {"
        "       return E.scenenodes.set(arguments[0], arguments[1], arguments[2]); return E.scenenodes;"
        "   }"
        "   throw new Error(\"Specify 2 or 3 arguments\");"
        "};"
    );
    assert( b );
}

// ReSharper disable once CppMemberFunctionMayBeConst
void scripting::Scripting::initLayoutGridBindings()
{
    dukpp03::qt::registerMetaType<scripting::layouts::ScriptableLengthValue>();
    dukpp03::qt::registerMetaType<scripting::layouts::ScriptableGrid>();
    dukpp03::qt::registerMetaType<scripting::layouts::ScriptableGridCell>();

    dukpp03::qt::JSObject* layouts = new dukpp03::qt::JSObject();

    m_global_value->setProperty("layouts", layouts);

    dukpp03::qt::JSObject* unit = new dukpp03::qt::JSObject();
    unit->setProperty("LU_Auto", static_cast<unsigned int>(sad::layouts::LU_Auto));
    unit->setProperty("LU_Pixels", static_cast<unsigned int>(sad::layouts::LU_Pixels));
    unit->setProperty("LU_Percents", static_cast<unsigned int>(sad::layouts::LU_Percents));
    layouts->setProperty("Unit", unit); // E.layouts.Unit
    layouts->setEvaluatedProperty("LU_Auto", "E.layouts.Unit.LU_Auto");
    layouts->setEvaluatedProperty("LU_Pixels", "E.layouts.Unit.LU_Pixels");
    layouts->setEvaluatedProperty("LU_Percents", "E.layouts.Unit.LU_Percents");

    dukpp03::qt::JSObject* horizontal_alignment = new dukpp03::qt::JSObject();
    horizontal_alignment->setProperty("LHA_Left", static_cast<unsigned int>(sad::layouts::LHA_Left));
    horizontal_alignment->setProperty("LHA_Middle", static_cast<unsigned int>(sad::layouts::LHA_Middle));
    horizontal_alignment->setProperty("LHA_Right", static_cast<unsigned int>(sad::layouts::LHA_Right));
    layouts->setProperty("HorizontalAlignment", horizontal_alignment); // E.layouts.HorizontalAlignment
    layouts->setEvaluatedProperty("LHA_Left", "E.layouts.HorizontalAlignment.LHA_Left");
    layouts->setEvaluatedProperty("LHA_Middle", "E.layouts.HorizontalAlignment.LHA_Middle");
    layouts->setEvaluatedProperty("LHA_Right", "E.layouts.HorizontalAlignment.LHA_Right");

    dukpp03::qt::JSObject* vertical_alignment = new dukpp03::qt::JSObject();
    vertical_alignment->setProperty("LVA_Top", static_cast<unsigned int>(sad::layouts::LVA_Top));
    vertical_alignment->setProperty("LVA_Middle", static_cast<unsigned int>(sad::layouts::LVA_Middle));
    vertical_alignment->setProperty("LVA_Bottom", static_cast<unsigned int>(sad::layouts::LVA_Bottom));
    layouts->setProperty("VerticalAlignment", vertical_alignment); // E.layouts.VerticalAlignment
    layouts->setEvaluatedProperty("LVA_Top", "E.layouts.VerticalAlignment.LVA_Top");
    layouts->setEvaluatedProperty("LVA_Middle", "E.layouts.VerticalAlignment.LVA_Middle");
    layouts->setEvaluatedProperty("LVA_Bottom", "E.layouts.VerticalAlignment.LVA_Bottom");

    dukpp03::qt::JSObject* stacking_type = new dukpp03::qt::JSObject();
    stacking_type->setProperty("LST_Horizontal", static_cast<unsigned int>(sad::layouts::LST_Horizontal));
    stacking_type->setProperty("LST_Vertical", static_cast<unsigned int>(sad::layouts::LST_Vertical));
    stacking_type->setProperty("LST_NoStacking", static_cast<unsigned int>(sad::layouts::LST_NoStacking));
    layouts->setProperty("StackingType", stacking_type); // E.layouts.StackingType
    layouts->setEvaluatedProperty("LST_Horizontal", "E.layouts.StackingType.LST_Horizontal");
    layouts->setEvaluatedProperty("LST_Vertical", "E.layouts.StackingType.LST_Vertical");
    layouts->setEvaluatedProperty("LST_NoStacking", "E.layouts.StackingType.LST_NoStacking");

    scripting::Scripting* scriptable_me = this;
    std::function<scripting::layouts::ScriptableLengthValue*(unsigned int, double)> length_value = [scriptable_me](unsigned int unit, double value)
    {
        if ((unit != sad::layouts::LU_Auto) && (unit != sad::layouts::LU_Percents) && (unit != sad::layouts::LU_Pixels))
        {
            scriptable_me->context()->throwError("Argument 1 must have sad::layouts::Unit type");
            throw new dukpp03::ArgumentException();
        }
        return new scripting::layouts::ScriptableLengthValue(static_cast<sad::layouts::Unit>(unit), value, scriptable_me);
    };
    layouts->setProperty("LengthValue", static_cast<dukpp03::qt::Callable*>(dukpp03::qt::make_lambda::from(length_value)));

    bool b  = m_ctx->eval(
        "E.layouts.Auto = function() { return E.layouts.LengthValue(E.layouts.Unit.LU_Auto, 0); };"
        "E.layouts.Pixels = function(a) { if (typeof a != \"number\") throw \"E.layouts.Pixels: first argument should be numeric\"; return E.layouts.LengthValue(E.layouts.Unit.LU_Pixels, a); };"
        "E.layouts.Percents = function(a) { if (typeof a != \"number\") throw \"E.layouts.Percents: first argument should be numeric\"; return E.layouts.LengthValue(E.layouts.Unit.LU_Percents, a); };"
    );
    assert( b );

    layouts->setProperty("list", dukpp03::qt::make_function::from(scripting::layouts::list)); // E.layouts.list
    layouts->setProperty("_query", dukpp03::qt::curried1::from(this, scripting::layouts::_query)); // E.layouts._query

    dukpp03::qt::MultiMethod* add = new dukpp03::qt::MultiMethod();
    {
        add->add(dukpp03::qt::curried1::from(this, scripting::layouts::add));
        scripting::Scripting* local_me = this;
        std::function<scripting::layouts::ScriptableGrid*()> add_no_args = [local_me]() {
            return scripting::layouts::add(local_me, "");
        };
        add->add(dukpp03::qt::make_lambda::from(add_no_args));
    }
    layouts->setProperty("add", static_cast<dukpp03::qt::Callable*>(add)); // E.scenenodes.add
    layouts->setProperty("remove", dukpp03::qt::curried1::from(this, scripting::layouts::remove)); // E.layouts.remove
    layouts->setProperty("parent", dukpp03::qt::curried1::from(this, scripting::layouts::parent)); // E.layouts.parent

    {
        dukpp03::qt::ClassBinding* binding = new dukpp03::qt::ClassBinding();
        binding->addMethod("setArea", dukpp03::qt::bind_method::from(&scripting::layouts::ScriptableGrid::setArea));
        binding->addMethod("findChild", dukpp03::qt::bind_method::from(&scripting::layouts::ScriptableGrid::findChild));
        binding->addMethod("cell", dukpp03::qt::bind_method::from(&scripting::layouts::ScriptableGrid::cell));
        binding->addMethod("children", dukpp03::qt::bind_method::from(&scripting::layouts::ScriptableGrid::children));
        binding->registerMetaObject<scripting::layouts::ScriptableGrid>();
        m_ctx->addClassBinding("scripting::layouts::ScriptableGrid", binding);
    }

    {
        dukpp03::qt::ClassBinding* binding = new dukpp03::qt::ClassBinding();
        binding->addMethod("children", dukpp03::qt::bind_method::from(&scripting::layouts::ScriptableGridCell::children));
        binding->addMethod("findChild", dukpp03::qt::bind_method::from(&scripting::layouts::ScriptableGridCell::findChild));
        binding->addMethod("addChild", dukpp03::qt::bind_method::from(&scripting::layouts::ScriptableGridCell::addChild));
        binding->registerMetaObject<scripting::layouts::ScriptableGridCell>();
        m_ctx->addClassBinding("scripting::layouts::ScriptableGridCell", binding);
    }

    b = m_ctx->eval(
        "E.layouts.query = function(a) {  try { return E.layouts._query(a); } catch(e) { return null; } };"
    );
    assert( b );
    
}

void scripting::Scripting::initWaysBindings()
{
    scripting::ways::init(this, m_global_value);
}


void scripting::Scripting::initDialoguesBindings()
{
    scripting::dialogues::init(this, m_global_value);
}


void scripting::Scripting::initAnimationsBindings()
{
    m_animations_value = scripting::animations::init(this, m_global_value);
}

void scripting::Scripting::initAnimationInstanceBindings(QScriptValue& v)
{
    /*
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
        "   if (\"name\" in o == false)    "
        "   {                              "
        "     o[\"name\"] = \"\";          "
        "   }                              "
        "   if (\"object\" in o == false)  "
        "   {                              "
        "      o[\"object\"] = 0;          "
        "   }                              "
        "   if (\"starttime\" in o == false)    "
        "   {                              "
        "      o[\"starttime\"] = 0;            "
        "   }                              "
        "   return E.animations.instances._addInstance(o[\"name\"], o[\"animationid\"], o[\"animationname\"], o[\"object\"], o[\"starttime\"]);"
        "};"
        "E.animations.instances.addWayInstance = function(o) {"
        "   if (typeof o != \"object\")    "
        "   {                              "
        "      o = {};                     "
        "   }                              "
        "   if (\"name\" in o == false)    "
        "   {                              "
        "     o[\"name\"] = \"\";          "
        "   }                              "
        "   if (\"way\" in o == false)     "
        "   {                              "
        "      o[\"way\"] = 0;             "
        "   }                              "
        "   if (\"object\" in o == false)  "
        "   {                              "
        "      o[\"object\"] = 0;          "
        "   }                              "
        "   if (\"starttime\" in o == false)    "
        "   {                              "
        "      o[\"starttime\"] = 0;            "
        "   }                              "
        "   return E.animations.instances._addWayInstance(o[\"name\"], o[\"way\"], o[\"object\"], o[\"starttime\"]);"
        "};"
        "E.animations.instances.attr = function() {"
        "   if (arguments.length == 2)"
        "   {"
        "       return E.animations.instances.get(arguments[0], arguments[1]);"
        "   }"
        "   if (arguments.length == 3)"
        "   {"
        "       return E.animations.instances.set(arguments[0], arguments[1], arguments[2]);"
        "   }"
        "   throw new Error(\"Specify 2 or 3 arguments\");"
        "};"
    );
    */
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


    /*
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
    */
    v.property("animations").setProperty("groups", groups, m_flags);


    m_engine->evaluate(
         "E.animations.groups.add = function(o) {"
        "   if (typeof o != \"object\")    "
        "   {                              "
        "      o = {};                     "
        "   }                              "
        "   if (\"name\" in o == false)    "
        "   {                              "
        "     o[\"name\"] = \"\";          "
        "   }                              "
        "   if (\"looped\" in o == false)  "
        "   {                              "
        "      o[\"looped\"] = false;      "
        "   }                              "
        "   return E.animations.groups._add(o[\"name\"], o[\"looped\"]);"
        "};"
        "E.animations.groups.attr = function() {"
        "   if (arguments.length == 2)"
        "   {"
        "       return E.animations.groups.get(arguments[0], arguments[1]);"
        "   }"
        "   if (arguments.length == 3)"
        "   {"
        "       return E.animations.groups.set(arguments[0], arguments[1], arguments[2]);"
        "   }"
        "   throw new Error(\"Specify 2 or 3 arguments\");"
        "};"
    );
}

void scripting::Scripting::copyProperties(scripting::Scripting::CopyPropertiesDirection direction)
{
    duk_context* c = m_ctx->context();
    if (direction == scripting::Scripting::SSC_CPD_FROM_GLOBAL_TO_HEAP)
    {
        duk_push_global_object(c); // Will have id -2
        duk_push_global_stash(c); // Will have id -1
    }
    else
    {
        duk_push_global_stash(c); // Will have id -2
        duk_push_global_object(c); // Will have id -1
    }

    this->copyObjectsOnStackRecursive(-2, -1);


    duk_pop_2(c);
}

void scripting::Scripting::copyObjectsOnStackRecursive(duk_idx_t source_id, duk_idx_t dest_id)
{
    duk_context* c = m_ctx->context();
    duk_enum(c, source_id, DUK_ENUM_OWN_PROPERTIES_ONLY); // -1
    while(duk_next(c, -1, 1))
    {
        // value -1, name -2, enum -3, destination object - dest_id-3, source_id -3
        if (duk_is_object(c, -1) && !duk_is_c_function(c, -1) && !duk_is_function(c, -1))
        {
            duk_push_object(c);
            // From value to new objects
            this->copyObjectsOnStackRecursive(-2, -1);
            // Pop old object
            duk_swap(c, -2, -1);
            duk_pop(c);
        }
        // put prop removes stack items, so we could do nothing after putting prop
        duk_put_prop(c, dest_id - 3);
    }
    duk_pop(c);
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
