/*! \file context.h
  
    Defines a context, that can be used in Saddy
 */
#pragma once
#include "basiccontext.h"
#include "getvalue.h"
#include "pushvalue.h"
#include "classbinding.h"
#include "../refcountable.h"

namespace sad
{

class Renderer;

namespace dukpp03
{

typedef ::dukpp03::make_fun<sad::dukpp03::BasicContext> make_function;
typedef ::dukpp03::register_constructor<sad::dukpp03::BasicContext> register_constructor;
typedef ::dukpp03::make_method<sad::dukpp03::BasicContext> make_method;
typedef ::dukpp03::bind_method<sad::dukpp03::BasicContext> bind_method;
typedef ::dukpp03::bind_explicit_method<sad::dukpp03::BasicContext> bind_expl;
typedef ::dukpp03::rebind_method<sad::dukpp03::BasicContext> rebind_method;
typedef ::dukpp03::CompiledFunction<sad::dukpp03::BasicContext> CompiledFunction;
typedef ::dukpp03::getter<sad::dukpp03::BasicContext> getter;
typedef ::dukpp03::setter<sad::dukpp03::BasicContext> setter;
typedef ::dukpp03::make_lambda<sad::dukpp03::BasicContext> make_lambda;
typedef ::dukpp03::bind_lambda<sad::dukpp03::BasicContext> bind_lambda;

#define DUKPP03_FROM_STACK(TYPE, CONTEXT, INDEX) (::dukpp03::GetValue< DUKPP03_TYPE(TYPE), sad::dukpp03::BasicContext>::perform((CONTEXT),(INDEX)))

#define SAD_DUKPP03_TEMPLATE_REGFUNC_BODY this->registerCallable(name, sad::dukpp03::make_function::from(f));
/*! A context, that should be used, when working with library
 */
class Context: public sad::dukpp03::BasicContext, public sad::RefCountable
{
public:
    /*! Construct default context
        \param[in] vanilla if true, does not initalizes context
     */
    Context(bool vanilla = false);
    /*! Destructs a context
     */
    ~Context();
    /*! Evals file. If no error occured, result is not popped
        out from stack, since we still may need it
        \param[in] path a path to file
        \param[in] clean_heap whether heap should be cleaned after execution. If provided, result is popped from stack
        \param[out] error a string, where error should be written
        \return true if no error
     */
    bool evalFromFile(const std::string& path, bool clean_heap = true,std::string* error = NULL);
    /*! Sets renderer for context
        \param[in] r renderer
     */ 
    void setRenderer(sad::Renderer* r);
    /*! Returns renderer for contex
        \return renderer
     */
    sad::Renderer* renderer() const;
    /*! Resets the context with initialization
     */
    virtual void reset();
    /*! Adds class binding bot to self and adress of type
        \param[in] name a name for class binding
        \param[in] c a class binding
     */
    virtual bool addClassBinding(const std::string& name, ::dukpp03::ClassBinding<Self>* c);
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    inline void bind(const std::string& name, void (*f)()) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Arg1
    >
    inline void bind(const std::string& name, void (*f)(_Arg1)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Arg1,
        typename _Arg2
    >
    inline void bind(const std::string& name, void (*f)(_Arg1,_Arg2)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Arg1,
        typename _Arg2,
        typename _Arg3
    >
    inline void bind(const std::string& name, void (*f)(_Arg1,_Arg2,_Arg3)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4
    >
    inline void bind(const std::string& name, void (*f)(_Arg1,_Arg2,_Arg3,_Arg4)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5
    >
    inline void bind(const std::string& name, void (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6
    >
    inline void bind(const std::string& name, void (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7
    >
    inline void bind(const std::string& name, void (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8
    >
    inline void bind(const std::string& name, void (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8,
        typename _Arg9
    >
    inline void bind(const std::string& name, void (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8,
        typename _Arg9,
        typename _Arg10
    >
    inline void bind(const std::string& name, void (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8,
        typename _Arg9,
        typename _Arg10,
        typename _Arg11
    >
    inline void bind(const std::string& name, void (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8,
        typename _Arg9,
        typename _Arg10,
        typename _Arg11,
        typename _Arg12
    >
    inline void bind(const std::string& name, void (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8,
        typename _Arg9,
        typename _Arg10,
        typename _Arg11,
        typename _Arg12,
        typename _Arg13
    >
    inline void bind(const std::string& name, void (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8,
        typename _Arg9,
        typename _Arg10,
        typename _Arg11,
        typename _Arg12,
        typename _Arg13,
        typename _Arg14
    >
    inline void bind(const std::string& name, void (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13,_Arg14)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8,
        typename _Arg9,
        typename _Arg10,
        typename _Arg11,
        typename _Arg12,
        typename _Arg13,
        typename _Arg14,
        typename _Arg15
    >
    inline void bind(const std::string& name, void (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13,_Arg14,_Arg15)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8,
        typename _Arg9,
        typename _Arg10,
        typename _Arg11,
        typename _Arg12,
        typename _Arg13,
        typename _Arg14,
        typename _Arg15,
        typename _Arg16
    >
    inline void bind(const std::string& name, void (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13,_Arg14,_Arg15,_Arg16)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Ret
    >
    inline void bind(const std::string& name, _Ret (*f)()) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Ret,
        typename _Arg1
    >
    inline void bind(const std::string& name, _Ret (*f)(_Arg1)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Ret,
        typename _Arg1,
        typename _Arg2
    >
    inline void bind(const std::string& name, _Ret (*f)(_Arg1,_Arg2)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Ret,
        typename _Arg1,
        typename _Arg2,
        typename _Arg3
    >
    inline void bind(const std::string& name, _Ret (*f)(_Arg1,_Arg2,_Arg3)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Ret,
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4
    >
    inline void bind(const std::string& name, _Ret (*f)(_Arg1,_Arg2,_Arg3,_Arg4)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Ret,
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5
    >
    inline void bind(const std::string& name, _Ret (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Ret,
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6
    >
    inline void bind(const std::string& name, _Ret (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Ret,
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7
    >
    inline void bind(const std::string& name, _Ret (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Ret,
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8
    >
    inline void bind(const std::string& name, _Ret (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Ret,
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8,
        typename _Arg9
    >
    inline void bind(const std::string& name, _Ret (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Ret,
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8,
        typename _Arg9,
        typename _Arg10
    >
    inline void bind(const std::string& name, _Ret (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Ret,
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8,
        typename _Arg9,
        typename _Arg10,
        typename _Arg11
    >
    inline void bind(const std::string& name, _Ret (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Ret,
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8,
        typename _Arg9,
        typename _Arg10,
        typename _Arg11,
        typename _Arg12
    >
    inline void bind(const std::string& name, _Ret (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Ret,
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8,
        typename _Arg9,
        typename _Arg10,
        typename _Arg11,
        typename _Arg12,
        typename _Arg13
    >
    inline void bind(const std::string& name, _Ret (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Ret,
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8,
        typename _Arg9,
        typename _Arg10,
        typename _Arg11,
        typename _Arg12,
        typename _Arg13,
        typename _Arg14
    >
    inline void bind(const std::string& name, _Ret (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13,_Arg14)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Ret,
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8,
        typename _Arg9,
        typename _Arg10,
        typename _Arg11,
        typename _Arg12,
        typename _Arg13,
        typename _Arg14,
        typename _Arg15
    >
    inline void bind(const std::string& name, _Ret (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13,_Arg14,_Arg15)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
    /*! Registers function in context
        \param[in] name a name for function
        \param[in] f function
     */
    template<
        typename _Ret,
        typename _Arg1,
        typename _Arg2,
        typename _Arg3,
        typename _Arg4,
        typename _Arg5,
        typename _Arg6,
        typename _Arg7,
        typename _Arg8,
        typename _Arg9,
        typename _Arg10,
        typename _Arg11,
        typename _Arg12,
        typename _Arg13,
        typename _Arg14,
        typename _Arg15,
        typename _Arg16
    >
    inline void bind(const std::string& name, _Ret (*f)(_Arg1,_Arg2,_Arg3,_Arg4,_Arg5,_Arg6,_Arg7,_Arg8,_Arg9,_Arg10,_Arg11,_Arg12,_Arg13,_Arg14,_Arg15,_Arg16)) 
    {SAD_DUKPP03_TEMPLATE_REGFUNC_BODY}
protected:
    /*! A parent renderer for context
     */
    sad::Renderer* m_renderer;
    /*! Makes a vanilla cotnext, without any bindings
     */
    bool m_vanilla;
    /*! Initializes context with bindings
     */
    void initialize();
    /*! Exposes Point2D bindings
     */
    void exposePoint2D();
    /*! Exposes Point3D bindings
     */
    void exposePoint3D();
    /*! Exposes Point2I bindings
     */
    void exposePoint2I();
    /*! Exposes Point3D bindings
     */
    void exposePoint3I();
    /*! Expose sad::p2d::Vector
     */
    void exposeP2DVector();
    /*! Expose sad::Size2D
     */
    void exposeSize2D();
    /*! Expose sad::Size2I
     */
    void exposeSize2I();
    /*! Expose sad::Rect2D
     */
    void exposeRect2D();
    /*! Expose sad::Rect2I
     */
    void exposeRect2I();
    /*! Expose sad::Color
     */
    void exposeColor();
    /*! Expose sad::AColor
     */
    void exposeAColor();
    /*! Expose sad::util and fs
     */
    void exposeUtilFS();
    /*! Exposes slurp and spit functions
     */
    void exposeSlurpSpit();
    /*! Exposes context and it's functions
     */
    void exposeContext();
    /*! Exposes renderer and it's functions
     */
    void exposeRenderer();
    /*! Exposes camera and its's functions
     */
    void exposeCamera();
    /*! Exposes orthographic camera and its's functions
     */
    void exposeOrthographicCamera();
    /*! Exposes scene class and it's functions
     */
    void exposeScene();
    /*! Exposes scene node class and it's functions
     */
    void exposeSceneNode();
    /*! Exposes Sprite 3D class and it's functions
     */
    void exposeSprite3D();
    /*! Exposes rectangle, which consists from 3D points
     */
    void exposeSadRectPoint3D();
    /*! Exposes label class
     */
    void exposeLabel();
    /*! Exposes Sprite2D class and it's functions
     */
    void exposeSprite2D();
    /*! Exposes Sprite2D::Options class and it's functions
     */
    void exposeSprite2DOptions();
    /*! Exposes sad::db::CustomObject class and it's functions
     */
    void exposeCustomObject();
    /*! Exposes clipboard class and it's functions
     */
    void exposeClipboard();
};

#undef SAD_DUKPP03_TEMPLATE_REGFUNC_BODY

/*! Exposes API, related to context
    \param[in] ctx context
 */
void exposeAPI(sad::dukpp03::Context* ctx);

/*! Exposes hierarchical finite state machine functions, related to context
    \param[in] ctx context
 */
void exposeHFSM(sad::dukpp03::Context* ctx);

/*! Exposes physics functions, related to context
    \param[in] ctx context
 */
void exposeP2D(sad::dukpp03::Context* ctx);

/*! Exposes dialogues functions, related to context
    \param[in] ctx context
 */
void exposeDialogue(sad::dukpp03::Context* ctx);

/*! Exposes layouts functions, related to context
    \param[in] ctx context
 */
void exposeLayouts(sad::dukpp03::Context* ctx);
/*! Exposes animations classes and functions, related to context
    \param[in] ctx context
 */
void exposeAnimations(sad::dukpp03::Context* ctx);

}

}
