#include "irrklang/singlesound.h"

#include <db/dbtypename.h>

sad::irrklang::SingleSound::SingleSound()
: m_current_music_source(nullptr), m_current_music(nullptr)
{
    
}

::irrklang::ISound* sad::irrklang::SingleSound::play2D(
    sad::irrklang::Sound* sound, 
    double volume, 
    bool looped
)
{
    if (!sound)
    {
        return nullptr;
    }

    // Stop old music
    bool start_new_music = true;
    if (m_current_music_source && m_current_music)
    {                    
        if (m_current_music_source != sound) {
            if (m_current_music_source->isPlaying())
            {
                m_current_music->stop();    
            }
        }
        else
        {
            if (m_current_music_source->isPlaying())
            {
                start_new_music = false;
            }
        }
    }
    // Start new music
    if (start_new_music)
    {
        m_current_music_source = sound;
        m_current_music = sound->play2D(volume, looped);      
    }
    return m_current_music;
}

void sad::irrklang::SingleSound::setVolume(double volume)
{
    if (m_current_music)
    {
        m_current_music->setVolume(static_cast<float>(volume));        
    }
}

void sad::irrklang::SingleSound::stop()
{
    if (m_current_music)
    {
        m_current_music->stop();
        m_current_music = nullptr;
        m_current_music_source = nullptr;
    }
}

DECLARE_COMMON_TYPE(sad::irrklang::SingleSound)
