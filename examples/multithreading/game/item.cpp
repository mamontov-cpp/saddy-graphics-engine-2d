#include "item.h"

#include "../game.h"

#include "../weapons/weapon.h"

// ========================================== game::Item::Definition ==========================================

game::Item::Definition::Definition(const sad::String& icon, const sad::String& title, const sad::String& description, bool delete_after_apply)
: Icon(icon), Title(title), Description(description), DeleteAfterApply(delete_after_apply)
{
    
}

// ========================================== PUBLIC METHODS ==========================================

DECLARE_SOBJ(game::Item);

const int game::Item::SpriteSize = 24;

game::Item::Item(game::Item::Definition* definition)
: m_definition(definition), m_weapon(NULL)
{
    if (!m_definition)
    {
        throw  std::logic_error("Unablet to create item without definition");
    }
    m_definition->addRef();
}


game::Item::~Item()
{
    if (m_sprite)
    {
        m_sprite->delRef();
    }
    if (m_definition)
    {
        m_definition->delRef();
    }
    if (m_weapon)
    {
        m_weapon->delRef();
    }
}

const sad::String& game::Item::icon() const
{
    return m_definition->Icon;
}

const sad::String& game::Item::title() const
{
    return m_definition->Title;
}

const sad::String& game::Item::description() const
{
    return m_definition->Description;
}


void game::Item::setSprite(sad::Sprite2D* sprite)
{
    if (m_sprite)
    {
        m_sprite->delRef();
    }
    m_sprite = sprite;
    if (m_sprite)
    {
        m_sprite->addRef();
    }
}


sad::Sprite2D* game::Item::sprite() const
{
    return m_sprite;
}


void game::Item::setGame(Game* game)
{
    m_game = game;
}

void game::Item::notifyAdded(game::Actor* owner)
{
    this->invokeCompiledFunction(&(m_definition->OnItemAdded), owner);
}

void game::Item::notifyRemoved(game::Actor* owner)
{
    this->invokeCompiledFunction(&(m_definition->OnItemRemoved), owner);
}

void game::Item::applyActiveEffect(game::Actor* owner)
{
    this->invokeCompiledFunction(&(m_definition->ApplyCallback), owner);
}

bool game::Item::shouldDeleteWhenApplied() const
{
    return m_definition->DeleteAfterApply;
}

void game::Item::setGivenWeapon(weapons::Weapon* weapon)
{
   if (weapon)
   {
       weapon->addRef();
   }
   if (m_weapon)
   {
       m_weapon->delRef();
   }
   m_weapon = weapon;
}

void game::Item::removeGivenWeaponFrom(game::Actor* owner)
{
    if (m_weapon)
    {
        owner->removeWeapon(m_weapon);
        m_weapon->delRef();
        m_weapon = NULL;
    }
}

void game::Item::activateGivenWeapon(game::Actor* owner)
{
    if (m_weapon && owner)
    {
        owner->activateWeapon(m_weapon);
    }
}

weapons::Weapon* game::Item::givenWeapon() const
{
    return m_weapon;
}

// ========================================== PRIVATE METHODS ==========================================


void game::Item::invokeCompiledFunction(sad::dukpp03::CompiledFunction* f, game::Actor* owner)
{
    sad::dukpp03::Context* ctx = m_game->context();
    ::dukpp03::PushValue<game::Item*, sad::dukpp03::BasicContext>::perform(ctx, this);
    ::dukpp03::PushValue<game::Actor*, sad::dukpp03::BasicContext>::perform(ctx, owner);
    f->call(ctx);
    ::dukpp03::Maybe<std::string>  maybe_error = ctx->errorOnStack(-1);
    if (maybe_error.exists())
    {
        ctx->renderer()->log()->critical(maybe_error.value().c_str(), __FILE__, __LINE__);
    }
    ctx->cleanStack();
}


void game::exposeItem(void* c)
{
    sad::dukpp03::Context* ctx = reinterpret_cast<sad::dukpp03::Context*>(c);

    // Expose player to script
    sad::dukpp03::ClassBinding* item_binding = new sad::dukpp03::ClassBinding();
    item_binding->addMethod("title", sad::dukpp03::bind_method::from(&game::Item::title));
    item_binding->addMethod("setGivenWeapon", sad::dukpp03::bind_method::from(&game::Item::setGivenWeapon));
    item_binding->addMethod("removeGivenWeaponFrom", sad::dukpp03::bind_method::from(&game::Item::removeGivenWeaponFrom));
    item_binding->addMethod("activateGivenWeapon", sad::dukpp03::bind_method::from(&game::Item::activateGivenWeapon));

    ctx->addClassBinding("game::Item", item_binding);
}
