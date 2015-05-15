/*! \file sound.h
    \author HiddenSeeker

    Describes a sound as resource
 */
#pragma once
#include "engine.h"

#include <resource/resource.h>
#include <resource/error.h>
#include <renderer.h>

#include <3rdparty/picojson/valuetotype.h>

namespace sad
{
    
namespace irrklang
{

/*! Defines a wrapper for sound from irrKlang to make it loadable
    from resource files
 */
class Sound: public sad::resource::Resource
{
SAD_OBJECT
public:
    /*! Constructs default, invalid source
     */
    Sound();
    /*! Returns source for data
        \return wrapped source
     */
    ::irrklang::ISoundSource* s() const;
    /*! Loads a texture from specified file, using specified renderer for building mip maps.
		\param[in] file a file, via which a resource should be loaded
		\param[in] r  a renderer, which resource should be linked to (NULL if global renderer)
		\param[in] options  an options for loading a resource
		\return whether loading was successfull
	 */
	virtual bool load(
		const sad::resource::PhysicalFile & file,
		sad::Renderer * r,
		const picojson::value& options
	);
protected:
    /*! A sound source, wrapped in resource
     */
    ::irrklang::ISoundSource* m_source;
};

}

}
