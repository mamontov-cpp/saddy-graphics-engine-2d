#include "duktape/context.h"

/*! A property, where context is located
 */
#define SAD_DUKTAPE_CONTEXT_PROPERTY_NAME "_____context"
/*! A signature, which points, that current string is linked to variant pool
 */
#define SAD_DUKTAPE_VARIANT_SIGNATURE "\1sad::db::Variant\1"
/*! A signature, which points, that current string is linked to variant persistent
 */
#define SAD_DUKTAPE_PERSISTENT_VARIANT_SIGNATURE "\1sad::db::Variant__persistent\1"

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

bool sad::duktape::Context::eval(const sad::String& string, bool clean_heap, sad::String* error)
{
	duk_push_string(m_context, string.c_str());
	bool result = false;
	if (duk_peval(m_context) != 0) 
	{
		if (error)
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
	if (clean_heap)
	{
		clean();
	}
	return result;
}

void sad::duktape::Context::clean()
{
	m_pool.free();
}

void sad::duktape::Context::reset()
{
	m_pool.free();
	m_persistent_pool.free();

	duk_destroy_heap(m_context);
	m_context = duk_create_heap_default();
	initContextBeforeAccessing();
}


duk_context* sad::duktape::Context::context()
{
	this->initContextBeforeAccessing();
	return m_context;
}

sad::String sad::duktape::Context::pushVariant(sad::db::Variant* v)
{
	sad::String result = sad::String(SAD_DUKTAPE_VARIANT_SIGNATURE) + m_pool.insert(v);
	duk_push_string(m_context, result.c_str());
	return result;
}

sad::String sad::duktape::Context::pushPersistentVariant(sad::db::Variant* v)
{
	sad::String result = sad::String(SAD_DUKTAPE_PERSISTENT_VARIANT_SIGNATURE) 
					   + m_persistent_pool.insert(v);
	duk_push_string(m_context, result.c_str());
	return result;
}

sad::db::Variant* sad::duktape::Context::getValueFromPool(const sad::String& key)
{
	sad::String signature = SAD_DUKTAPE_VARIANT_SIGNATURE;
	if (key.subString(0, signature.length()) != signature)
	{
		sad::String persistent_signature = SAD_DUKTAPE_PERSISTENT_VARIANT_SIGNATURE;
		if (key.subString(0, persistent_signature.length()) != persistent_signature)
		{
			return NULL;
		}
		return m_persistent_pool.get(
			key.subString(
				persistent_signature.length(), 
				key.length() - persistent_signature.length()
			)
		);
	}
	return m_pool.get(key.subString(signature.length(), key.length() - signature.length()));
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