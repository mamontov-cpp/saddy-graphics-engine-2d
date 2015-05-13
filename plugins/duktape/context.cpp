#include "duktape/context.h"

#include "renderer.h"

#include "util/fs.h"

#include <fstream>

/*! A signature, which points, that current string is linked to variant pool
 */
#define SAD_DUKTAPE_VARIANT_SIGNATURE "\1sad::db::Variant\1"
/*! A signature, which points, that current string is linked to variant persistent
 */
#define SAD_DUKTAPE_PERSISTENT_VARIANT_SIGNATURE "\1sad::db::Variant__persistent\1"

// ================================= PUBLIC METHODS =================================

sad::duktape::Context::Context() : m_maximal_execution_time(30000), m_running(false)
{
	m_context = duk_create_heap(NULL,NULL, NULL, this, NULL);
}

sad::duktape::Context::~Context()
{
	duk_destroy_heap(m_context);
}


sad::duktape::Context* sad::duktape::Context::getContext(duk_context* ctx)
{
	duk_memory_functions funcs;
	duk_get_memory_functions(ctx, &funcs);
	assert( funcs.udata );
	return static_cast<sad::duktape::Context*>(funcs.udata);
}

bool sad::duktape::Context::eval(const sad::String& string, bool clean_heap, sad::String* error)
{
	m_running = true;
	m_timeout_timer.start();
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
	m_running = false;
	m_timeout_timer.stop();
	return result;
}


bool sad::duktape::Context::evalFromFile(
		const sad::String& file_name,
		bool clean_heap,
		sad::String* error,
		sad::Renderer* renderer
)
{
	sad::Maybe<sad::String> maybetext;
	std::ifstream stream(file_name.c_str());
	if (stream.good())
	{
		std::string alldata(
			(std::istreambuf_iterator<char>(stream)), 
			std::istreambuf_iterator<char>()
		);
		maybetext.setValue(alldata);
	}
	else
	{
		if (util::isAbsolutePath(file_name) == false)
		{
			if (!renderer)
			{
				renderer = sad::Renderer::ref();
			}
			sad::String path = util::concatPaths(renderer->executablePath(), file_name);
			stream.clear();
			stream.open(path.c_str());
			if (stream.good())
			{
				std::string alldata(
					(std::istreambuf_iterator<char>(stream)), 
					 std::istreambuf_iterator<char>()
					);
				maybetext.setValue(alldata);
			}
		}
	}
	if (maybetext.exists())
	{
		return eval(maybetext.value(), clean_heap, error);
	}
	if (error)
	{
		*error = sad::String("Cannot open file \"");
		*error += file_name;
		*error = "\"";
	}
	return false;
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
	m_context = duk_create_heap(NULL,NULL, NULL, this, NULL);
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

bool sad::duktape::Context::timeoutReached() const
{
	if (!m_running)
	{
		return false;
	}
	const_cast<sad::duktape::Context*>(this)->m_timeout_timer.stop();
	double elapsed_time = m_timeout_timer.elapsed();
	return (elapsed_time >= m_maximal_execution_time);
}

void sad::duktape::Context::setMaximumExecutionTime(double time)
{
	m_maximal_execution_time = time;
}

double sad::duktape::Context::maximumExecutionTime() const
{
	return m_maximal_execution_time;
}

void sad::duktape::Context::throwError(const sad::String& error_string, sad::duktape::ErrorCodes code)
{
	duk_push_error_object(m_context, static_cast<int>(code), error_string.c_str());
	if (m_running) 
	{
		duk_throw(m_context);
	}
}

// ================================= PROTECTED METHODS =================================

void sad::duktape::Context::initContextBeforeAccessing()
{
	
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

// ===================== sad::duktape::____check_timeout  ===========================

int sad::duktape::____check_timeout(void* ptr)
{
	if (!ptr)
	{
		return 0;
	}
	if (static_cast<sad::duktape::Context*>(ptr)->timeoutReached())
		return 1;
	return 0;
}