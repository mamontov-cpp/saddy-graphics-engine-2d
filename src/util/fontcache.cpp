#include "util/fontcache.h"

#include "sadscopedlock.h"

// ======================== PUBLIC METHODS ========================

sad::util::FontCache::FontCache()
{

}

sad::util::FontCache::~FontCache()
{
    this->clear();
}


sad::util::FontCache::FontCache(const sad::util::FontCache& cache)
{
    this->copy(cache);
}

sad::util::FontCache& sad::util::FontCache::operator=(const sad::util::FontCache& cache)
{
    this->clear();
    this->copy(cache);
    return *this;
}

void sad::util::FontCache::clear()
{
    m_mtx.lock();
    for (sad::Hash<sad::String, sad::resource::Link<sad::Font> *>::iterator it = m_fonts_for_document.begin();
        it != m_fonts_for_document.end();
        ++it
        )
    {
        delete it.value();
    }
    m_fonts_for_document.clear();
    m_mtx.unlock();
}

sad::Font* sad::util::FontCache::get(const sad::String& s, sad::resource::Link<sad::Font>& parent_font)
{
    sad::ScopedLock lock(&m_mtx);

    if (m_fonts_for_document.contains(s))
    {
        sad::Font* fnt = m_fonts_for_document[s]->get();
        if (fnt == nullptr)
        {
            return parent_font.get();
        }
        else
        {
            return fnt;
        }
    }
    sad::resource::Link<sad::Font>* copy = new sad::resource::Link<sad::Font>(parent_font);
    copy->setPath(s);
    sad::Font* fnt = copy->get();
    if (fnt == nullptr)
    {
        delete copy;
        return parent_font.get();
    }
    else
    {
        m_fonts_for_document.insert(s, copy);
        return fnt;
    }
}

void sad::util::FontCache::setTree(sad::Renderer* r, const sad::String& tree_name)
{
    sad::ScopedLock lock(&m_mtx);

    for (sad::Hash<sad::String, sad::resource::Link<sad::Font> *>::iterator it = m_fonts_for_document.begin();
        it != m_fonts_for_document.end();
        ++it
        )
    {
        it.value()->setTree(r, tree_name);
    }
}

void sad::util::FontCache::setRenderer(sad::Renderer* r)
{
    sad::ScopedLock lock(&m_mtx);

    for (sad::Hash<sad::String, sad::resource::Link<sad::Font> *>::iterator it = m_fonts_for_document.begin();
        it != m_fonts_for_document.end();
        ++it
        )
    {
        it.value()->setRenderer(r);
    }
}

// ======================== PRIVATE METHODS ========================


void sad::util::FontCache::copy(const sad::util::FontCache& cache)
{
    m_mtx.lock();
    const_cast<sad::util::FontCache&>(cache).m_mtx.lock();
    for (sad::Hash<sad::String, sad::resource::Link<sad::Font> *>::iterator it = m_fonts_for_document.begin();
        it != m_fonts_for_document.end();
        ++it
        )
    {
        sad::resource::Link<sad::Font>* copy = new sad::resource::Link<sad::Font>(*(it.value()));
        m_fonts_for_document.insert(it.key(), copy);
    }
    const_cast<sad::util::FontCache&>(cache).m_mtx.unlock();
    m_mtx.unlock();
}
