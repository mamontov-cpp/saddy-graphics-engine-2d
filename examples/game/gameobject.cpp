#include "gameobject.h"
#include "game.h"

DECLARE_SOBJ_INHERITANCE(GameObject, sad::phy52d::Object)

GameObject::GameObject(): sad::phy52d::Object()
{
    m_hp = 1;
}

GameObject::~GameObject()  // NOLINT(modernize-use-equals-default)
{
    for (size_t i = 0; i < m_guns.count(); i++)
    {
        delete m_guns[i];
    }
}

void GameObject::render()
{
    this->sad::phy52d::Object::render();
    for(size_t i = 0; i < m_guns.count(); i++)
    {
        m_guns[i]->tryShoot();;
    }
}

int GameObject::hitPoints() const
{
    return m_hp;
}

void GameObject::incrementHP(int count)
{
    m_hp += count;
}

void GameObject::decrementHP(int count)
{
    m_hp -= count;

    // If hp less than zero - remove object
    if (m_hp <= 0 && this->game() != nullptr)
    {
        this->game()->removeObject(this);
    }
}

Game * GameObject::game() const
{
    if (m_app == nullptr)
    {
        return nullptr;
    }
    return static_cast<Game *>(m_app);
}

void GameObject::addGun(AbstractAutomaticGun * gun)
{
    m_guns << gun;
    gun->setObject(this);
}

