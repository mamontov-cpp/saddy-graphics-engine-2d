#include "irrklang/sound.h"

DECLARE_SOBJ_INHERITANCE(sad::irrklang::Sound, sad::resource::Resource);

sad::irrklang::Sound::Sound() : m_source(NULL)
{
    
}

::irrklang::ISoundSource* sad::irrklang::Sound::s() const
{
    return m_source;
}

bool sad::irrklang::Sound::load(
	const sad::resource::PhysicalFile & file,
	sad::Renderer * r,
	const picojson::value& options
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
 