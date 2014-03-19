#include "fontmanager.h"
#include "renderer.h"
#include "sadscopedlock.h"

sad::FontManager::FontManager()
{

}

void sad::FontManager::add(sad::Font * font,const sad::String & name )
{
	sad::ScopedLock lock(&m_m);
	if (m_fonts.contains(name))
	{
		delete m_fonts[name];
	}
	m_fonts.insert(name,font);	
}

sad::Font * sad::FontManager::get(const sad::String &key) 
{	
	sad::ScopedLock lock(&m_m);

	sad::Font * font = NULL;
	if (m_fonts.contains(key)) 
	{
	    font = m_fonts[key]; 
	}
	return font;
}
void sad::FontManager::remove(const sad::String &key) 
{
	sad::ScopedLock lock(&m_m);

	if (m_fonts.contains(key))  
	{ 
		delete m_fonts[key];
		m_fonts.remove(key);
	}
}

sad::FontManager * sad::FontManager::ref()
{
	return sad::Renderer::ref()->fonts();
}
