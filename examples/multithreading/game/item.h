/*! \file item.h
 *
 *   Contains a basic in-game item
 */
#pragma once
#include <sadstring.h>
#include <sprite2d.h>

class Game;

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
     *  \param[in] delete_after_apply whether we should apply an item data
     */
    Item(const sad::String& icon, const sad::String& title, const sad::String& description, bool delete_after_apply = false);
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
    /*! A game data for item
     *  \param[in] game a game data
     */
    void setGame(Game* game);
    /*! Sets evaluated script
     *  \param[in] s script
     */
    void setEvaluatedScript(sad::String* s);
    /*! Called when item is added to inventory
     */
    void notifyAdded() const;
    /*! Called, when item is removed from inventory
     */
    void notifyRemoved() const;
    /*! Applies active effect on item
     */
    void applyActiveEffect() const;
    /*! Whether we should delete item, when applied
     *  \return whether we should delete item when applied
     */
    bool shouldDeleteWhenApplied() const;
protected:
    /*! Eval's item script
     *  \return whether item's script evaluation was successfull
     */
    bool evalItemScript() const;
    /*! A game data
     */
    Game* m_game{NULL};
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
    sad::Sprite2D* m_sprite{NULL};
    /*! An evaluated script for applying item data
     */
    sad::String* m_script{NULL};
    /*! Whether we should delete an item, after it was applied
     */
    bool m_delete_after_apply;
};

}