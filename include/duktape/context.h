/*! \file context.h
	\author HiddenSeeker

	A simple wrapper around basic Duktape context to support embeddable operations.
 */
#pragma once
#include "variantpool.h"
#include "pushvalue.h"
#include "getvalue.h"
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
	/*! Pushes variant to a persistent pool
		\param[in] v variant
		\return string signature
	 */
	sad::String pushPersistentVariant(sad::db::Variant* v);
	/*! Pushes variant to a pool
		\param[in] v variant
		\return string signature
	 */
	sad::String pushVariant(sad::db::Variant* v);
	/*! Gets value from pool by key
		\param[in] a key from value
		\return value
	 */
	sad::db::Variant* getValueFromPool(const sad::String& key);
	/*! Returns value from pool by string, linked on stack
		\param[in] pos position on stack
		\return value
	 */
	template<
		typename T
	>
	sad::Maybe<T> getValueFromPoolByStringFromStack(duk_idx_t pos)
	{
		sad::Maybe<T> result;
		if (duk_is_string(m_context, pos))
		{
			sad::db::Variant* v = getValueFromPool(duk_to_string(m_context, pos));
			if (v)
			{
				result = v->get<T>();
			}
		}
		return result;
	}
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
	/*! A persistent pool for values
	 */
	sad::duktape::VariantPool m_persistent_pool;
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

#include "pushvalue_src.h"
#include "getvalue_src.h"
