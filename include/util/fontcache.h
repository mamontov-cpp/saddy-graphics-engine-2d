/*! \file fontcache.h
  
    A font cache for simple synchronized getting font
 */
#pragma once
#include "../resource/link.h"
#include "../sadhash.h"
#include "../sadmutex.h"
#include "../font.h"

namespace sad
{

namespace util
{

/*! A font cache for getting fonts in the run-time
 */
class FontCache
{
public:
    /*! Creates new empty cache
     */
    FontCache();
    /*! Could be inherited
     */
    virtual ~FontCache();
    /*! Copies other cache
        \param[in] cache a cache value
     */
    FontCache(const sad::util::FontCache& cache);
    /*! Copies other cache
        \param[in] cache a cache value
        \return self-reference
     */
    sad::util::FontCache& operator=(const sad::util::FontCache& cache);
    /*! Clears cache
     */
    void clear();
    /*! Tries to get font. If not found, returns default font, which is passed as link
        \param[in] s string
        \param[in] parent_font a default font
        \return font, if can be found
    */
    sad::Font* get(const sad::String& s, sad::resource::Link<sad::Font>& parent_font);
    /*! Sets tree for cache
        \param[in] r renderer, which tree belongs to
        \param[in] tree_name a name for tree
     */
    void setTree(sad::Renderer* r, const sad::String& tree_name);
    /*! Sets renderer for cache
        \param[in] r renderer, which tree belongs to
     */
    void setRenderer(sad::Renderer* r);
private:
    /*! Copies other cache
        \param[in] cache a cache value
     */
    void copy(const sad::util::FontCache& cache);
    /*! A font for document, that will be used for label
    */
    sad::Hash<sad::String, sad::resource::Link<sad::Font> *> m_fonts_for_document;
    /*! A mutex for locking operations
    */
    sad::Mutex m_mtx;
};


}

}