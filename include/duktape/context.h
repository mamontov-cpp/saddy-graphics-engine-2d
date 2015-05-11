/*! \file context.h
	\author HiddenSeeker

	A simple wrapper around basic Duktape context to support embeddable operations.
 */
#pragma once
#include "variantpool.h"
#include "../3rdparty/duktape/duktape.h"

namespace sad
{
	
namespace duktape
{

/*! A wrapper around basic duktape context to support embedded operations with non-basic
	operations
 */
class Context
{
public:
	/*! Creates new context
	 */
	Context();
	/*! Context is inheritable
	 */
	virtual ~Context();
	/*! Tries to get current context from raw context 
		\param[in] ctx context
		\return context
	 */
	static sad::duktape::Context* getContext(duk_context* ctx);
	/*! Returns new context
		\return context data
	 */
	virtual duk_context* context();
protected:
	/*! Inits context for evaluating
	 */
	virtual void initContextBeforeAccessing();
	/*! Inner context
	 */
	duk_context* m_context;
	/*! A basic pool for objects in context
	 */
	sad::duktape::VariantPool m_pool;
private:
	/*! This object is non-copyable
		\param[in] p context
	 */
	Context(const sad::duktape::Context& p);
	/*! This object is non-copyable
		\param[in] p context
		\return self-reference
	 */
	sad::duktape::Context& operator=(const sad::duktape::Context& p);
};

}

}
