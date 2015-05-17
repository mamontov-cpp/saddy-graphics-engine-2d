#include "irrklang/engine.h"

#include <renderer.h>

#include <util/fs.h>

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

::irrklang::ISoundSource* sad::irrklang::Engine::tryLoad(const sad::String& source)
{
    ::irrklang::ISoundSource* result = m_engine->addSoundSourceFromFile(source.c_str());
    if (result)
    {
        ::irrklang::ISound* test = m_engine->play2D(result, false, true, true);
        if (!test)
        {
            m_engine->removeSoundSource(result);
            result = NULL;
        }
        else
        {
            test->stop();
            test->drop();
        }
    }
    if (!result && !util::isAbsolutePath(source))
	{
		sad::String newpath = util::concatPaths(sad::Renderer::ref()->executablePath(), source);
		result = m_engine->addSoundSourceFromFile(newpath.c_str());
        if (result)
        {
            ::irrklang::ISound* test = m_engine->play2D(result, false, true, true);
            if (!test)
            {
                m_engine->removeSoundSource(result);
                result = NULL;
            }
            else
            {
                test->stop();
                test->drop();
            }
        }
	}
    return result;
}

::irrklang::ISoundSource* sad::irrklang::Engine::addAsAlias(
    ::irrklang::ISoundSource* source,
    const sad::String& name
)
{
   return m_engine->addSoundSourceAlias(source, name.c_str()); 
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

sad::irrklang::Engine* sad::irrklang::Engine::m_instance = NULL;

// ============================= PRIVATE METHODS =============================

sad::irrklang::Engine::Engine()
{
	m_engine = ::irrklang::createIrrKlangDevice();
	assert( m_engine );
}

sad::irrklang::Engine::Engine(const Engine& o)
: m_engine(o.m_engine) 
{
	m_engine->grab();
}

sad::irrklang::Engine& sad::irrklang::Engine::operator=(const sad::irrklang::Engine& o)
{
	m_engine = o.m_engine;
	m_engine->grab();
	return *this;
}
