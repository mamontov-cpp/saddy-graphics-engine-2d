#include "irrklang/sound.h"

DECLARE_SOBJ_INHERITANCE(sad::irrklang::Sound, sad::resource::Resource);

sad::irrklang::Sound::Sound() : m_source(nullptr)
{
    
}

::irrklang::ISoundSource* sad::irrklang::Sound::s() const
{
    return m_source;
}

bool sad::irrklang::Sound::load(
    const sad::resource::ResourceFile & file,
    sad::Renderer*,
    const picojson::value&
)
{
    ::irrklang::ISoundSource* newsource = sad::irrklang::Engine::ref()->tryLoad(file.name());
    bool result = false;
    if (newsource)
    {
        if (!m_source)
        {
            sad::irrklang::Engine::eref()->removeSoundSource(m_source);
        }
        m_source = newsource;
        result = true;
    }
    return result;   
}
 

void sad::irrklang::Sound::setDefaultVolume(double volume)
{
    if (m_source)
    {
        m_source->setDefaultVolume(static_cast<float>(volume));
    }
}

::irrklang::ISound* sad::irrklang::Sound::play2D(double volume, bool looped)
{
    if (!m_source)
    {
        return nullptr;
    }
    ::irrklang::ISoundEngine* engine = sad::irrklang::Engine::eref();
    setDefaultVolume(volume);
    ::irrklang::ISound* sound = engine->play2D(m_source, looped, false, true);
    if (sound)
    {
        sound->setSoundStopEventReceiver(this);
    }
    return sound;
}


bool sad::irrklang::Sound::isPlaying() const
{
    if (!m_source)
    {
        return false;
    }
    ::irrklang::ISoundEngine* engine = sad::irrklang::Engine::eref();
    return engine->isCurrentlyPlaying(m_source);
}

void sad::irrklang::Sound::addCallback(sad::irrklang::Sound::StopCallback* cb)
{
    m_stop_callbacks << cb;
}

void sad::irrklang::Sound::OnSoundStopped(	
        ::irrklang::ISound *,
        ::irrklang::E_STOP_EVENT_CAUSE 	reason,
        void *	 
)
{
    if (reason == ::irrklang::ESEC_SOUND_FINISHED_PLAYING)
    {
        for(size_t i = 0; i < m_stop_callbacks.size(); i++)
        {
            m_stop_callbacks[i]->call(this);
        }
    }
}