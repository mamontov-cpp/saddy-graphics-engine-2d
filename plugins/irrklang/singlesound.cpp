#include "irrklang/singlesound.h"

sad::irrklang::SingleSound::SingleSound()
: m_current_music_source(NULL), m_current_music(NULL)
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
        return NULL;
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
