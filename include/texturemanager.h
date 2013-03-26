/*!  \file    texturemanager.h
     \author  HiddenSeeker
     \brief   Here placed a texturemanager
*/
#include "texturecontainer.h"
#include "os/mutex.h"
#pragma once

namespace sad
{
	class TextureLoader;
	/*!  \class TextureManager
	     TextureManager provides an easy access to every texture. He
		 can build mipmaps of every texture. Also, it frees all associated
		 textures, providing a basic garbage collecting
	*/
	class TextureManager
	{
	private:
		hst::hash<hst::string, sad::TextureContainer *>  m_containers; //!< Container data
		hst::hash<hst::string, sad::TextureLoader *>     m_loaders;     //!< A loaders for various formats
		os::mutex                        m_m;         //!< Mutex to block side effects		
		TextureManager(const TextureManager &);
		TextureManager & operator=(const TextureManager &);
	public:
		TextureManager();
		/*! Builds all mipmaps
		*/
		void buildAll();
		/*! Current instance
		    \return current instance of manager
		*/
		static TextureManager * ref();
		/*! Destructor
		*/
		~TextureManager();
		/*! Returns a texture by a name, if exists
		    \param[in] name name of a texture
			\param[in] containername name of container
			\return texture pointer. NULL, if can't be found
		*/
		Texture *  get(const hst::string & name,const hst::string & containername="default");
		/*! Tests, whether manager has a container
			\param[in] name
			\return true if has
		 */
		bool hasContainer(const hst::string & name) const;
		/*! Adds a texture. If container is not found, new container should be created.
		    \param[in] name name of a texture
		    \param[in] tex  texture
			\param[in] containername name of container
		*/
		void load(const hst::string & name, Texture * tex,const hst::string & containername="default");
		/*! Unloads a texture. If container is not found, nothing is done
		    \param[in] name name of a texture
			\param[in] containername name of container
		*/
		void unload(const hst::string & name,const hst::string & containername="default");
		/*! Sets a new container within manager. An old container DOES NOT destroyed. 
			To change container safely, remove old container manually
			\param[in] container new container
			\param[in] containername name of container
		 */
		void setContainer(sad::TextureContainer * container,const hst::string & containername="default");
		/*! Returns a container from other container
			\return container of textures
			\param[in] containername name of container
		 */
		sad::TextureContainer * getContainer(const hst::string & containername="default");
		/*! Returns a loader for format
			\param[in] format format data
			\return loader data
		 */
		sad::TextureLoader * loader(const hst::string & format);
		/*! Sets a loader for texture
			\param[in] format format data
			\param[in] l loader
		 */
		void setLoader(const hst::string & format, sad::TextureLoader * l);
	};

}
