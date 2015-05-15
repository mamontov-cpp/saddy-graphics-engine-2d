#include "irrklang/engine.h"

#include <cassert>

// ============================= PUBLIC METHODS =============================

sad::irrklang::Engine* sad::irrklang::Engine::ref()
{
	if (m_instance == NULL)
	{
		m_instance = new sad::irrklang::Engine();
	}
	return m_instance;
}

sad::irrklang::Engine::~Engine()
{
	m_engine->drop();
}

::irrklang::ISoundEngine* sad::irrklang::Engine::get() const
{
	return m_engine;
}

::irrklang::ISoundEngine* sad::irrklang::Engine::eref()
{
	return ref()->get();
}

// ============================= PROTECTED METHODS =============================

static sad::irrklang::Engine* m_instance = NULL;

// ============================= PRIVATE METHODS =============================

sad::irrklang::Engine::Engine()
{
	m_engine = ::irrklang::createIrrKlangDevice();
	assert( m_engine );
}

sad::irrklang::Engine::Engine(const Engine& o)
: m_registered(o.m_registered), m_engine(o.m_engine) 
{
	m_engine->grab();
}

sad::irrklang::Engine& sad::irrklang::Engine::operator=(const sad::irrklang::Engine& o)
{
	m_registered = o.m_registered;
	m_engine = o.m_engine;
	m_engine->grab();
	return *this;
}
