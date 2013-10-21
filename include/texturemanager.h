/*!  \file    texturemanager.h
     \author  HiddenSeeker

	This file contains declaration of TextureManager class, which holds all textures, 
	registered in saddy, providing access point for them and loading them on GPU

*/
#include "texturecontainer.h"
#include "sadmutex.h"
#include "imageformats/loader.h"
#include "sadptrhash.h"
#pragma once

namespace sad
{
/*!  \class TextureManager
	 TextureManager provides an easy access to every texture. It
	 can build mipmaps of every texture. Also, it frees all associated
	 textures and associated resources, providing a basic garbage collecting
 */
class TextureManager
{
public:
	/*! Inits an empty manager, which holds no textures and one container with
		name "default".
	 */
	TextureManager();
	/*! Builds all mipmaps for ALL registered textures in manager
	 */
	void buildAll();
	/*! Returns current global instance for manager
		\return current instance of manager
	 */
	static TextureManager * ref();
	/*! Returns a texture by a name, if exists
		\param[in] name name of a texture
		\param[in] containername name of container
		\return texture pointer. NULL, if can't be found
	 */
	sad::Texture *  get(const sad::String & name,const sad::String & containername="default");
	/*! Adds a texture. If container is not found, new container should be created.
		\param[in] name name of a texture
		\param[in] tex  texture
		\param[in] containername name of container
	 */
	void add(const sad::String & name, sad::Texture * tex,const sad::String & containername="default");
	/*! Unloads a texture. If container is not found, nothing is done
		\param[in] name name of a texture
		\param[in] containername name of container
	 */
	void remove(const sad::String & name,const sad::String & containername="default");
	/*! Unloads all textures from videocard memory
	 */
	void unload();	
	/*! Tests, whether manager has a container
		\param[in] name
		\return true if has
	 */
	bool hasContainer(const sad::String & name) const;
	/*! Sets a new container within manager. An old container DOES NOT destroyed. 
		To change container safely, remove old container manually
		\param[in] container new container
		\param[in] containername name of container
	 */
	void setContainer(sad::TextureContainer * container,const sad::String & containername="default");
	/*! Returns a container by name (NULL if not found)
		\return container of textures
		\param[in] containername name of container
	 */
	sad::TextureContainer * getContainer(const sad::String & containername="default");
	/*! Returns a loader for specified format (NULL if not found). A format must be
		all uppercase items
		\param[in] format format data
		\return loader data
	 */
	sad::imageformats::Loader * loader(const sad::String & format);
	/*! Sets a loader for texture
		\param[in] format format data
		\param[in] l loader
	 */
	void setLoader(const sad::String & format, sad::imageformats::Loader * l);
private:
	/*! A hash of loaders for each of formats
	 */
	sad::PtrHash<sad::String, sad::imageformats::Loader>  m_loaders; 
	/*! A shorcut for  map of containers
	 */
	typedef sad::PtrHash<sad::String, sad::TextureContainer> ContainerMap;	
	/*! A hash of containers, where all textures are stored
	 */
	ContainerMap     m_containers;
	/*! A mutex, which is locked, when any container operation is performed
	 */
	::sad::Mutex                        m_m;         		
	/*! Copying is DISABLED
		\param[in] o manager
	 */	
	TextureManager(const TextureManager &);
	/*! Copying is DISABLED
		\param[in] o manager
		\return self-reference
	 */
	TextureManager & operator=(const TextureManager &);
};

}
