/*! \file context.h
  
    Defines a context, that can be used in Saddy
 */
#pragma once
#include "basiccontext.h"
#include "getvalue.h"
#include "pushvalue.h"

namespace sad
{

class Renderer;

namespace dukpp03
{

typedef ::dukpp03::make_fun<sad::dukpp03::BasicContext> make_function;
typedef ::dukpp03::register_constructor<sad::dukpp03::BasicContext> register_constructor;
typedef ::dukpp03::make_method<sad::dukpp03::BasicContext> make_method;
typedef ::dukpp03::bind_method<sad::dukpp03::BasicContext> bind_method;
typedef ::dukpp03::CompiledFunction<sad::dukpp03::BasicContext> CompiledFunction;
typedef ::dukpp03::getter<sad::dukpp03::BasicContext> getter;
typedef ::dukpp03::setter<sad::dukpp03::BasicContext> setter;
typedef ::dukpp03::ClassBinding<sad::dukpp03::BasicContext> ClassBinding;

/*! A context, that should be used, when working with library
 */
class Context: public sad::dukpp03::BasicContext
{
public:
    /*! Construct default context
     */
    Context();
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
protected:
    /*! A parent renderer for context
     */
    sad::Renderer* m_renderer;
};


}

}
