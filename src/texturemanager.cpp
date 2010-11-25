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
	if (m_data.contains(name)) return m_data[name];
	return NULL;
}

void sad::TextureManager::load(const hst::string & name, Texture * tex)
{
	if (m_data.contains(name)) delete m_data[name];
	m_data.insert(name,tex);
}

void sad::TextureManager::unload(const hst::string & name)
{
	if (m_data.contains(name)) delete m_data[name];
	m_data.remove(name);
}
