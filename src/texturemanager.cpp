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

	for (hst::hash<hst::string,sad::TextureContainer *>::iterator it=instance()->m_containers.begin();
		it!=instance()->m_containers.end();
		it++
		)
		it.value()->build();

	instance()->m_m.unlock();
}
sad::TextureManager::TextureManager()
{
  m_containers.insert("default",new sad::TextureContainer());
}
sad::TextureManager::~TextureManager()
{
	for (hst::hash<hst::string,sad::TextureContainer *>::iterator it=m_containers.begin();
		 it!=m_containers.end();
		 it++)
	delete it.value();
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