#include "item.h"


game::Item::Item(const sad::String& icon, const sad::String& title, const sad::String& description)  : m_icon(icon), m_title(title), m_description(description), m_sprite(NULL)
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
