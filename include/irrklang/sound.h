/*! \file sound.h

    Describes a sound as resource
 */
#pragma once
#include "engine.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <resource/resource.h>
#include <renderer.h>

#include <3rdparty/picojson/valuetotype.h>

#include <util/pointercallback.h>

namespace sad
{
    
namespace irrklang
{

/*! Defines a wrapper for sound from irrKlang to make it loadable
    from resource files
 */
class Sound: public sad::resource::Resource, public ::irrklang::ISoundStopEventReceiver 
{
SAD_OBJECT
public:
    /*! A callback, which is called, when sound is stopped
     */
    typedef sad::util::PointerCallback<sad::irrklang::Sound> StopCallback;
    /*! Constructs default, invalid source
     */
    Sound();
    /*! Returns source for data
        \return wrapped source
     */
    ::irrklang::ISoundSource* s() const;
    /*! Loads a texture from specified file, using specified renderer for building mip maps.
        \param[in] file a file, via which a resource should be loaded
        \param[in] r  a renderer, which resource should be linked to (nullptr if global renderer)
        \param[in] options  an options for loading a resource
        \return whether loading was successfull
     */
    virtual bool load(
        const sad::resource::ResourceFile & file,
        sad::Renderer * r,
        const picojson::value& options
    ) override;
    /*! Sets default volume for a sound
        \param[in] volume a default volume
     */
    void setDefaultVolume(double volume);
    /*! Plays sound with specified volume
        \param[in] volume a volume
        \param[in] looped whether sound is looped
        \return sound object
     */
    ::irrklang::ISound* play2D(double volume, bool looped = true);
    /*! Returns true, whether sound is playing
        \return whether sound is playing
     */
    bool isPlaying() const;
    /*! Adds callback, which will be called, when sound is stopped
        \param[in] cb callback
     */
    void addCallback(StopCallback* cb);
    /*! Adds callback, which will be called, when sound is stopped
        \param[in] cb callback
     */
    inline void addCallback(void (*cb)())
    {
        addCallback(new sad::util::FreeZeroArgCallback<sad::irrklang::Sound>(cb));
    }
    /*! Adds callback, which will be called, when sound is stopped
        \param[in] cb callback
     */
    inline void addCallback(void (*cb)(sad::irrklang::Sound*))
    {
        addCallback(new sad::util::FreeOneArgCallback<sad::irrklang::Sound>(cb));
    }
    /*! Adds callback, which will be called, when sound is stopped
        \param[in] o object
        \param[in] cb callback
     */
    template<
        typename _Object,
        typename _CalledObject
    >
    inline void addCallback(_Object* o, void (_CalledObject::*cb)())
    {
        addCallback(new sad::util::MethodZeroArgCallback<sad::irrklang::Sound, _Object, _CalledObject>(o, cb));
    }
    /*! Adds callback, which will be called, when sound is stopped
        \param[in] o object
        \param[in] cb callback
     */
    template<
        typename _Object,
        typename _CalledObject
    >
    inline void addCallback(_Object* o, void (_CalledObject::*cb)(sad::irrklang::Sound*))
    {
        addCallback(new sad::util::MethodOneArgCallback<sad::irrklang::Sound, _Object, _CalledObject>(o, cb));
    }
    /*! Adds callback, which will be called, when sound is stopped
        \param[in] o object
        \param[in] cb callback
     */
    template<
        typename _Object,
        typename _CalledObject
    >
    inline void addCallback(_Object* o, void (_CalledObject::*cb)() const)
    {
        addCallback(new sad::util::ConstMethodZeroArgCallback<sad::irrklang::Sound, _Object, _CalledObject>(o, cb));
    }
    /*! Adds callback, which will be called, when sound is stopped
        \param[in] o object
        \param[in] cb callback
     */
    template<
        typename _Object,
        typename _CalledObject
    >
    inline void addCallback(_Object* o, void (_CalledObject::*cb)(sad::irrklang::Sound*) const)
    {
        addCallback(new sad::util::ConstMethodOneArgCallback<sad::irrklang::Sound, _Object, _CalledObject>(o, cb));
    }
protected:
    /*! Called, when sound is stopped
     */
    void OnSoundStopped(
        ::irrklang::ISound *  sound,
        ::irrklang::E_STOP_EVENT_CAUSE  reason,
        void * 	userData
    ) override;
    /*! A sound source, wrapped in resource
     */
    ::irrklang::ISoundSource* m_source;
    /*! A callbacks on stopping sound
     */
    sad::PtrVector<StopCallback> m_stop_callbacks;
};

}

}
