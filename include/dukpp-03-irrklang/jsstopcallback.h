/*! \file jsstopcallback.h
    
    A stop callback to be called with JS
 */
#pragma once
#include "../dukpp-03/context.h"

#include "../irrklang/sound.h"

namespace sad
{

namespace dukpp03irrklang
{

class JSStopCallback: public  sad::util::PointerCallback<sad::irrklang::Sound>
{
public:
    /*! Makes new callback
        \param[in] ctx context
        \param[in] f function
     */
    JSStopCallback(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f);
    /*! Calls a callback
        \param[in] o a new object
     */
    virtual void call(sad::irrklang::Sound* o) override;
    /*! Copies a pointer callback
        \return copy
     */
    virtual sad::util::PointerCallback<sad::irrklang::Sound>* copy() override;
    /*! Removes reference from context
     */
    virtual ~JSStopCallback() override;
protected:
    /*! A context to be called with handler
     */
    sad::dukpp03::Context* m_ctx;
    /*! A function to be called
     */
    sad::dukpp03::CompiledFunction m_function;
};

}

}
