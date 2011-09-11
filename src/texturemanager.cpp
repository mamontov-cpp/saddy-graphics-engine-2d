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

	for (hst::hash<hst::string,sad::Texture *>::iterator it=instance()->m_data.begin();it!=instance()->m_data.end();it++)
		it.value()->buildMipMaps();

	instance()->m_m.unlock();
}
sad::TextureManager::TextureManager()
{

}
sad::TextureManager::~TextureManager()
{
	for (hst::hash<hst::string,sad::Texture *>::iterator it=m_data.begin();it!=m_data.end();it++)
		delete it.value();
}
sad::Texture *  sad::TextureManager::get(const hst::string & name)
{
	m_m.lock();

	sad::Texture * r=NULL;
	if (m_data.contains(name)) r=m_data[name];
	
	m_m.unlock();
	
	return r;
}

void sad::TextureManager::load(const hst::string & name, Texture * tex)
{
	m_m.lock();

	if (m_data.contains(name)) delete m_data[name];
	m_data.insert(name,tex);
	
	m_m.unlock();
}

void sad::TextureManager::unload(const hst::string & name)
{
	m_m.lock();

	if (m_data.contains(name)) delete m_data[name];
	m_data.remove(name);

	m_m.unlock();
}
