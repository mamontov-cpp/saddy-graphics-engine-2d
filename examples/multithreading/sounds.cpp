#include "sounds.h"

#include <renderer.h>
#include <irrklang/singlesound.h>
#include <irrklang/sound.h>

Sounds::Sounds(sad::Renderer* r) : m_footstep_sound(nullptr), m_footsteps(nullptr), m_renderer(r), m_paused(false)
{
    m_footsteps = new sad::irrklang::SingleSound();
}

Sounds::~Sounds()
{
    delete m_footsteps;
}

void Sounds::playSound(const sad::String& sound_name, double volume) const
{
    sad::irrklang::Sound* theme_data = m_renderer->tree("")->get<sad::irrklang::Sound>(sound_name);
    if (theme_data)
    { 
        ::irrklang::ISound* sound = theme_data->play2D(volume, false);
        if (m_paused)
        {
            sound->setIsPaused(true);
        }
    }
}

void Sounds::playWalkingSound(double volume) const
{
    sad::irrklang::Sound* theme_data = m_renderer->tree("")->get<sad::irrklang::Sound>("footstep");
    if (theme_data)
    { 
        ::irrklang::ISound* sound = m_footsteps->play2D(theme_data, volume);
        if (m_paused)
        {
            sound->setIsPaused(true);
        }
    }
}


void Sounds::stopWalkingSound() const
{
    m_footsteps->stop();
}

void Sounds::pause()
{
    sad::irrklang::Engine::eref()->setAllSoundsPaused(true);
}

void Sounds::resume()
{
    sad::irrklang::Engine::eref()->setAllSoundsPaused(false);
}



