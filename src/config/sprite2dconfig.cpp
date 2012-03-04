#include "sprite2dconfig.h"
#include "sprite2dobserver.h"
#include <texturemanager.h>
#include <assert.h>



bool Sprite2DObserverContainer::fireSpriteConsistencyCheck(const Sprite2DTemplateContainer & container)
{
	bool ok=true;
	for (hst::hash<Sprite2DConfigObserver *,Sprite2DConfigObserver *>::iterator it=m_observers.begin();
		 it!=m_observers.end();
		 it++)
	{
		ok=ok && it.value()->canBeUpdatedFrom(container);
	}
	return ok;
}

void Sprite2DObserverContainer::fireReloadSprite()
{
 for (hst::hash<Sprite2DConfigObserver *,Sprite2DConfigObserver *>::iterator it=m_observers.begin();
	  it!=m_observers.end();
	  it++)
 {
	 it.value()->notifyConfigChanged();
 }
}


bool Sprite2DConfig::fireSpriteConsistencyCheck(const Sprite2DTemplateContainer & container)
{
	return m_observers.fireSpriteConsistencyCheck(container);
}

void Sprite2DConfig::fireReloadSprite()
{
	m_observers.fireReloadSprite();
}

sad::TextureContainer * Sprite2DConfig::getTextures()
{
	return sad::TextureManager::instance()->getContainer(m_texturecontainername);
}


Sprite2DConfig::Sprite2DConfig(const hst::string & containername)
{
	m_loader=NULL;
	m_texturecontainername=containername;
	if (sad::TextureManager::instance()->getContainer(containername)==NULL)
		sad::TextureManager::instance()->setContainer(new sad::TextureContainer(),containername);
}

Sprite2DConfig::~Sprite2DConfig()
{
	delete m_loader;
}

hst::vector<hst::string> Sprite2DConfig::getTexturesToLoad(const Sprite2DTemplateContainer & container)
{
	hst::vector<hst::string>  result; //!< Resulting vector
	hst::hash<hst::string,bool> lookup; //!< Lookup table to filter unique textures
	for (Sprite2DTemplateContainer::const_iterator group=container.const_begin();
		 group!=container.const_end();
		 group++)
	{
		const hst::hash<int,Sprite2DTemplate> & hash=group.value();
		for (hst::hash<int,Sprite2DTemplate>::const_iterator template_it=hash.const_begin();
			 template_it!=hash.const_end();
			 template_it++)
		{
			const hst::string & texture_name=template_it.value().textureName();
			if (lookup.contains(texture_name)==false)
			{
				lookup.insert(texture_name,true);
				result<<texture_name;
			}
		}
	}
	return result;
}


void Sprite2DConfig::setLoader(Sprite2DConfigLoader * loader)
{
	delete m_loader;
	m_loader=loader;
}

Sprite2DConfigLoadingResult Sprite2DConfig::reload()
{
	//If no loader specified - an error occured
	if (m_loader==NULL)
		return SCR_NOLOADER;

	Sprite2DTemplateContainer newtemplates;  //!< New templates for config
	
	//If we can not load new templates, exit
	if (m_loader->load(newtemplates)==false)
		return SCR_LOADINGFAILED;

	//If there are unfilled config data, stop right
	if (this->fireSpriteConsistencyCheck(newtemplates)==false)
	{
		return SCR_ABSENT;
	}

	//Try to load new textures
	hst::vector<hst::string> textures=getTexturesToLoad(newtemplates);
	bool ok=true; //!< Whether textures loading was successful
	//New container, where every texture is stored
	sad::TextureContainer * newcontainer=new sad::TextureContainer();
	for (unsigned int i=0;(i<textures.count()) && ok;i++)
	{
		sad::Texture * texture=new sad::Texture();
		if (texture->load(textures[i]))
		{
			//If load is successfull add it to container
			newcontainer->add(textures[i],texture);
		}
		else
		{
			//Remove a texture
			ok=false;
			delete texture;
		}
	}

	//If texture loading is failed, remove container and report of error
	if (ok==false)
	{
		delete newcontainer;
		return SCR_TEXTURE;
	}

	//Set appropriate containers
	m_container=newtemplates;
	delete sad::TextureManager::instance()->getContainer(m_texturecontainername);
	sad::TextureManager::instance()->setContainer(newcontainer,m_texturecontainername);

	//Build mips
	newcontainer->build();

	//Reload all sprites
	fireReloadSprite();

	//Return success
	return SCR_OK;
}