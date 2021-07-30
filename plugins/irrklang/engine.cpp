#include "irrklang/engine.h"

#include <renderer.h>

#include <db/dbtypename.h>

#include <util/fs.h>

#include <cassert>
#include <stdlib.h>


// ============================= PUBLIC METHODS =============================


static sad::Mutex sad_irrklang_engine_instance;

sad::irrklang::Engine* sad::irrklang::Engine::ref()
{
    if (m_instance == nullptr)
    {
        sad_irrklang_engine_instance.lock();
        if (m_instance == nullptr)
        {
            m_instance = new sad::irrklang::Engine();
            atexit(sad::irrklang::Engine::freeInstance);
        }
        sad_irrklang_engine_instance.unlock();
    }
    return m_instance;
}

::irrklang::ISoundSource* sad::irrklang::Engine::tryLoad(const sad::String& source)
{
    m_add_lock.lock();
    ::irrklang::ISoundSource* result = m_engine->addSoundSourceFromFile(source.c_str());
    if (result)
    {
        ::irrklang::ISound* test = m_engine->play2D(result, false, true, true);
        if (!test)
        {
            m_engine->removeSoundSource(result);
            result = nullptr;
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
                result = nullptr;
            }
            else
            {
                test->stop();
                test->drop();
            }
        }
    }
    m_add_lock.unlock();
    return result;
}

::irrklang::ISoundSource* sad::irrklang::Engine::addAsAlias(
    ::irrklang::ISoundSource* source,
    const sad::String& name
)
{
   m_add_lock.lock();
   ::irrklang::ISoundSource* result = m_engine->addSoundSourceAlias(source, name.c_str()); 
   m_add_lock.unlock();   
   return result;
}

sad::irrklang::Engine::~Engine()
{
    if (m_engine) 
    {
        m_engine->drop();
    }
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

sad::irrklang::Engine* sad::irrklang::Engine::m_instance = nullptr;

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

void sad::irrklang::Engine::freeInstance()
{
    if (m_instance) 
    {
        delete m_instance;
        m_instance = nullptr;
    }
}


DECLARE_COMMON_TYPE(sad::irrklang::Engine)
