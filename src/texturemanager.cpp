#include "texturemanager.h"
#include "renderer.h"
#include "texture.h"
#include "tga/tga.h"
#include "png/zlib.h"
#include "png/png.h"

void sad::TextureManager::buildAll()
{
	m_m.lock();

	for (sad::Hash<sad::String,sad::TextureContainer *>::iterator it=m_containers.begin();
		it!=m_containers.end();
		it++
		)
		it.value()->build();

	m_m.unlock();
}
sad::TextureManager::TextureManager()
{
  m_containers.insert("default",new sad::TextureContainer());
  setLoader("BMP", new sad::BMPTextureLoader());
  setLoader("TGA", new sad::TGATextureLoader());
  setLoader("PNG", new sad::PNGTextureLoader());

}
sad::TextureManager::~TextureManager()
{
	{
	for (sad::Hash<sad::String,sad::TextureContainer *>::iterator it=m_containers.begin();
		 it!=m_containers.end();
		 it++)
	delete it.value();
	}
	{
	for (sad::Hash<sad::String,sad::TextureLoader *>::iterator it=m_loaders.begin();
		 it!=m_loaders.end();
		 it++)
	delete it.value();
	}
}
sad::Texture *  sad::TextureManager::get(const sad::String & name,const sad::String & containername)
{
	m_m.lock();

	sad::Texture * r=NULL;
	if (m_containers.contains(containername))
		r=m_containers[containername]->get(name);
	
	m_m.unlock();
	return r;
}

void sad::TextureManager::add(const sad::String & name, Texture * tex,const sad::String & containername)
{
	m_m.lock();

	if (m_containers.contains(containername)==false)
		m_containers.insert(containername,new sad::TextureContainer());
	m_containers[containername]->add(name,tex);
	
	m_m.unlock();
}

void sad::TextureManager::remove(const sad::String & name,const sad::String & containername)
{
	m_m.lock();

	if (m_containers.contains(containername))
	{
	m_containers[containername]->remove(name);
	}

	m_m.unlock();
}


void sad::TextureManager::setContainer(sad::TextureContainer * container,const sad::String & containername)
{
	m_m.lock();

	m_containers.insert(containername,container);

	m_m.unlock();
}

sad::TextureContainer * sad::TextureManager::getContainer(const sad::String & containername)
{
	m_m.lock();
	
	sad::TextureContainer * r=NULL;
	if (m_containers.contains(containername))
		r=m_containers[containername];

	m_m.unlock();

	return r;
}

sad::TextureManager * sad::TextureManager::ref()
{
	return sad::Renderer::ref()->textures();
}

sad::TextureLoader * sad::TextureManager::loader(const sad::String & format)
{
	if (m_loaders.contains(format) == false)
		return NULL;
	return m_loaders[format];
}

void sad::TextureManager::setLoader(const sad::String & format, sad::TextureLoader * l)
{
	if (m_loaders.contains(format))
	{
		delete m_loaders[format];
		m_loaders[format] = l;
	}
	else 
	{
		m_loaders.insert(format, l);
	}
	
}

bool sad::TextureManager::hasContainer(const sad::String & name) const
{
	return m_containers.contains(name);
}

void sad::TextureManager::unload()
{
	m_m.lock();

	for (sad::Hash<sad::String,sad::TextureContainer *>::iterator it=m_containers.begin();
		it!=m_containers.end();
		it++
		)
		it.value()->unload();

	m_m.unlock();
}