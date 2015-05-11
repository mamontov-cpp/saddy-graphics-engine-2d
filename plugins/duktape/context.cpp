#include "duktape/context.h"

/*! A property, where context is located
 */
#define SAD_DUKTAPE_CONTEXT_PROPERTY_NAME "_____context"

// ================================= PUBLIC METHODS =================================

sad::duktape::Context::Context()
{
	m_context = duk_create_heap_default();
}

sad::duktape::Context::~Context()
{
	duk_destroy_heap(m_context);
}


sad::duktape::Context* sad::duktape::Context::getContext(duk_context* ctx)
{
	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx, -1, SAD_DUKTAPE_CONTEXT_PROPERTY_NAME);
	sad::duktape::Context* result = static_cast<sad::duktape::Context*>(duk_require_pointer(ctx, -1));
	duk_pop(ctx);
	return result;
}

duk_context* sad::duktape::Context::context()
{
	this->initContextBeforeAccessing();
	return m_context;
}

// ================================= PROTECTED METHODS =================================

void sad::duktape::Context::initContextBeforeAccessing()
{
	duk_push_global_stash(m_context);
	duk_push_pointer(m_context, this);
	duk_put_prop_string(m_context, -2, SAD_DUKTAPE_CONTEXT_PROPERTY_NAME);
	duk_pop(m_context);
}

// ================================= PRIVATE METHODS =================================

sad::duktape::Context::Context(const sad::duktape::Context& p)
{
	throw std::logic_error("sad::duktape::Context is non-copyable!");
}

sad::duktape::Context& sad::duktape::Context::operator=(const sad::duktape::Context& p)
{
	throw std::logic_error("sad::duktape::Context is non-copyable!");
	return *this;
}