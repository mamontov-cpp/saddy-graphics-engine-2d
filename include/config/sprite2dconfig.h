/*! \file   sprite2dconfig.h
	\author HiddenSeeker
	
	A config for 2d sprites hot reload
 */
#include "sprite2dconfigloader.h"
#include "../texturecontainer.h"
#pragma once

// An observer, which listens to config changes
class Sprite2DConfigObserver;


/*! Reloading srites are failed
 */
enum Sprite2DConfigLoadingResult
{
	SCR_OK = 0,             //!< Data is successfully loaded
	SCR_NOLOADER = 1,       //!< No loader is specified for config
	SCR_LOADINGFAILED = 2 , //!< Config loading from file is failed
	SCR_TEXTURE = 3,        //!< Texture loading failed
	SCR_ABSENT =4           //!< Templates for reloading sprites are absent
};

/*!  \class Sprite2DObserverContainer
     A container for working with observers
 */
class Sprite2DObserverContainer
{
 private:
		 hst::hash<Sprite2DConfigObserver *,Sprite2DConfigObserver *> m_observers; //!< An observers for config
 public: 
		 /*! A container is empty at beginning
		  */
		 inline Sprite2DObserverContainer() {}
		 /*! Adds a new observer
			 \param[in] observer new observer to be added
		  */
		 inline void add(Sprite2DConfigObserver * observer) { m_observers.insert(observer,observer); }
		 /*! Removes an observer from list
			 \param[in] observer new observer to be added
		  */
		 inline void remove(Sprite2DConfigObserver * observer) { m_observers.remove(observer); }
		 /*! Fires a sprite consistency check, checking, whether all sprites are saved
			 \param[in] container container for checking consistency
		  */
		 bool fireSpriteConsistencyCheck(const Sprite2DTemplateContainer & container);
		 /*! Fires a reload sprite event, forcing observers to react to sprite changes
		  */
		 void fireReloadSprite();
		 /*! Notifies observers, that config is gone and no need to notify him about death
		  */
		 void fireDying();
		 /*! A destructor for observer
		  */
		 inline ~Sprite2DObserverContainer() {}
};

/*! \class Sprite2DConfig
    A config for 2d sprite, which supports hot reload
 */
class  Sprite2DConfig
{
 private:
		Sprite2DTemplateContainer  m_container;  //!< A container for sprites
		Sprite2DConfigLoader    *  m_loader;     //!< A loader for sprites
		Sprite2DObserverContainer  m_observers; //!< An observers for config
		hst::string                m_texturecontainername;  //!< A name for container of texture
		/*! Fires a sprite consistency check, checking, whether all sprites are saved
			\param[in] container container for checking consistency
		 */
		bool fireSpriteConsistencyCheck(const Sprite2DTemplateContainer & container);
		/*! Fires a reload sprite event, forcing observers to react to sprite changes
		 */
		void fireReloadSprite();
		/*! Get available textures
			\param[in] container container of textures
			\return list of textures to load
		 */
		hst::vector<hst::string> getTexturesToLoad(const Sprite2DTemplateContainer & container);
 public:
	    /*! Creates a config, with associated container
			\param[in] containername container, where textures can be reloaded
		 */
	    Sprite2DConfig(const hst::string & containername);
		/*! Destructor of sprite config, must delete loader
		 */
		~Sprite2DConfig();
		/*! Returns a texture container, associated with config
			\return texture container
		 */
		sad::TextureContainer * getTextures();
		/*! Returns a templates from config
			\return templates
		 */
		inline const Sprite2DTemplateContainer & getTemplates() { return m_container; }
		/*! Sets new loader for config
			\param[in] loader new loader
		 */
		void setLoader(Sprite2DConfigLoader * loader);
		/*! Registers a new observer
			\param[in] obs observer
		 */
		inline void addObserver(Sprite2DConfigObserver * obs) { m_observers.add(obs); }
		/*! Unregisters an observer
			\param[in] obs observer
		 */
		inline void removeObserver(Sprite2DConfigObserver * obs) { m_observers.remove(obs); }
		/*! Reloads a config
			\return result of loading
		 */
		Sprite2DConfigLoadingResult reload();
};

