/*! \file item.h
 *
 *   Contains a basic in-game item
 */
#pragma once
#include <sadstring.h>
#include <sprite2d.h>

namespace game 
{

 /*! A basic in-game item
  */
class Item   // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
    /*! Constructs new item
     *  \param[in] icon an icon for item
     *  \param[in] title a title for item
     *  \param[in] description a description for item
     */
    Item(const sad::String& icon, const sad::String& title, const sad::String& description);
    /*! An item can be inherited to implement own items
     */
    virtual ~Item();
    /*! Returns icon for an item
     *  \return icon an icon for item
     */
    const sad::String& icon() const;
    /*! Returns title for an item
     *  \return title for an item
     */
    const sad::String& title() const;
    /*! Returns description for an item
     *  \return  a description for an item
     */
    const sad::String& description() const;
    /*! Sets sprite for an item
     *  \param[in] sprite a sprite part
     */
    void setSprite(sad::Sprite2D* sprite);
    /*! Returns a sprite for an item
     *  \return sprite
     */
    sad::Sprite2D* sprite() const;
protected:
    /*! An icon for item
     */
    sad::String m_icon;
    /*! A highlight title for item
     */
    sad::String m_title;
    /*! A description for item
     */
    sad::String m_description;
    /*! A sprite for an item
     */
    sad::Sprite2D* m_sprite;
};

}