/*! \file sounds.h
 *
 *  Declares sounds containers for game
 */
#pragma once
#include <sadhash.h>
#include <memory>

namespace irrklang
{
class ISound;
}

namespace sad
{

namespace irrklang
{
class SingleSound;
}

class Renderer;
}

/*! A class for storing sound containers
 */
class Sounds
{
public:
    /*! Constructs sounds objects, using renderer
     *  \param[in] r renderer
     */
    Sounds(sad::Renderer* r);
    /**
     * Plays sound, storing it in container
     * \param[in] sound_name sound name
     * \param[in] volume a volume
     */
    void playSound(const sad::String& sound_name, double volume);
    /**
     * Plays walking sound
     * \param[in] volume a new volume
     */
    void playWalkingSound(double volume);

    /*! Stops walking sound
     */
    void stopWalkingSound();
    /*! Pauses all sounds
     */
    void pause();
    /*! Resumes all sounds
     */
    void resume();
private:
    /*! A list of sounds to lookup
     */
    sad::Hash<::irrklang::ISound*, ::irrklang::ISound*> m_sounds;
    /*! A footstep sound
     */
    ::irrklang::ISound* m_footstep_sound;
    /*! A footsteps sound is playing
     */
    std::unique_ptr<sad::irrklang::SingleSound> m_footsteps;
    /*! A renderer, where all sounds are stored
     */
    sad::Renderer* m_renderer;
    /*! Sets if sound is paused
     */
    bool m_paused;
};
