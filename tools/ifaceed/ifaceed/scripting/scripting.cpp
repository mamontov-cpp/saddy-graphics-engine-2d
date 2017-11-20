#include "scripting.h"
#include "scriptinghelptext.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "abstractgetter.h"
#include "abstractsetter.h"
#include "queryresource.h"


#include "../scriptinghelp.h"

#include "../core/editor.h"

#include "../gui/codeedit/codeedit.h"

#include "../gui/uiblocks/uiblocks.h"
#include "../gui/uiblocks/uiconsoleblock.h"


#include "database/databasebindings.h"
#include "scenes/scenesbindings.h"
#include "scenenodes/scenenodesbindings.h"
#include "layouts/gridbindings.h"
#include "ways/waysbindings.h"
#include "dialogues/dialoguesbindings.h"
#include "animations/animationsbindings.h"
#include "instances/instancesbindings.h"
#include "groups/groupsbindings.h"

#include <QFileDialog>
#include <QDebug>
#include <QTextStream>
#include <QTextEdit>

#include <scene.h>

#include "lambda.h"
#include "function.h"

Q_DECLARE_METATYPE(sad::Vector<unsigned long long>) //-V566

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
        cblk->txtConsoleResults->append(QString("<font color=\"black\">") + c + "</font>");
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
    
}

scripting::Scripting::~Scripting()
{
    delete m_ctx;
    m_global_value->delRef();
}

void scripting::Scripting::setEditor(core::Editor* editor)
{
    m_editor = editor;
    this->initSadTypeConstructors();
    scripting::database::init(this, m_global_value);
    scripting::scenes::init(this, m_global_value);
    scripting::scenenodes::init(this, m_global_value);
    scripting::layouts::init(this, m_global_value);
    scripting::ways::init(this, m_global_value);
    scripting::dialogues::init(this, m_global_value);
    m_animations_value = scripting::animations::init(this, m_global_value);
    scripting::instances::init(this, m_animations_value);
    scripting::groups::init(this, m_animations_value);

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
