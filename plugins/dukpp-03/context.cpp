#include "dukpp-03/context.h"

#include <sadpoint.h>
#include <sadsize.h>
#include <sadrect.h>
#include <sadcolor.h>
#include <p2d/vector.h>
#include <opengl.h>
#include <slurp.h>
#include <spit.h>
#include <mousecursor.h>

#include <dukpp-03/renderer.h>
#include <fpsinterpolation.h>
#include <objectdependentfpsinterpolation.h>
#include <util/fs.h>

#include <sadsleep.h>

#include <db/dbdatabase.h>
#include <db/dbtable.h>

#include <cstdio>

#include <pipeline/pipeline.h>


DECLARE_COMMON_TYPE(sad::dukpp03::CompiledFunction)

// ============================================ PUBLIC METHODS ============================================

sad::dukpp03::Context::Context(bool vanilla) : m_renderer(NULL), m_vanilla(vanilla)
{
    if (!m_vanilla)
    {
        this->initialize();
    }    
}

sad::dukpp03::Context::~Context()
{
    
}

bool sad::dukpp03::Context::evalFromFile(
    const std::string& path, 
    bool clean_heap,
    std::string* error
)
{
    // Test if file exists
    sad::String mpath = path.c_str();
    if (sad::util::fileExists(mpath.c_str()) == false)
    {
        sad::Renderer* r = m_renderer;
        if (!r)
        {
            r = sad::Renderer::ref();
        }
        if (sad::util::isAbsolutePath(mpath) == false)
        {
             mpath = util::concatPaths(r->executablePath(), mpath);
             if (sad::util::fileExists(mpath.c_str()) == false)
             {
                if (error) 
                {
                    *error = "File \"";
                    *error += path;
                    *error += "\" does not exist";
                }
                return false;
             }
        }
        else
        {
            if (error) 
            {
                *error = "File \"";
                *error += path;
                *error += "\" does not exist";
            }
            return false;
        }
    }
    m_running = true;
    startEvaluating();
    bool result = false;
    if (duk_peval_file(m_context, mpath.c_str()) != 0) 
    {
        if (duk_has_prop_string(m_context,  -1, "stack"))
        {
            duk_get_prop_string(m_context, -1, "stack");
            *error = duk_safe_to_string(m_context, -1);
            duk_pop(m_context);
        }
        else 
        {
            *error = duk_safe_to_string(m_context, -1);
        }
        duk_pop(m_context);
    } 
    else 
    {
        if (error)
        {
            *error = "";
        }
        result = true;
        if (clean_heap)
        {
            duk_pop(m_context);
        }
    }
    m_running = false;
    return result;
    
}

void sad::dukpp03::Context::setRenderer(sad::Renderer* r)
{
    m_renderer = r;
}

sad::Renderer* sad::dukpp03::Context::renderer() const
{
    if (!m_renderer)
    {
        return sad::Renderer::ref();
    }
    return m_renderer;
}

void sad::dukpp03::Context::reset()
{
    this->sad::dukpp03::BasicContext::reset();
    if (!m_vanilla)
    {
        this->initialize();
    }
}


bool sad::dukpp03::Context::addClassBinding(const std::string& name, ::dukpp03::ClassBinding<Self>* c)
{
    if (this->sad::dukpp03::BasicContext::addClassBinding(name, c))
    {
        if (name.find("*") == std::string::npos)
        {
            std::string ptrname = name;
            ptrname += " *";
            this->sad::dukpp03::BasicContext::addClassBinding(ptrname, new ::dukpp03::ClassBinding<Self>(*c));
        }
        return true;
    }
    return false;
}

// ============================================ PROTECTED METHODS ============================================

extern const std::string __context_eval_info;


static duk_ret_t isNativeObject(duk_context* ctx)
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

sad::String dumpNativeObject(const sad::db::Variant& v)
{
    sad::Maybe<sad::db::Object*> maybeobject = v.get<sad::db::Object*>();
    if (maybeobject.exists())
    {
        std::stringstream ss;
        ss << maybeobject.value()->serializableName();
        ss << "(" << maybeobject.value() << ")";
        std::string name = ss.str(); 
        return name;
    }
    else
    {
        std::stringstream ss;
        ss << v.typeName();
        ss << "(" << v.data() << ")";
        std::string name = ss.str(); 
        return name;
    }
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void sad::dukpp03::Context::initialize()
{
    if (sad::dukpp03::internal::conversion_table.converter("sad::dukpp03::Renderer *", "sad::Renderer *") == NULL)
    {
        sad::dukpp03::internal::conversion_table.declareImplicit<sad::dukpp03::Renderer*, sad::Renderer*>();
        sad::dukpp03::internal::conversion_table.declareImplicit<sad::Renderer*, sad::dukpp03::Renderer*>();
    }

    // Register SadInternalIsNativeObject function
    duk_push_global_object(m_context);
    duk_push_c_function(m_context, isNativeObject, 1);
    duk_int_t result = duk_peval_string(m_context, "new Function()");
    assert(result == 0);
    duk_dup(m_context, -1);
    duk_put_prop_string(m_context, -3, "prototype");
    duk_set_prototype(m_context, - 2);   
    duk_put_prop_string(m_context, -2 /*idx:global*/, "SadInternalIsNativeObject");
    duk_pop(m_context);  

    // Register SadInternalDumpNativeObject function
    this->registerCallable("SadInternalDumpNativeObject", sad::dukpp03::make_function::from(dumpNativeObject));

    this->exposePoint2D();
    this->exposePoint3D();
    this->exposePoint2I();
    this->exposePoint3I();
    this->exposeP2DVector();
    this->exposeSize2D();
    this->exposeSize2I();
    this->exposeRect2D();
    this->exposeRect2I();
    this->exposeColor();
    this->exposeAColor();
    this->exposeUtilFS();
    this->exposeSlurpSpit();
    this->exposeContext();
    this->exposeRenderer();
    exposeAPI(this);

    std::string error;
    bool ok =  this->eval(__context_eval_info, true, &error);
#ifdef CONFIG_DEBUG
    if (!ok)
    {
        printf("%s\n", error.c_str());
    }
#endif
    assert( ok );
}


void sad::dukpp03::Context::exposePoint2D()
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addConstructor<sad::Point2D>("SadPoint2D");
    c->addConstructor<sad::Point2D, double, double>("SadPoint2D");
    c->addAccessor("x", sad::dukpp03::bind_method::from(&sad::Point2D::x), sad::dukpp03::bind_method::from(&sad::Point2D::setX));
    c->addAccessor("y", sad::dukpp03::bind_method::from(&sad::Point2D::y), sad::dukpp03::bind_method::from(&sad::Point2D::setY));
    c->addMethod("distance", sad::dukpp03::bind_method::from(&sad::Point2D::distance));
    c->addCloneValueObjectMethodFor<sad::Point2D>();
    c->setPrototypeFunction("SadPoint2D");

    this->addClassBinding("sad::Point2D", c);
}

void sad::dukpp03::Context::exposePoint3D()
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addConstructor<sad::Point3D>("SadPoint3D");
    c->addConstructor<sad::Point3D, double, double, double>("SadPoint3D");
    c->addAccessor("x", sad::dukpp03::rebind_method::to<sad::Point3D>::from(&sad::Point3D::x), sad::dukpp03::rebind_method::to<sad::Point3D>::from(&sad::Point3D::setX));
    c->addAccessor("y", sad::dukpp03::rebind_method::to<sad::Point3D>::from(&sad::Point3D::y), sad::dukpp03::rebind_method::to<sad::Point3D>::from(&sad::Point3D::setY));
    c->addAccessor("z", sad::dukpp03::bind_method::from(&sad::Point3D::z), sad::dukpp03::bind_method::from(&sad::Point3D::setZ));
    c->addMethod("distance", sad::dukpp03::bind_method::from(&sad::Point3D::distance));
    c->addCloneValueObjectMethodFor<sad::Point3D>();
    c->setPrototypeFunction("SadPoint3D");

    this->addClassBinding("sad::Point3D", c);    
}

void sad::dukpp03::Context::exposePoint2I()
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addConstructor<sad::Point2I>("SadPoint2I");
    c->addConstructor<sad::Point2I, int, int>("SadPoint2I");
    c->addAccessor("x", sad::dukpp03::bind_method::from(&sad::Point2I::x), sad::dukpp03::bind_method::from(&sad::Point2I::setX));
    c->addAccessor("y", sad::dukpp03::bind_method::from(&sad::Point2I::y), sad::dukpp03::bind_method::from(&sad::Point2I::setY));
    c->addMethod("distance", sad::dukpp03::bind_method::from(&sad::Point2I::distance));
    c->addCloneValueObjectMethodFor<sad::Point2I>();
    c->setPrototypeFunction("SadPoint2I");

    this->addClassBinding("sad::Point2I", c);       
}

void sad::dukpp03::Context::exposePoint3I()
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addConstructor<sad::Point3I>("SadPoint3I");
    c->addConstructor<sad::Point3I, int, int, int>("SadPoint3I");
    c->addAccessor("x", sad::dukpp03::rebind_method::to<sad::Point3I>::from(&sad::Point2I::x), sad::dukpp03::rebind_method::to<sad::Point3I>::from(&sad::Point2I::setX));
    c->addAccessor("y", sad::dukpp03::rebind_method::to<sad::Point3I>::from(&sad::Point2I::y), sad::dukpp03::rebind_method::to<sad::Point3I>::from(&sad::Point2I::setY));
    c->addAccessor("z", sad::dukpp03::bind_method::from(&sad::Point3I::z), sad::dukpp03::bind_method::from(&sad::Point3I::setZ));
    c->addMethod("distance", sad::dukpp03::bind_method::from(&sad::Point3I::distance));
    c->addCloneValueObjectMethodFor<sad::Point3I>();
    c->setPrototypeFunction("SadPoint3I");

    this->addClassBinding("sad::Point3I", c);  
}


static sad::p2d::Vector local_ortho(const sad::p2d::Vector& v, int ortho)
{
    return sad::p2d::ortho(v, sad::p2d::OrthoVectorIndex(ortho));
}

void sad::dukpp03::Context::exposeP2DVector()
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addConstructor<sad::p2d::Vector>("SadP2DVector");
    c->addConstructor<sad::p2d::Vector, double, double>("SadP2DVector");
    c->addAccessor("x", sad::dukpp03::rebind_method::to<sad::p2d::Vector>::from(&sad::Point2D::x), sad::dukpp03::rebind_method::to<sad::p2d::Vector>::from(&sad::Point2D::setX));
    c->addAccessor("y", sad::dukpp03::rebind_method::to<sad::p2d::Vector>::from(&sad::Point2D::y), sad::dukpp03::rebind_method::to<sad::p2d::Vector>::from(&sad::Point2D::setY));
    c->addMethod("distance", sad::dukpp03::rebind_method::to<sad::p2d::Vector>::from(&sad::Point2D::distance));
    c->addCloneValueObjectMethodFor<sad::p2d::Vector>();
    c->setPrototypeFunction("Sad2DVector");

    this->registerCallable("SadP2DModulo", sad::dukpp03::make_function::from(sad::p2d::modulo));
    this->registerCallable("SadP2DBasis", sad::dukpp03::make_function::from(sad::p2d::basis));
    this->registerCallable("SadP2DUnit", sad::dukpp03::make_function::from(sad::p2d::unit));
    this->registerCallable("SadP2DScalar", sad::dukpp03::make_function::from(sad::p2d::scalar));
    this->registerCallable("SadP2DOrtho", sad::dukpp03::make_function::from(local_ortho));

    this->addClassBinding("sad::p2d::Vector", c);    
}

void  sad::dukpp03::Context::exposeSize2D()
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addConstructor<sad::Size2D>("SadSize2D");
    c->addConstructor<sad::Size2D, double, double>("SadSize2D");
    c->addAccessor("width", sad::dukpp03::getter::from(&sad::Size2D::Width), sad::dukpp03::setter::from(&sad::Size2D::Width));
    c->addAccessor("height", sad::dukpp03::getter::from(&sad::Size2D::Height), sad::dukpp03::setter::from(&sad::Size2D::Height));
    c->addCloneValueObjectMethodFor<sad::Size2D>();
    c->setPrototypeFunction("SadSize2D");

    this->addClassBinding("sad::Size2D", c);
}

void  sad::dukpp03::Context::exposeSize2I()
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addConstructor<sad::Size2I>("SadSize2I");
    c->addConstructor<sad::Size2I, int, int>("SadSize2I");
    c->addAccessor("width", sad::dukpp03::getter::from(&sad::Size2I::Width), sad::dukpp03::setter::from(&sad::Size2I::Width));
    c->addAccessor("height", sad::dukpp03::getter::from(&sad::Size2I::Height), sad::dukpp03::setter::from(&sad::Size2I::Height));
    c->addCloneValueObjectMethodFor<sad::Size2I>();
    c->setPrototypeFunction("SadSize2I");

    this->addClassBinding("sad::Size2I", c);   
}


void sad::dukpp03::Context::exposeRect2D()
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addConstructor<sad::Rect2D>("SadRect2D");
    c->addConstructor<sad::Rect2D, sad::Point2D, sad::Point2D>("SadRect2D");
    c->addConstructor<sad::Rect2D, sad::Point2D, sad::Point2D, sad::Point2D, sad::Point2D>("SadRect2D");
    c->addConstructor<sad::Rect2D, double, double, double, double>("SadRect2D");
    c->addMethod("width", sad::dukpp03::bind_method::from(&sad::Rect2D::width));
    c->addMethod("height", sad::dukpp03::bind_method::from(&sad::Rect2D::height));
    c->addMethod("p0", sad::dukpp03::bind_method::from(&sad::Rect2D::p0));
    c->addMethod("p1", sad::dukpp03::bind_method::from(&sad::Rect2D::p1));
    c->addMethod("p2", sad::dukpp03::bind_method::from(&sad::Rect2D::p2));
    c->addMethod("p3", sad::dukpp03::bind_method::from(&sad::Rect2D::p3));
    c->addCloneValueObjectMethodFor<sad::Rect2D>();
    c->setPrototypeFunction("SadRect2D");

    this->addClassBinding("sad::Rect2D", c);    
}

void sad::dukpp03::Context::exposeRect2I()
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addConstructor<sad::Rect2I>("SadRect2I");
    c->addConstructor<sad::Rect2I, sad::Point2I, sad::Point2I>("SadRect2I");
    c->addConstructor<sad::Rect2I, sad::Point2I, sad::Point2I, sad::Point2I, sad::Point2I>("SadRect2I");
    c->addConstructor<sad::Rect2I, double, double, double, double>("SadRect2I");
    c->addMethod("width", sad::dukpp03::bind_method::from(&sad::Rect2I::width));
    c->addMethod("height", sad::dukpp03::bind_method::from(&sad::Rect2I::height));
    c->addMethod("p0", sad::dukpp03::bind_method::from(&sad::Rect2I::p0));
    c->addMethod("p1", sad::dukpp03::bind_method::from(&sad::Rect2I::p1));
    c->addMethod("p2", sad::dukpp03::bind_method::from(&sad::Rect2I::p2));
    c->addMethod("p3", sad::dukpp03::bind_method::from(&sad::Rect2I::p3));
    c->addCloneValueObjectMethodFor<sad::Rect2I>();
    c->setPrototypeFunction("SadRect2I");

    this->addClassBinding("sad::Rect2I", c);       
}

static bool ___file_exists(const sad::String& s)
{
    return sad::util::fileExists(s.c_str());
}

void sad::dukpp03::Context::exposeColor()
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addConstructor<sad::Color>("SadColor");
    c->addConstructor<sad::Color, unsigned char, unsigned char, unsigned char>("SadColor");
    c->addAccessor("r", sad::dukpp03::bind_method::from(&sad::Color::r), sad::dukpp03::bind_method::from(&sad::Color::setR));
    c->addAccessor("g", sad::dukpp03::bind_method::from(&sad::Color::g), sad::dukpp03::bind_method::from(&sad::Color::setG));
    c->addAccessor("b", sad::dukpp03::bind_method::from(&sad::Color::r), sad::dukpp03::bind_method::from(&sad::Color::setB));
    c->addCloneValueObjectMethodFor<sad::Color>();
    c->setPrototypeFunction("SadColor");

    this->addClassBinding("sad::Color", c); 
}

void sad::dukpp03::Context::exposeAColor()
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addConstructor<sad::AColor>("SadAColor");
    c->addConstructor<sad::AColor, unsigned char, unsigned char, unsigned char>("SadAColor");
    c->addConstructor<sad::AColor, unsigned char, unsigned char, unsigned char, unsigned char>("SadAColor");
    c->addAccessor("r", sad::dukpp03::rebind_method::to<sad::AColor>::from(&sad::AColor::r), sad::dukpp03::rebind_method::to<sad::AColor>::from(&sad::AColor::setR));
    c->addAccessor("g", sad::dukpp03::rebind_method::to<sad::AColor>::from(&sad::AColor::g), sad::dukpp03::rebind_method::to<sad::AColor>::from(&sad::AColor::setG));
    c->addAccessor("b", sad::dukpp03::rebind_method::to<sad::AColor>::from(&sad::AColor::b), sad::dukpp03::rebind_method::to<sad::AColor>::from(&sad::AColor::setB));
    c->addAccessor("a", sad::dukpp03::bind_method::from(&sad::AColor::a), sad::dukpp03::bind_method::from(&sad::AColor::setA));
    c->addCloneValueObjectMethodFor<sad::AColor>();
    c->setPrototypeFunction("SadAColor");

    this->addClassBinding("sad::AColor", c); 
}

void sad::dukpp03::Context::exposeUtilFS()
{
    this->registerCallable("SadUtilPathDelimiter", sad::dukpp03::make_function::from(sad::util::pathDelimiter));
    this->registerCallable("SadUtilCanonicalizePath", sad::dukpp03::make_function::from(sad::util::canonicalizePath));
    this->registerCallable("SadUtilIsAbsolutePath", sad::dukpp03::make_function::from(sad::util::isAbsolutePath));
    this->registerCallable("SadUtilConcatPaths", sad::dukpp03::make_function::from(sad::util::concatPaths));
    this->registerCallable("SadUtilFolder", sad::dukpp03::make_function::from(sad::util::folder));
    this->registerCallable("SadUtilFileExists", sad::dukpp03::make_function::from(___file_exists));
}


static duk_ret_t __slurp(duk_context* c)
{
    sad::dukpp03::Context* ctx = static_cast<sad::dukpp03::Context*>(sad::dukpp03::BasicContext::getContext(c));
    ::dukpp03::Maybe<sad::String> maybefile = ::dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(ctx, 0);
    if (maybefile.exists())
    {
        sad::Maybe<sad::String> maybecontent = sad::slurp(maybefile.value(), ctx->renderer());
        if (maybecontent.exists())
        {
            ::dukpp03::PushValue<sad::String, sad::dukpp03::BasicContext>::perform(ctx, maybecontent.value());
        }
        else
        {
            duk_push_null(c);
        }
        return 1;
    }
    
    ctx->throwInvalidTypeError(1, "sad::String");
    return 0;
}




static duk_ret_t __spit(duk_context* c)
{
    sad::dukpp03::Context* ctx = static_cast<sad::dukpp03::Context*>(sad::dukpp03::BasicContext::getContext(c));
    ::dukpp03::Maybe<sad::String> maybefile = ::dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(ctx, 0);
    if (maybefile.exists())
    {
        ::dukpp03::Maybe<sad::String> maybecontent = ::dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(ctx, 1);
        if (maybecontent.exists())
        {
            bool result = sad::spit(maybefile.value(), maybecontent.value(), ctx->renderer());
            ::dukpp03::PushValue<bool, sad::dukpp03::BasicContext>::perform(ctx, result);
            return 1;
        }
        ctx->throwInvalidTypeError(2, "sad::String");
        return 0;
    }      
    ctx->throwInvalidTypeError(1, "sad::String");
    return 0;
}

void sad::dukpp03::Context::exposeSlurpSpit()
{
    this->registerNativeFunction("SadSlurp", __slurp, 1);
    this->registerNativeFunction("SadSpit", __spit, 2);
}


static duk_ret_t __eval(duk_context* c)
{
    sad::dukpp03::Context* ctx = static_cast<sad::dukpp03::Context*>(sad::dukpp03::BasicContext::getContext(c));
    ::dukpp03::Maybe<sad::dukpp03::Context*> maybectx = ::dukpp03::GetValue<sad::dukpp03::Context*, sad::dukpp03::BasicContext>::perform(ctx, 0);
    if (maybectx.exists())
    {
        ::dukpp03::Maybe<sad::String> maybecontent = ::dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(ctx, 1);
        if (maybecontent.exists())
        {
            std::string error;
            bool result = maybectx.value()->eval(maybecontent.value(), true, &error);
            if (!result)
            {
                ctx->throwError(error);
            }
            return 0;
        }
        ctx->throwInvalidTypeError(2, "sad::String");
        return 0;
    }      
    ctx->throwInvalidTypeError(1, "sad::dukpp03::Context*");
    return 0;
}


static duk_ret_t __eval_from_file(duk_context* c)
{
    sad::dukpp03::Context* ctx = static_cast<sad::dukpp03::Context*>(sad::dukpp03::BasicContext::getContext(c));
    ::dukpp03::Maybe<sad::dukpp03::Context*> maybectx = ::dukpp03::GetValue<sad::dukpp03::Context*, sad::dukpp03::BasicContext>::perform(ctx, 0);
    if (maybectx.exists())
    {
        ::dukpp03::Maybe<sad::String> maybecontent = ::dukpp03::GetValue<sad::String, sad::dukpp03::BasicContext>::perform(ctx, 1);
        if (maybecontent.exists())
        {
            std::string error;
            bool result = maybectx.value()->evalFromFile(maybecontent.value(), true, &error);
            if (!result)
            {
                ctx->throwError(error);
            }
            return 0;
        }
        ctx->throwInvalidTypeError(2, "sad::String");
        return 0;
    }      
    ctx->throwInvalidTypeError(1, "sad::dukpp03::Context*");
    return 0;
}

void sad::dukpp03::Context::exposeContext()
{
    this->registerNativeFunction("SadContextEval", __eval, 2);
    this->registerNativeFunction("SadContextEvalFromFile", __eval_from_file, 2); 

    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding();
    c->addObjectConstructor<sad::dukpp03::Context>("SadContext");
    c->addMethod("renderer", sad::dukpp03::bind_method::from(&sad::dukpp03::Context::renderer));
    c->addMethod("setRenderer", sad::dukpp03::bind_method::from(&sad::dukpp03::Context::setRenderer));
    c->setPrototypeFunction("SadContext");

    this->addClassBinding("sad::dukpp03::Context", c);       

    this->registerGlobal("context", this);
}

template<typename T> static void ___make_interpolation_default(T* a)
{
    a->setFPSInterpolation(new sad::FPSInterpolation);
}

template<typename T> static void ___make_interpolation_object_dependent(T* a)
{
    a->setFPSInterpolation(new sad::ObjectDependentFPSInterpolation);
}

void sad::dukpp03::Context::exposeRenderer()
{
    sad::dukpp03::ClassBinding* c = new sad::dukpp03::ClassBinding(); 
    c->addMethod("init", sad::dukpp03::bind_method::from(&sad::Renderer::init));
    c->addMethod("settings", sad::dukpp03::bind_method::from(&sad::Renderer::settings));
    c->addMethod("run", sad::dukpp03::bind_method::from(&sad::Renderer::run));
    c->addMethod("quit", sad::dukpp03::bind_method::from(&sad::Renderer::quit));
    c->addMethod("makeFixedSize", sad::dukpp03::bind_method::from(&sad::Renderer::makeFixedSize));
    c->addMethod("makeResizeable", sad::dukpp03::bind_method::from(&sad::Renderer::makeResizeable));
    c->addMethod("running", sad::dukpp03::bind_method::from(&sad::Renderer::running));
    c->addMethod("hasValidContext", sad::dukpp03::bind_method::from(&sad::Renderer::hasValidContext));
    c->addMethod("setCursorPosition", sad::dukpp03::bind_method::from(&sad::Renderer::setCursorPosition));
    c->addMethod("cursorPosition", sad::dukpp03::bind_method::from(&sad::Renderer::cursorPosition));
    c->addMethod("render", sad::dukpp03::bind_method::from(&sad::Renderer::render));
    c->addMethod("setPrimitiveRenderer", sad::dukpp03::bind_method::from(&sad::Renderer::setPrimitiveRenderer));
    c->addMethod("textureLoader", sad::dukpp03::bind_method::from(&sad::Renderer::textureLoader));
    c->addMethod("setTextureLoader", sad::dukpp03::bind_method::from(&sad::Renderer::setTextureLoader));
    c->addMethod("isOwnThread", sad::dukpp03::bind_method::from(&sad::Renderer::isOwnThread));
    c->addMethod("globalTranslationOffset", sad::dukpp03::bind_method::from(&sad::Renderer::globalTranslationOffset));
    c->addMethod("setGlobalTranslationOffset", sad::dukpp03::bind_method::from(&sad::Renderer::setGlobalTranslationOffset));
    c->addMethod("setWindowTitle", sad::dukpp03::bind_method::from(&sad::Renderer::setWindowTitle));
    c->addMethod("toggleFullscreen", sad::dukpp03::bind_method::from(&sad::Renderer::toggleFullscreen));
    c->addMethod("fps", sad::dukpp03::bind_method::from(&sad::Renderer::fps));
    c->addMethod("executablePath", sad::dukpp03::bind_method::from(&sad::Renderer::executablePath));
    c->addMethod("lockRendering", sad::dukpp03::bind_method::from(&sad::Renderer::lockRendering));
    c->addMethod("unlockRendering", sad::dukpp03::bind_method::from(&sad::Renderer::unlockRendering));
    c->addMethod("error", sad::dukpp03::bind_method::from(&sad::Renderer::error));
    c->addMethod("information", sad::dukpp03::bind_method::from(&sad::Renderer::information));
    c->addMethod("warning", sad::dukpp03::bind_method::from(&sad::Renderer::warning));
    c->addMethod("window", sad::dukpp03::bind_method::from(&sad::Renderer::window));
    c->addMethod("log", sad::dukpp03::bind_method::from(&sad::Renderer::log));
    c->addMethod("cursor", sad::dukpp03::bind_method::from(&sad::Renderer::cursor));
    c->addMethod("setCursor", sad::dukpp03::bind_method::from(&sad::Renderer::setCursor));
    c->addMethod("opengl", sad::dukpp03::bind_method::from(&sad::Renderer::opengl));
    c->addMethod("tree", sad::dukpp03::bind_method::from(&sad::Renderer::tree));
    c->addMethod("addTree", sad::dukpp03::bind_method::from(&sad::Renderer::addTree));
    c->addMethod("takeTree", sad::dukpp03::bind_method::from(&sad::Renderer::takeTree));
    c->addMethod("removeTree", sad::dukpp03::bind_method::from(&sad::Renderer::removeTree));
    c->addMethod("tryLoadResources", sad::dukpp03::bind_method::from(&sad::Renderer::tryLoadResources));
    c->addMethod("addDatabase", sad::dukpp03::bind_method::from(&sad::Renderer::addDatabase));
    c->addMethod("removeDatabase", sad::dukpp03::bind_method::from(&sad::Renderer::removeDatabase));
    c->addMethod("database", sad::dukpp03::bind_method::from(&sad::Renderer::database));
    c->addMethod("pipeline", sad::dukpp03::bind_method::from(&sad::Renderer::pipeline));
    
    c->setPrototypeFunction("sad.Renderer");

    this->addClassBinding("sad::Renderer", c); 

    this->registerCallable("SadRendererRef", sad::dukpp03::make_function::from(sad::Renderer::ref));

    sad::dukpp03::ClassBinding* cext = new sad::dukpp03::ClassBinding(); 
    cext->addObjectConstructor<sad::dukpp03::Renderer>("SadRenderer");
    cext->addMethod("init", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::init));
    cext->addMethod("settings", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::settings));
    cext->addMethod("run", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::run));
    cext->addMethod("quit", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::quit));
    cext->addMethod("makeFixedSize", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::makeFixedSize));
    cext->addMethod("makeResizeable", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::makeResizeable));
    cext->addMethod("running", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::running));
    cext->addMethod("hasValidContext", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::hasValidContext));
    cext->addMethod("setCursorPosition", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::setCursorPosition));
    cext->addMethod("cursorPosition", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::cursorPosition));
    cext->addMethod("render", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::render));
    cext->addMethod("setPrimitiveRenderer", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::setPrimitiveRenderer));
    cext->addMethod("textureLoader", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::textureLoader));
    cext->addMethod("setTextureLoader", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::setTextureLoader));
    cext->addMethod("isOwnThread", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::isOwnThread));
    cext->addMethod("globalTranslationOffset", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::globalTranslationOffset));
    cext->addMethod("setGlobalTranslationOffset", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::setGlobalTranslationOffset));
    cext->addMethod("setWindowTitle", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::setWindowTitle));
    cext->addMethod("toggleFullscreen", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::toggleFullscreen));
    cext->addMethod("fps", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::fps));
    cext->addMethod("executablePath", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::executablePath));
    cext->addMethod("lockRendering", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::lockRendering));
    cext->addMethod("unlockRendering", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::unlockRendering));
    cext->addMethod("error", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::error));
    cext->addMethod("information", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::information));
    cext->addMethod("warning", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::warning));
    cext->addMethod("window", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::window));
    cext->addMethod("log", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::log));
    cext->addMethod("cursor", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::cursor));
    cext->addMethod("setCursor", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::setCursor));
    cext->addMethod("opengl", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::opengl));
    cext->addMethod("tree", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::tree));
    cext->addMethod("addTree", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::addTree));
    cext->addMethod("takeTree", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::takeTree));
    cext->addMethod("removeTree", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::removeTree));
    cext->addMethod("tryLoadResources", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::tryLoadResources));
    cext->addMethod("addDatabase", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::addDatabase));
    cext->addMethod("removeDatabase", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::removeDatabase));
    cext->addMethod("database", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::database));
    cext->addMethod("pipeline", sad::dukpp03::rebind_method::to<sad::dukpp03::Renderer>::from(&sad::Renderer::pipeline));
    cext->setPrototypeFunction("sad.Renderer");

    this->addClassBinding("sad::dukpp03::Renderer", cext); 

    sad::dukpp03::ClassBinding* prenderer = new sad::dukpp03::ClassBinding();
    prenderer->addMethod("line", sad::dukpp03::bind_method::from(&sad::PrimitiveRenderer::line));
    prenderer->addMethod("rectangle", sad::dukpp03::bind_method::from(&sad::PrimitiveRenderer::rectangle));

    this->addClassBinding("sad::PrimitiveRenderer", prenderer); 

    ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>* makeFPSInterpolationDefault = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
    makeFPSInterpolationDefault->add(sad::dukpp03::make_function::from(___make_interpolation_default<sad::Renderer>));
    makeFPSInterpolationDefault->add(sad::dukpp03::make_function::from(___make_interpolation_default<sad::dukpp03::Renderer>));
    this->registerCallable("SadInternalMakeFPSInterpolationDefault", makeFPSInterpolationDefault);

    ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>* makeFPSInterpolationObjectDependent = new ::dukpp03::MultiMethod<sad::dukpp03::BasicContext>();
    makeFPSInterpolationObjectDependent->add(sad::dukpp03::make_function::from(___make_interpolation_object_dependent<sad::Renderer>));
    makeFPSInterpolationObjectDependent->add(sad::dukpp03::make_function::from(___make_interpolation_object_dependent<sad::dukpp03::Renderer>));
    this->registerCallable("SadInternalMakeFPSInterpolationObjectDependent", makeFPSInterpolationObjectDependent);


    // Expose sad::sleep
    this->registerCallable("SadSleep", sad::dukpp03::make_function::from(sad::sleep));
}

DECLARE_COMMON_TYPE(sad::dukpp03::Context);
