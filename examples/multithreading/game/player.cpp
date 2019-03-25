#include "player.h"
// ReSharper disable once CppUnusedIncludeDirective
#include <object.h>

#include "../game.h"
#include "../weapons/weapon.h"

#include <dukpp-03/context.h>

DECLARE_COMMON_TYPE(game::Player)

game::Player::Player() : m_is_dead(false)
{
    this->onDeath([=](game::Actor*) {
        this->die();
    });
    // Actor is destroyed automatically, added to make sure actor won't be destroyed by context
    m_actor.addRef();
    m_actor.toggleIsAffectedByWind(true);
}


game::Player::~Player()
{

}

bool game::Player::setActorOptions(game::ActorOptions* opts)
{
    return m_actor.setOptions(opts);
}

void game::Player::onPlatformCollision(const sad::p2d::BasicCollisionEvent & ev)
{
    m_actor.onPlatformCollision(ev);
}


void game::Player::setGame(Game* game)
{
    m_actor.setGame(game);
}

Game* game::Player::game() const
{
    return m_actor.game();
}

void game::Player::reset()
{
    m_inventory.reset();
    m_actor.reset();
    m_is_dead = false;
    setFloaterState(false);
    this->setLives(Game::BasicPlayerLivesCount);
}


void game::Player::init(bool no_sound)
{
    m_actor.init(no_sound);
}

bool game::Player::isFloater() const
{
    return m_actor.isFloater();
}

void game::Player::setFloaterState(bool is_floater)
{
    m_actor.setFloaterState(is_floater);
}

game::Inventory* game::Player::inventory()
{
    return &m_inventory;
}


void game::Player::tryStartGoingUp()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.tryStartGoingUp();
}

void game::Player::tryStopGoingUp()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.tryStopGoingUp();
}

void game::Player::tryStartGoingDown()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.tryStartGoingDown();
}

void game::Player::tryStopGoingDown()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.tryStopGoingDown();
}

void game::Player::tryStartGoingLeft()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.tryStartGoingLeft();
}

void game::Player::tryStopGoingLeft()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.tryStopGoingLeft();
}


void game::Player::tryStartGoingRight()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.tryStartGoingRight();
}

void game::Player::tryStopGoingRight()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.tryStopGoingRight();
}

void game::Player::setSprite(sad::Sprite2D* sprite)
{
    m_actor.setSprite(sprite);
}

void game::Player::setBody(sad::p2d::Body* body)
{
    m_actor.setBody(body);
}

sad::Rect2D game::Player::area() const
{
    if (m_is_dead)
    {
        return {};
    }
    return dynamic_cast<sad::p2d::Rectangle*>(m_actor.body()->currentShape())->rect();
}

sad::Point2D game::Player::middle() const
{
    if (m_is_dead)
    {
        return {};
    }
    return m_actor.middle();
}

void game::Player::testResting()
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.testResting();
}

void game::Player::enableGravity() const
{
    if (m_is_dead)
    {
        return;
    }
    m_actor.enableGravity();
}


game::Actor* game::Player::actor()
{
    return &m_actor;
}

bool game::Player::isLastMovedLeft() const
{
    return m_actor.isLastMovedLeft();
}

void game::Player::toggleInvincibility(bool on)
{
    m_actor.toggleInvincibility(on);
}

bool game::Player::isInvincible() const
{
    return m_actor.isInvincible();
}

int game::Player::lives() const
{
    return m_actor.lives();
}

void game::Player::setLives(int lives)
{
    m_actor.setLives(lives);
}

 void game::Player::incrementLives(int lives)
{
     m_actor.incrementLives(lives);
}

void game::Player::decrementLives(int lives)
{
    m_actor.decrementLives(lives);
}

void game::Player::onDeath(const std::function<void(game::Actor*)>& action)
{
    m_actor.onDeath(action);
}

void game::Player::setHurtAnimation(sad::animations::Animation* animation)
{
    m_actor.setHurtAnimation(animation);
}

void game::Player::tryDecrementLives(int lives)
{
     m_actor.tryDecrementLives(lives);
}

void game::Player::toggleIsDead(bool is_dead)
{
    m_is_dead = is_dead;
}

bool game::Player::isDead() const
{
    return m_is_dead;
}

void game::Player::pushWeapon(weapons::Weapon* w)
{
    m_actor.pushWeapon(w);
}


void game::Player::removeWeapon(weapons::Weapon* w)
{
    m_actor.removeWeapon(w);
}

void game::Player::activateWeapon(weapons::Weapon* w)
{
    m_actor.activateWeapon(w);
}

weapons::Weapon* game::Player::weapon() const
{
    return m_actor.weapon();
}

void game::Player::tryShoot()
{
    m_actor.tryShoot();
}

void game::Player::initPhysics(sad::p2d::World* world, sad::db::Database* db)
{
    sad::Sprite2D* sprite = db->objectByName<sad::Sprite2D>("Player");
    if (sprite)
    {
        this->setSprite(sprite);
        sad::p2d::Body* body = new sad::p2d::Body();
        body->setCurrentAngularVelocity(0);
        body->setCurrentTangentialVelocity(sad::p2d::Vector(0, 0));
        body->setUserObject(sprite);
        sad::p2d::Rectangle* rect = new sad::p2d::Rectangle();
        rect->setRect(sprite->area());
        body->setShape(rect);
        body->attachObject(sprite);
        body->initPosition(sprite->middle());

        world->addBodyToGroup("player", body);
        this->setBody(body);
        this->enableGravity();
        this->init(true);
    }
}

void game::Player::die()
{
    Game* game = this->game();
    this->toggleIsDead(true);
    sad::Sprite2D* local_sprite = m_actor.sprite();
    game->playSound("lose");
    game->physicsWorld()->removeBody(this->actor()->body());
    game->rendererForMainThread()->animations()->stopProcessesRelatedToObject(local_sprite);
    sad::animations::Instance* instance = m_actor.playDeathAnimation();
    instance->end([=]() {  game->changeSceneToLoseScreen(); });
}

void game::Player::onBottomWallCollision()
{
    if (!this->isFloater())
    {
        this->die();
    }
}

void game::Player::incrementAttackModifier(int attack_delta)
{
    m_actor.incrementAttackModifier(attack_delta);
}

void game::Player::decrementAttackModifier(int attack_delta)
{
    m_actor.decrementAttackModifier(attack_delta);
}

void game::Player::incrementDefense(int delta)
{
    m_actor.incrementDefense(delta);
}

void game::Player::decrementDefense(int delta)
{
    m_actor.decrementDefense(delta);
}

int game::Player::defense() const
{
    return m_actor.defense();
}

void game::Player::takeDamage(int base_dmg)
{
    m_actor.takeDamage(base_dmg);
}

void game::Player::updateHorizontalVelocity()
{
    if (!isDead())
    { 
        m_actor.updateHorizontalVelocity();
    }
}

void game::Player::pauseWeaponsReloading()
{
    m_actor.pauseWeaponsReloading();
}

void game::Player::resumeWeaponsReloading()
{
    m_actor.resumeWeaponsReloading();
}


// ===================================== UTILITY FUNCTIONS =====================================

// Player cannot be copied so, disable it here to ensure context proper initialization
namespace sad
{

namespace dukpp03
{


template<>
struct GetAddressOfType<game::Player*, false, false>
{
public:
    /*! Returns address of type, stored in variant.
        \return empty maybe
     */
    static ::dukpp03::Maybe<game::Player*> getAddress(sad::db::Variant*)
    {
        return {};
    }
};

}

}


void game::exposePlayer(void* c, Game* game)
{
    sad::dukpp03::Context* ctx = reinterpret_cast<sad::dukpp03::Context*>(c);

    // Expose player to script
    sad::dukpp03::ClassBinding* player_binding = new sad::dukpp03::ClassBinding();
    player_binding->addMethod("reset", sad::dukpp03::bind_method::from(&game::Player::reset));
    player_binding->addMethod("middle", sad::dukpp03::bind_method::from(&game::Player::middle));
    player_binding->addMethod("area", sad::dukpp03::bind_method::from(&game::Player::area));
    player_binding->addMethod("actor", sad::dukpp03::bind_method::from(&game::Player::actor));
    player_binding->addMethod("isFloater", sad::dukpp03::bind_method::from(&game::Player::isFloater));
    player_binding->addMethod("setFloaterState", sad::dukpp03::bind_method::from(&game::Player::setFloaterState));

    player_binding->addMethod("tryStartGoingUp", sad::dukpp03::bind_method::from(&game::Player::tryStartGoingUp));
    player_binding->addMethod("tryStartGoingDown", sad::dukpp03::bind_method::from(&game::Player::tryStartGoingDown));
    player_binding->addMethod("tryStartGoingLeft", sad::dukpp03::bind_method::from(&game::Player::tryStartGoingLeft));
    player_binding->addMethod("tryStartGoingRight", sad::dukpp03::bind_method::from(&game::Player::tryStartGoingRight));

    player_binding->addMethod("tryStopGoingUp", sad::dukpp03::bind_method::from(&game::Player::tryStopGoingUp));
    player_binding->addMethod("tryStopGoingDown", sad::dukpp03::bind_method::from(&game::Player::tryStopGoingDown));
    player_binding->addMethod("tryStopGoingLeft", sad::dukpp03::bind_method::from(&game::Player::tryStopGoingLeft));
    player_binding->addMethod("tryStopGoingRight", sad::dukpp03::bind_method::from(&game::Player::tryStopGoingRight));
    player_binding->addMethod("setLives", sad::dukpp03::bind_method::from(&game::Player::setLives));
    player_binding->addMethod("incrementLives", sad::dukpp03::bind_method::from(&game::Player::incrementLives));
    player_binding->addMethod("decrementLives", sad::dukpp03::bind_method::from(&game::Player::decrementLives));
    player_binding->addMethod("lives", sad::dukpp03::bind_method::from(&game::Player::lives));
    player_binding->addMethod("toggleInvincibility", sad::dukpp03::bind_method::from(&game::Player::toggleInvincibility));
    player_binding->addMethod("isInvincible", sad::dukpp03::bind_method::from(&game::Player::isInvincible));

    player_binding->addMethod("testResting", sad::dukpp03::bind_method::from(&game::Player::testResting));
    player_binding->addMethod("enableGravity", sad::dukpp03::bind_method::from(&game::Player::enableGravity));

    player_binding->addMethod("pushWeapon", sad::dukpp03::bind_method::from(&game::Player::pushWeapon));
    player_binding->addMethod("removeWeapon", sad::dukpp03::bind_method::from(&game::Player::removeWeapon));
    player_binding->addMethod("activateWeapon", sad::dukpp03::bind_method::from(&game::Player::activateWeapon));
    player_binding->addMethod("weapon", sad::dukpp03::bind_method::from(&game::Player::weapon));
    player_binding->addMethod("tryShoot", sad::dukpp03::bind_method::from(&game::Player::tryShoot));

    player_binding->addMethod("incrementAttackModifier", sad::dukpp03::bind_method::from(&game::Player::incrementAttackModifier));
    player_binding->addMethod("decrementAttackModifier", sad::dukpp03::bind_method::from(&game::Player::decrementAttackModifier));
    player_binding->addMethod("incrementDefense", sad::dukpp03::bind_method::from(&game::Player::incrementDefense));
    player_binding->addMethod("decrementDefense", sad::dukpp03::bind_method::from(&game::Player::decrementDefense));
    player_binding->addMethod("defense", sad::dukpp03::bind_method::from(&game::Player::defense));


    ctx->addClassBinding("game::Player", player_binding);
    std::function<game::Player*()> player = [=]() { return game->player(); };
    ctx->registerCallable("player", sad::dukpp03::make_lambda::from(player));
}
