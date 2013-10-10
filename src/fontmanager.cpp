#include "fontmanager.h"
#include "renderer.h"

using namespace sad;


BasicFont::~BasicFont()
{

}
FontManager::FontManager()
{

}

void FontManager::add(sad::BasicFont * font,const sad::String & name )
{
	m_m.lock();

	if (m_fonts.contains(name))
		delete m_fonts[name];
	m_fonts.insert(name,font);
	
	m_m.unlock();
}

sad::BasicFont* FontManager::get(const sad::String &key) 
{	
	m_m.lock();

	sad::BasicFont * r=NULL;
	if (m_fonts.contains(key)) 
	    r=m_fonts[key]; 
	
	m_m.unlock();

	return r;
}
void FontManager::remove(const sad::String &key) 
{
	m_m.lock();

	if (m_fonts.contains(key))  
	{ 
		delete m_fonts[key];
		m_fonts.remove(key);
	}

	m_m.unlock();
}
/* destructor delete all fonts object keeping in this manager
*/
FontManager::~FontManager() 
{
	sad::Hash<sad::String, sad::BasicFont*>::iterator font_iterator;
	font_iterator = m_fonts.begin();
	while (font_iterator != m_fonts.end()) 
	{
		delete font_iterator.value();
		++font_iterator;
	}
}

FontManager * FontManager::ref()
{
	return sad::Renderer::ref()->fonts();
}