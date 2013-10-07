#include "sprite2dconfig.h"
#include "sprite2dobserver.h"
#include <texturemanager.h>
#include <assert.h>

using namespace sad;

bool Sprite2DObserverContainer::fireSpriteConsistencyCheck(const Sprite2DTemplateContainer & container)
{
	bool ok=true;
	for (sad::Hash<Sprite2DConfigObserver *,Sprite2DConfigObserver *>::iterator it=m_observers.begin();
		 it!=m_observers.end();
		 it++)
	{
		ok=ok && it.value()->canBeUpdatedFrom(container);
	}
	return ok;
}

void Sprite2DObserverContainer::fireReloadSprite()
{
 for (sad::Hash<Sprite2DConfigObserver *,Sprite2DConfigObserver *>::iterator it=m_observers.begin();
	  it!=m_observers.end();
	  it++)
 {
	 it.value()->notifyConfigChanged();
 }
}

void Sprite2DObserverContainer::fireDying()
{
 for (sad::Hash<Sprite2DConfigObserver *,Sprite2DConfigObserver *>::iterator it=m_observers.begin();
	  it!=m_observers.end();
	  it++)
 {
	 it.value()->notifyConfigGone();
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
	return m_renderer->textures()->getContainer(m_texturecontainername);
}


Sprite2DConfig::Sprite2DConfig(const sad::String & containername, sad::Renderer * r)
{
	m_renderer = r;
	m_loader=NULL;
	m_texturecontainername=containername;
	if (m_renderer->textures()->getContainer(containername)==NULL)
		m_renderer->textures()->setContainer(new sad::TextureContainer(),containername);
}

Sprite2DConfig::~Sprite2DConfig()
{
	m_observers.fireDying();
	delete m_loader;
}

sad::Vector<sad::String> Sprite2DConfig::getTexturesToLoad(const Sprite2DTemplateContainer & container)
{
	sad::Vector<sad::String>  result; //!< Resulting vector
	sad::Hash<sad::String,bool> lookup; //!< Lookup table to filter unique textures
	for (Sprite2DTemplateContainer::const_iterator group=container.const_begin();
		 group!=container.const_end();
		 group++)
	{
		const sad::Hash<int,Sprite2DTemplate> & hash=group.value();
		for (sad::Hash<int,Sprite2DTemplate>::const_iterator template_it=hash.const_begin();
			 template_it!=hash.const_end();
			 template_it++)
		{
			const sad::String & texture_name=template_it.value().textureName();
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
	sad::Vector<sad::String> textures=getTexturesToLoad(newtemplates);
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


	//Set transparency for all of textures, using information from sprites
	for (Sprite2DTemplateContainer::iterator it=newtemplates.begin();it!=newtemplates.end();it++)
	{
		sad::Hash<int,Sprite2DTemplate> & hash=it.value();
		for (sad::Hash<int,Sprite2DTemplate>::iterator it=hash.begin();it!=hash.end();it++)
		{
			if (it.value().isTransparent())
			{
				sad::Texture * tex=newcontainer->get(it.value().textureName());
				tex->setAlpha(0,it.value().transparencyColor(),it.value().textureRect());
			}
		}
	}

	//Set appropriate containers
	m_container=newtemplates;
	delete m_renderer->textures()->getContainer(m_texturecontainername);
	m_renderer->textures()->setContainer(newcontainer,m_texturecontainername);

	//Build mips
	newcontainer->build();

	//Reload all sprites
	fireReloadSprite();

	//Return success
	return SCR_OK;
}
