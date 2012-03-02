#include "texturemanager.h"

sad::TextureManager * sad::TextureManager::m_instance=NULL;

void sad::TextureManager::freeInstance()
{
	delete m_instance;
}

sad::TextureManager * sad::TextureManager::instance()
{
	if (!m_instance) { m_instance=new sad::TextureManager(); }
	return m_instance;
}
void sad::TextureManager::buildAll()
{
	instance()->m_m.lock();

	instance()->m_container->build();

	instance()->m_m.unlock();
}
sad::TextureManager::TextureManager()
{
  m_container=new sad::TextureContainer();
}
sad::TextureManager::~TextureManager()
{
  delete m_container;
}
sad::Texture *  sad::TextureManager::get(const hst::string & name)
{
	m_m.lock();
	sad::Texture * r=m_container->get(name);
	m_m.unlock();
	return r;
}

void sad::TextureManager::load(const hst::string & name, Texture * tex)
{
	m_m.lock();

	m_container->add(name,tex);
	
	m_m.unlock();
}

void sad::TextureManager::unload(const hst::string & name)
{
	m_m.lock();

	m_container->remove(name);

	m_m.unlock();
}


void sad::TextureManager::setContainer(sad::TextureContainer * container)
{
	m_m.lock();
	m_container=container;
	m_m.unlock();
}

sad::TextureContainer * sad::TextureManager::getContainer()
{
	m_m.lock();
	sad::TextureContainer * r=m_container;
	m_m.unlock();
	return r;
}