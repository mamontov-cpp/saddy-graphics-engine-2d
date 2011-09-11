
#include "fontmanager.h"

using namespace sad;

FontManager FontManager::m_instance;

BasicFont::~BasicFont()
{

}
FontManager::FontManager()
{

}

void FontManager::add(sad::BasicFont * font,const hst::string & name )
{
	m_instance.m_m.lock();

	if (m_instance.m_fonts.contains(name))
		delete m_instance.m_fonts[name];
	m_instance.m_fonts.insert(name,font);
	
	m_instance.m_m.unlock();
}

sad::BasicFont* FontManager::get(const hst::string &key) 
{	
	m_instance.m_m.lock();

	sad::BasicFont * r=NULL;
	if (m_instance.m_fonts.contains(key)) 
	    r=m_instance.m_fonts[key]; 
	
	m_instance.m_m.unlock();

	return r;
}
void FontManager::remove(const hst::string &key) 
{
	m_instance.m_m.lock();

	if (m_instance.m_fonts.contains(key))  
	{ 
		delete m_instance.m_fonts[key];
		m_instance.m_fonts.remove(key);
	}

	m_instance.m_m.unlock();
}
/* destructor delete all fonts object keeping in this manager
*/
FontManager::~FontManager() 
{
	hhash<hString, sad::BasicFont*>::iterator font_iterator;
	font_iterator = m_fonts.begin();
	while (font_iterator != m_fonts.end()) 
	{
		delete font_iterator.value();
		++font_iterator;
	}
}

