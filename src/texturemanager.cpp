#include "texturemanager.h"
#include "renderer.h"
#include "texture.h"
#include "tga/tga.h"
#include "png/zlib.h"
#include "png/png.h"

void sad::TextureManager::buildAll()
{
	m_m.lock();

	for (hst::hash<hst::string,sad::TextureContainer *>::iterator it=m_containers.begin();
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
	for (hst::hash<hst::string,sad::TextureContainer *>::iterator it=m_containers.begin();
		 it!=m_containers.end();
		 it++)
	delete it.value();
	}
	{
	for (hst::hash<hst::string,sad::TextureLoader *>::iterator it=m_loaders.begin();
		 it!=m_loaders.end();
		 it++)
	delete it.value();
	}
}
sad::Texture *  sad::TextureManager::get(const hst::string & name,const hst::string & containername)
{
	m_m.lock();

	sad::Texture * r=NULL;
	if (m_containers.contains(containername))
		r=m_containers[containername]->get(name);
	
	m_m.unlock();
	return r;
}

void sad::TextureManager::load(const hst::string & name, Texture * tex,const hst::string & containername)
{
	m_m.lock();

	if (m_containers.contains(containername)==false)
		m_containers.insert(containername,new sad::TextureContainer());
	m_containers[containername]->add(name,tex);
	
	m_m.unlock();
}

void sad::TextureManager::unload(const hst::string & name,const hst::string & containername)
{
	m_m.lock();

	if (m_containers.contains(containername))
	{
	m_containers[containername]->remove(name);
	}

	m_m.unlock();
}


void sad::TextureManager::setContainer(sad::TextureContainer * container,const hst::string & containername)
{
	m_m.lock();

	m_containers.insert(containername,container);

	m_m.unlock();
}

sad::TextureContainer * sad::TextureManager::getContainer(const hst::string & containername)
{
	m_m.lock();
	
	sad::TextureContainer * r=NULL;
	if (m_containers.contains(containername))
		r=m_containers[containername];

	m_m.unlock();

	return r;
}

sad::TextureManager * sad::TextureManager::instance()
{
	return sad::Renderer::ref()->textures();
}

sad::TextureLoader * sad::TextureManager::loader(const hst::string & format)
{
	if (m_loaders.contains(format) == false)
		return NULL;
	return m_loaders[format];
}

void sad::TextureManager::setLoader(const hst::string & format, sad::TextureLoader * l)
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