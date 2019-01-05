/*! \file item.h
 *
 *   Contains a basic in-game item
 */
#pragma once
#include <sadstring.h>
#include <sprite2d.h>
#include <dukpp-03/context.h>

class Game;

namespace weapons
{
class Weapon;
}

namespace game 
{

class Actor;

 /*! A basic in-game item
  */
class Item: public sad::Object   // NOLINT(cppcoreguidelines-special-member-functions)
{
SAD_OBJECT
public:
/*! A definition for item
 */
struct Definition: public sad::RefCountable
{
    sad::String Icon;           //!< An icon
    sad::String Title;          //!<  A title
    sad::String Description;    //!< A description for item
    /*! Whether we should delete an item, after it was applied
     */
    bool DeleteAfterApply;
    /*! Constructs new definition for item
     *  \param[in] icon an icon data
     *  \param[in] title a title for item
     *  \param[in] description a description for item
     *  \param[in] delete_after_apply whether we should delete item, after it was applied
     */
    Definition(
        const sad::String& icon, 
        const sad::String& title, 
        const sad::String& description,
        bool delete_after_apply = false
    );
    /*! A callback on item added
     */
    sad::dukpp03::CompiledFunction OnItemAdded;
    /*! A callback on item removed
     */
    sad::dukpp03::CompiledFunction OnItemRemoved;
    /*! A callback on item applied
     */
    sad::dukpp03::CompiledFunction ApplyCallback;
};

    /*! Size of item's sprite
     */
    static const int SpriteSize;
    /*! Constructs new item
     *  \param[in] definition a definition for item
     */
    Item(game::Item::Definition* definition);
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
    /*! Called when item is added to inventory
     *  \param[in] owner an actor owner
     */
    void notifyAdded(game::Actor* owner);
    /*! Called, when item is removed from inventory
     *  \param[in] owner an actor owner
     */
    void notifyRemoved(game::Actor* owner);
    /*! Applies active effect on item
     *  \param[in] owner an actor owner
     */
    void applyActiveEffect(game::Actor* owner);
    /*! Whether we should delete item, when applied
     *  \return whether we should delete item when applied
     */
    bool shouldDeleteWhenApplied() const;
    /*! Sets given weapon for item
     *  \param[in] weapon a weapon
     */
    void setGivenWeapon(weapons::Weapon* weapon);
    /*! Removes given weapon from owner
     *  \param[in] owner an owner
     */
    void removeGivenWeaponFrom(game::Actor* owner);
    /*! Returns given weapon for item
     *  \return given weapon
     */
    weapons::Weapon* givenWeapon() const;
protected:
    /*! Invokes compiled function
     *  \param[in] f function
     *  \param[in] owner an owner
     */
    void invokeCompiledFunction(sad::dukpp03::CompiledFunction* f, game::Actor* owner);
    /*! A game data
     */
    Game* m_game{NULL};
    /*! A sprite for an item
     */
    sad::Sprite2D* m_sprite{NULL};
    /*! A definition for item
     */
    game::Item::Definition* m_definition;
    /*! A local weapon for item, given for user
     */
    weapons::Weapon* m_weapon;
};

/*! Exposes player to context
    \param[in] c context
 */
void exposeItem(void* c);

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(game::Item);