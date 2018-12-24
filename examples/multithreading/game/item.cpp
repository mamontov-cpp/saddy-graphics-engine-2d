#include "item.h"

#include "../game.h"

// ========================================== PUBLIC METHODS ==========================================

const int game::Item::SpriteSize = 16;

game::Item::Item(const sad::String& icon, const sad::String& title, const sad::String& description, bool delete_after_apply)
: m_icon(icon), 
m_title(title), 
m_description(description),
m_delete_after_apply(delete_after_apply)
{

}


game::Item::~Item()
{
    if (m_sprite)
    {
        m_sprite->delRef();
    }
}

const sad::String& game::Item::icon() const
{
    return m_icon;
}

const sad::String& game::Item::title() const
{
    return m_title;
}

const sad::String& game::Item::description() const
{
    return m_description;
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

void game::Item::setEvaluatedScript(sad::String* s)
{
    if (s->consistsOfWhitespaceCharacters())
    {
        m_script = NULL;
    }
    else
    { 
        m_script = s;
    }
}

void game::Item::notifyAdded() const
{
    if (evalItemScript())
    {
        // ReSharper disable once CppExpressionWithoutSideEffects
        m_game->evalScript("item.onAdded();");
    }
}

void game::Item::notifyRemoved() const
{
    if (evalItemScript())
    {
        // ReSharper disable once CppExpressionWithoutSideEffects
        m_game->evalScript("item.onRemoved();");
    }
}

void game::Item::applyActiveEffect() const
{
    if (evalItemScript())
    {
        // ReSharper disable once CppExpressionWithoutSideEffects
        m_game->evalScript("item.apply();");
    }
}

bool game::Item::shouldDeleteWhenApplied() const
{
    return m_delete_after_apply;
}

// ========================================== PRIVATE METHODS ==========================================


bool game::Item::evalItemScript() const
{
    if (m_script)
    { 
        if (m_script->length())
        {
            if (m_game->evalScript(sad::String("item = ") + (*m_script) + ";"))
            {
                return true;
            }
        }
    }
    return false;
}