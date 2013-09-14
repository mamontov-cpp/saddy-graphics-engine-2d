/*!  \file    texturecontainer.h
     \author  HiddenSeeker
     \brief   Here placed a non-thread-safe container for textures, providing access for
			  amounts of containers
*/
#include "texture.h"
#include "templates/hhash.hpp"
#pragma once

namespace sad
{

/*!  \class TextureContainer
	 TextureContainer provides an easy access to every texture. He
     can build mipmaps of every texture. Also, it frees all associated
     textures, providing a basic garbage collecting of textures.
	 Also it's not thread-safe, so be careful
*/
class TextureContainer
{
 	private:
		hst::hash<hst::string,Texture *> m_data; //!< Texture data
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
		void add(const hst::string & name, sad::Texture * tex);
		/*! Removes a texture from container
			\param[in] name texture name
    	 */
		void remove(const hst::string & name);
		/*! Returns a texture by a name, if exists
		    \param[in] name name of a texture
			\return texture pointer. NULL, if can't be found
		*/
		Texture *  get(const hst::string & name);
		/*! Builds a mipmap for every texture
		 */
		void build();
		/*! Clears a container
		 */
		void clear();
		/*! Unloads every texture from a container
		 */
		void unload();
};

}
