/*!  \file    texturecontainer.h
     \author  HiddenSeeker
     \brief   Here placed a non-thread-safe container for textures, providing access for
			  amounts of containers
*/
#include "texture.h"
#include "sadhash.h"
#pragma once

namespace sad
{

class TextureManager;
class Renderer;

/*!  \class TextureContainer
	 TextureContainer provides an easy access to every texture. He
     can build mipmaps of every texture. Also, it frees all associated
     textures, providing a basic garbage collecting of textures.
	 Also it's not thread-safe, so be careful
*/
class TextureContainer
{
public:
	/*! Creates a new empty container
	*/
	TextureContainer();
	/*! Destructor clears a container, freeing all memory
	 */
	~TextureContainer();
	/*! Adds a new texture to container
		\param[in] name texture name
		\param[in] tex texture
	 */
	void add(const sad::String & name, sad::Texture * tex);
	/*! Removes a texture from container
		\param[in] name texture name
     */
	void remove(const sad::String & name);
	/*! Returns a texture by a name, if exists
	    \param[in] name name of a texture
		\return texture pointer. NULL, if can't be found
	*/
	Texture *  get(const sad::String & name);
	/*! Builds a mipmap for every texture
	 */
	void build();
	/*! Clears a container
	 */
	void clear();
	/*! Unloads every texture from a container
	 */
	void unload();
	/*! Sets a manager for a container
		\param[in] m manager
	 */
	void setManager(sad::TextureManager * m);
	/*! Returns a mahager for a container
		\return manager
	 */
	sad::TextureManager * manager() const;
	/*! Returns a renderer, attached to a manager. 
		\return renderer.
	 */
	sad::Renderer * renderer() const;
private:
	/*! A manager, for interaction with other stuff 
	 */
	sad::TextureManager * m_manager;
	sad::Hash<sad::String,Texture *> m_data; //!< Texture data
};

}
