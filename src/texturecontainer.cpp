#include "texturecontainer.h"

sad::TextureContainer::TextureContainer()
{

}

sad::TextureContainer::~TextureContainer()
{
	clear();
}

void sad::TextureContainer::add(const sad::String & name, sad::Texture * tex)
{
	if (m_data.contains(name)) delete m_data[name];
	m_data.insert(name,tex);
}


void sad::TextureContainer::remove(const sad::String & name)
{
	if (m_data.contains(name)) 
	{
	 delete m_data[name];
	 m_data.remove(name);
	}
}

sad::Texture *  sad::TextureContainer::get(const sad::String & name)
{
	sad::Texture * r=NULL;
	if (m_data.contains(name)) r=m_data[name];
	return r;
}

void sad::TextureContainer::build()
{
	for (sad::Hash<sad::String,sad::Texture *>::iterator it=m_data.begin();it!=m_data.end();it++)
		it.value()->upload();
}

void  sad::TextureContainer::clear()
{
	for (sad::Hash<sad::String,sad::Texture *>::iterator it=m_data.begin();it!=m_data.end();it++)
		delete it.value();
	m_data.clear();
}

void sad::TextureContainer::unload()
{
	for (sad::Hash<sad::String,sad::Texture *>::iterator it=m_data.begin();it!=m_data.end();it++)
		it.value()->unload();
}