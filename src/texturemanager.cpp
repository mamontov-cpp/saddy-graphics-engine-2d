#include "texturemanager.h"
#include "renderer.h"
#include "texture.h"
#include "sadscopedlock.h"
#include "imageformats/bmploader.h"
#include "imageformats/tgaloader.h"
#include "imageformats/pngloader.h"

void sad::TextureManager::buildAll()
{
	sad::ScopedLock lock(&m_m);

	for (ContainerMap::iterator it = m_containers.begin();
		it != m_containers.end();
		it++
	)
	{
		it.value()->build();
	}
}
sad::TextureManager::TextureManager()
: m_renderer(NULL)
{
  setContainer(new sad::TextureContainer(), "default");
  setLoader("BMP", new sad::imageformats::BMPLoader());
  setLoader("TGA", new sad::imageformats::TGALoader());
  setLoader("PNG", new sad::imageformats::PNGLoader());
}

sad::Texture *  sad::TextureManager::get(const sad::String & name,const sad::String & containername)
{
	sad::ScopedLock lock(&m_m);

	sad::Texture * r = NULL;
	if (m_containers.contains(containername))
	{
		r = m_containers[containername]->get(name);
	}

	return r;
}

void sad::TextureManager::add(const sad::String & name, Texture * tex,const sad::String & containername)
{
	sad::ScopedLock lock(&m_m);

	if (m_containers.contains(containername) == false)
	{
		sad::TextureContainer * c = new sad::TextureContainer();
		c->setManager(this);
		m_containers.insert(containername, c);
	}
	m_containers[containername]->add(name,tex);	
}

void sad::TextureManager::remove(const sad::String & name,const sad::String & containername)
{
	sad::ScopedLock lock(&m_m);

	if (m_containers.contains(containername))
	{
		m_containers[containername]->remove(name);
	}

}


void sad::TextureManager::setContainer(sad::TextureContainer * container,const sad::String & containername)
{
	sad::ScopedLock lock(&m_m);

	container->setManager(this);
	m_containers.insert(containername,container);
}

sad::TextureContainer * sad::TextureManager::getContainer(const sad::String & containername)
{
	sad::ScopedLock lock(&m_m);
	
	sad::TextureContainer * r = NULL;
	if (m_containers.contains(containername))
	{
		r = m_containers[containername];
	}

	return r;
}

sad::TextureManager * sad::TextureManager::ref()
{
	return sad::Renderer::ref()->textures();
}

sad::imageformats::Loader * sad::TextureManager::loader(const sad::String & format)
{
	if (m_loaders.contains(format) == false)
	{
		return NULL;
	}

	return m_loaders[format];
}

void sad::TextureManager::setLoader(
	const sad::String & format, 
	sad::imageformats::Loader * l
)
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
	sad::ScopedLock lock(&m_m);

	for (
		ContainerMap::iterator it = m_containers.begin(); 
		it != m_containers.end();
		it++
	)
	{
		it.value()->unload();
	}

}

sad::Renderer * sad::TextureManager::renderer() const
{
	return m_renderer;
}

void sad::TextureManager::setRenderer(sad::Renderer * renderer)
{
	m_renderer = renderer;
}

