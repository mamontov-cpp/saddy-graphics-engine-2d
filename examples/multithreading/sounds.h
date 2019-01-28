/*! \file sounds.h
 *
 *  Declares sounds containers for game
 */
#pragma once
#include <sadhash.h>

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
    /*! Destructs an object
     */
    ~Sounds();
    /**
     * Plays sound, storing it in container
     * \param[in] sound_name sound name
     * \param[in] volume a volume
     */
    void playSound(const sad::String& sound_name, double volume) const;
    /**
     * Plays walking sound
     * \param[in] volume a new volume
     */
    void playWalkingSound(double volume) const;
    /*! Stops walking sound
     */
    void stopWalkingSound() const;
    /*! Pauses all sounds
     */
    static void pause();
    /*! Resumes all sounds
     */
    static void resume();
private:
    /*! A footstep sound
     */
    ::irrklang::ISound* m_footstep_sound;
    /*! A footsteps sound is playing
     */
    sad::irrklang::SingleSound* m_footsteps;
    /*! A renderer, where all sounds are stored
     */
    sad::Renderer* m_renderer;
    /*! Sets if sound is paused
     */
    bool m_paused;
};
