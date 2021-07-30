/*! \file singlesound.h

    Describes a sound type, which can be used to maintain main theme, e.g. one sound
    from source is played all the time, new sound only replaces old 
 */
#pragma once
#include "sound.h"
#include "../refcountable.h"

namespace sad
{
    
namespace irrklang
{

/*! Describes a sound type, which can be used to maintain main theme, e.g. one sound
    from source is played all the time, new sound only replaces old 
 */
class SingleSound: public sad::RefCountable
{
public:
    /*! Constructs new single sound instance
     */
    SingleSound();
    /*! Plays a sound, replacing old sound (if any)
        \param[in] sound a sound to be played
        \param[in] volume a volume
        \param[in] looped whether sound is looped
        \return sound object (or nullptr, if sound is null or failed to play)
     */
    ::irrklang::ISound* play2D(sad::irrklang::Sound* sound, double volume, bool looped = true);
    /*! Sets a volume for current playing sound
        \param[in] volume a volume parameter
     */
    void setVolume(double volume);
    /*! Stops music from playing
     */
    void stop();
protected:
    /*! Current playing music (source)
     */
    sad::irrklang::Sound* m_current_music_source;
    /*! Current playing music (sound)
     */
    ::irrklang::ISound* m_current_music;
};
    
}

}