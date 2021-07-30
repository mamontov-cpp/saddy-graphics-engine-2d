/*! \file jshfsmhandler.h

    An HFSM handler, that can call a compiled function
 */
#pragma once
#include "context.h"
#include "../hfsm/hfsmhandler.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "../renderer.h"

namespace sad
{

namespace dukpp03
{


/*! A js handler to be used in states and transitions
*/
class JSHFSMHandler: public sad::hfsm::AbstractHandler
{
public:
/*! Makes new handler
    \param[in] ctx context
    \param[in] f function
*/
JSHFSMHandler(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f);
/*! Invokes a handler
 */
virtual void invoke() override;
/*! Frees a handler
 */
virtual ~JSHFSMHandler() override;
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
