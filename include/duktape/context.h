/*! \file context.h
	\author HiddenSeeker

	A simple wrapper around basic Duktape context to support embeddable operations.
 */
#pragma once
#include "variantpool.h"
#include "pushvalue.h"
#include "getvalue.h"
#include "timer.h"
#include "errorcodes.h"

namespace sad
{

class Renderer;

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
	/*! Evals string, with code in it. If no error occured, result is not popped
		out from stack, since we still may need it
		\param[in] string a string
		\param[in] clean_heap whether heap should be cleaned after execution. If provided, result is popped from stack
		\param[out] error a string, where error should be written
		\return true if no error
	 */
	bool eval(const sad::String& string, bool clean_heap = true,sad::String* error = NULL);
	/*! Evals string, reading it from file
		\param[in] fileName name of file
		\param[in] clean_heap whether heap should be cleaned after execution. If provided, result is popped from stack
		\param[out] error a string, where error should be written
		\param[in] renderer renderer, which could be used for computing local path
		\return true if no error
 	 */
	bool evalFromFile(
		const sad::String& file_name,
		bool clean_heap = true,
		sad::String* error = NULL,
		sad::Renderer* renderer = NULL
	);
	/*! Cleans non-persistent pool of objects, resetting it
	 */
	void clean();
	/*! Resets context fully, erasing all data
	 */
	void reset();
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
	/*! Checks, whether timeout is reached for context. Called, while
		evaluating value
	 */
	bool timeoutReached() const;
	/*! Sets maximal execution time for a script
		\param[in] time maximal execution time
	 */
	void setMaximumExecutionTime(double time);
	/*! Return maximal execution time for a script
		\return maximal execution time
	 */
	double maximumExecutionTime() const;
	/*! Throws error from a context
		\param[in] error_string string data for error
		\param[in] code error codes
	 */
	void throwError(const sad::String& error_string, sad::duktape::ErrorCodes code = sad::duktape::SAD_DUK_E5_ERROR);
	/*! Registers variable as property of global object, pushing it into a persistent stack. Replaces existing property.
		\param[in] property_name name of new property of global object
		\param[in] value a value to be registered
	 */
	void registerGlobalVariable(const sad::String& property_name, sad::db::Variant* value);
	/*! Registers variable as global object, pushing it into a persistent stack. Replaces existing property.
		\param[in] property_name name of new property of global object
		\param[in] value a value to be registered
	 */
	template<
		typename T
	>
	void registerGlobal(const sad::String& property_name, const T& value)
	{
		registerGlobalVariable(property_name, new sad::db::Variant(value));
	}
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
	/*! A timeout timer for context
	 */
	sad::Timer m_timeout_timer;
	/*! A maximal execution time
	 */
	double m_maximal_execution_time;
	/*! Whether execution is running
	 */ 
	bool m_running;
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
