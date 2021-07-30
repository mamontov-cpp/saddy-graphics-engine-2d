/*! \file jscollisionhandler.h

    A handler for collisions
*/
#pragma once
#include "../p2d/collisionhandler.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "../renderer.h"
#include "context.h"

namespace sad
{

namespace dukpp03
{

        
/*! A js collision handler
 */
class JSCollisionHandler : public sad::p2d::BasicCollisionHandler
{
public:
    /*! Makes new handler
        \param[in] ctx context
        \param[in] f function
    */
    JSCollisionHandler(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f);
    /*! Calls a handler with event
        \param[in] ev event
     */
    virtual  void invoke(const sad::p2d::BasicCollisionEvent & ev) override;
    /*! Frees a handler
     */
    virtual ~JSCollisionHandler() override;
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
