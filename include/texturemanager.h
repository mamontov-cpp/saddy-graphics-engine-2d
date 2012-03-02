/*!  \file    texturemanager.h
     \author  HiddenSeeker
     \brief   Here placed a texturemanager
*/
#include "texturecontainer.h"
#include "os/mutex.h"
#pragma once

namespace sad
{
	/*!  \class TextureManager
	     TextureManager provides an easy access to every texture. He
		 can build mipmaps of every texture. Also, it frees all associated
		 textures, providing a basic garbage collecting
	*/
	class TextureManager
	{
	private:
		sad::TextureContainer *          m_container; //!< Container data
		os::mutex                        m_m;         //!< Mutex to block side effects
		static TextureManager * m_instance;           //!< Current instance of manager
		TextureManager();
		TextureManager(const TextureManager &);
		TextureManager & operator=(const TextureManager &);
		static void freeInstance();
	public:
		/*! Builds all mipmaps
		*/
		static void buildAll();
		/*! Current instance
		    \return current instance of manager
		*/
		static TextureManager * instance();
		/*! Destructor
		*/
		~TextureManager();
		/*! Returns a texture by a name, if exists
		    \param[in] name name of a texture
			\return texture pointer. NULL, if can't be found
		*/
		Texture *  get(const hst::string & name);
		/*! Adds a texture
		    \param[in] name name of a texture
		    \param[in] tex  texture
		*/
		void load(const hst::string & name, Texture * tex);
		/*! Unloads a texture
		    \param[in] name name of a texture
		*/
		void unload(const hst::string & name);
		/*! Sets a new container within manager. An old container DOES NOT destroyed. 
			To change container safely, remove old container manually
			\param[in] container new container
		 */
		void setContainer(sad::TextureContainer * container);
		/*! Returns a container from other container
			\return container of textures
		 */
		sad::TextureContainer * getContainer();
	};

}
