/*! \file resource/resourcefactory.h
	\author HiddenSeeker
	
	Contains definition of class ResourceFactory.

	Note, that resource factory can create resources by their type string.
 */
#pragma once
#include "../texture.h"
#include "../texturemappedfont.h"
#include "../sprite2d.h"
#include "../sadptrhash.h"
#include "../sadstring.h"

#ifdef USE_FREETYPE
#include <freetype/font.h>
#endif

namespace resource
{
class Resource; 

/*! A special kind of delegate, which is used by Factory to create resources
 */
class Creator
{
public:
	/*! Creates a resource
		\return created resource (NULL if cannot)
	 */ 
	virtual resource::Resource* create() = 0;
	/*! A creator must be inherited to create some stuff
	 */
	virtual ~Creator();
};

/*! A special kind of delegate, which is used by Factory to create resources of specified type
 */
template<
typename _Resource
>
class CreatorFor: public resource::Creator
{
public:
	/*! Creates a resource
		\return created resource (NULL if cannot)
	 */ 
	virtual resource::Resource* create()
	{
		return new _Resource();
	}
	/*! A creator must be inherited to create some stuff
	 */
	virtual ~CreatorFor()
	{
		
	}
};
/*! \class Factory

	Note, that resource factory can create resources by their type string.
 */
class Factory  
{	
public:	
	/*! Creates a factory
	 */
	inline Factory()
	{
		add(sad::Texture::metaData()->name(), new resource::CreatorFor<sad::Texture>());
		add(sad::TextureMappedFont::metaData()->name(), 
			new resource::CreatorFor<sad::TextureMappedFont>());
		add(sad::Sprite2D::Options::metaData()->name(), 
			new resource::CreatorFor<sad::Sprite2D::Options>());
#ifdef USE_FREETYPE
		add(sad::freetype::Font::metaData()->name(), new resource::CreatorFor<sad::freetype::Font>());
#endif
	}
	/*! This class can be inherited 
	 */
	virtual ~Factory();
	/*! Adds new creator to a factory
		\param[in] name a name for factory
		\param[in] c a creator for it
	 */
	virtual void add(const sad::String & name, resource::Creator * c);
	/*! Produces a resource by a string (NULL if cannot)
		\param[in] name a resource name
		\return created resource
	 */
	virtual resource::Resource* create(sad::String name);
protected:
	/*! Creates a hash for returning data
	 */
	sad::PtrHash<sad::String, resource::Creator> m_creators;
private:
	/*! Disabled, factory is uncopyable
		\param[in] o other factory
	 */
	Factory(const Factory & o);
	/*! Disabled, factory is uncopyable
		\param[in] o other factory
		\return self-reference
	 */
	Factory & operator=(const Factory & o);
};

}
