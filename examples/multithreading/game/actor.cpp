#include "actor.h"
#include "../game.h"

#include "../weapons/weapon.h"

#include <cmath>
#include <cstdio>

#include <p2d/world.h>
#include <p2d/collides1d.h>
#include <p2d/infiniteline.h>

#include <animations/animationssimplemovement.h>
#include <animations/animationsrotate.h>
#include <animations/animationsparallel.h>
#include <animations/animationsanimations.h>


#include <dukpp-03/context.h>

#include <object.h>


DECLARE_SOBJ(game::Actor)


// ============================================== PUBLIC METHODS ==============================================

game::Actor::Actor() : m_own_horizontal_velocity(0),
m_sprite(NULL),
m_body(NULL),
m_is_resting(false),
m_is_ducking(false),
m_is_free_fall(false),
m_is_walking_animation_playing(false),
m_is_jumping_animation_playing(false), 
m_resting_platform(NULL),
m_is_floater(false),
m_is_dying(false),
m_walking_animation(NULL),
m_walking_instance(NULL),
m_jumping_animation(NULL),
m_jumping_instance(NULL),
m_game(NULL),
m_options(NULL),
m_is_last_moved_left(false),
m_is_invincible(false),
m_lives(1),
m_hurt_animation(NULL),
m_lookup_angle(0),
m_fixed_size(false),
m_attack_modifier(0),
m_defense(0),
m_floater_state_counter(0),
m_affected_by_wind(false)
{
    m_key_states.reset();
}

game::Actor::~Actor()
{
    if (m_options)
    {
        m_options->delRef();
    }

    for(size_t i = 0; i < m_weapons.size(); i++)
    {
        m_weapons[i]->delRef();
    }
    m_weapons.clear();

    if (m_walking_instance)
    {
        m_walking_instance->delRef();
    }
    if (m_walking_animation)
    {
        m_walking_animation->delRef();
    }
 
    if (m_jumping_animation)
    {
        m_jumping_animation->delRef();
    }
    
    if (m_jumping_instance)
    {
        m_jumping_instance->delRef();
    }
}

bool game::Actor::setOptions(game::ActorOptions* opts)
{
    if (m_options)
    {
        return false;
    }
    
    if (!opts)
    {
        return false;
    }
    opts->validate();
    opts->addRef();
    m_options = opts;
    m_is_floater = m_options->IsFloater;

    m_walking_animation = new sad::animations::OptionList();
    m_walking_animation->setList(opts->WalkingAnimationOptions);
    m_walking_animation->setTime(opts->WalkingAnimationTime);
    m_walking_animation->setLooped(true);
    m_walking_animation->addRef();

    m_walking_instance = new sad::animations::Instance();
    m_walking_instance->setAnimation(m_walking_animation);
    m_walking_instance->addRef();

    m_jumping_animation = new sad::animations::OptionList();
    m_jumping_animation->setList(opts->JumpingAnimationOptions);
    m_jumping_animation->setTime(opts->JumpingAnimationTime);
    m_jumping_animation->setLooped(false);
    m_jumping_animation->addRef();

    m_jumping_instance = new sad::animations::Instance();
    m_jumping_instance->setAnimation(m_jumping_animation);
    m_jumping_instance->end([this]() { this->m_is_jumping_animation_playing = false; }); 
    m_jumping_instance->addRef();

    return true;
}

void game::Actor::tryStartGoingUp()
{
    m_key_states.set(game::Actor::ABTN_UP);

    m_key_states.reset(LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_UP);
    m_key_states.reset(LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_DOWN);

    m_key_states.set(LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_UP);
    
    if (m_options)
    {
        if (m_is_floater)
        {
            this->disableResting();
            this->setAngleForFloater();
            this->setVerticalVelocity(m_options->FloaterVerticalVelocity);
        }
        else
        {
            this->tryJump();
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void game::Actor::tryStopGoingUp()
{
    m_key_states.reset(game::Actor::ABTN_UP);
    m_key_states.reset(LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_UP);

    if (m_options)
    {
        if (m_is_floater)
        {
            this->setAngleForFloater();
            bool is_going_up = false, is_going_down = false;
            this->computeIsGoingUpDownFlags(is_going_up, is_going_down);
            if (is_going_down)
            { 
                this->setVerticalVelocity(m_options->FloaterVerticalVelocity * - 1);
            }
            else
            {
                this->setVerticalVelocity(0.0);
            }
        }
    }
}

void game::Actor::tryStartGoingDown()
{
    m_key_states.set(game::Actor::ABTN_DOWN);

    m_key_states.reset(LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_UP);
    m_key_states.reset(LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_DOWN);

    m_key_states.set(LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_DOWN);

    if (m_options)
    {
        if (m_is_floater)
        {
            this->setAngleForFloater();
            this->setVerticalVelocity(m_options->FloaterVerticalVelocity * - 1);
        }
        else
        {
            this->startFallingOrDuck();
        }
    }
}

void game::Actor::tryStopGoingDown()
{
    m_key_states.reset(game::Actor::ABTN_DOWN);
    m_key_states.reset(LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_DOWN);

    if (m_options)
    {
        if (m_is_floater)
        {
            this->setAngleForFloater();
            bool is_going_up = false, is_going_down = false;
            this->computeIsGoingUpDownFlags(is_going_up, is_going_down);
            if (is_going_up)
            {
                this->disableResting();
                this->setVerticalVelocity(m_options->FloaterVerticalVelocity);
            }
            else
            {
                this->setVerticalVelocity(0.0);
            }
        }
        else
        {
            this->stopFallingOrStopDucking();
        }
    }
}

void game::Actor::tryStartGoingLeft()
{
    m_is_last_moved_left = true;
    m_key_states.set(game::Actor::ABTN_LEFT);

    m_key_states.reset(LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_LEFT);
    m_key_states.reset(LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_RIGHT);

    m_key_states.set(LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_LEFT);

    if (m_options)
    {
        if (m_is_floater)
        {
            this->setAngleForFloater();
        }
        this->startMovingLeft();
    }
}

void game::Actor::tryStopGoingLeft()
{
    m_key_states.reset(game::Actor::ABTN_LEFT);
    m_key_states.reset(LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_LEFT);

    if (m_options)
    {
        bool is_going_left = false, is_going_right = false;
        this->computeIsGoingLeftRightFlags(is_going_left, is_going_right);
        if (m_is_floater)
        {
            this->setAngleForFloater();
        }
        if (is_going_right)
        {
            this->startMovingRight();
        }
        else
        {
            this->stopMovingHorizontally();
        }
    }
}


void game::Actor::tryStartGoingRight()
{
    m_is_last_moved_left = false;
    m_key_states.set(game::Actor::ABTN_RIGHT);

    m_key_states.reset(LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_LEFT);
    m_key_states.reset(LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_RIGHT);

    m_key_states.set(LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_RIGHT);

    if (m_options)
    {
        if (m_is_floater)
        {
            this->setAngleForFloater();
        }        
        this->startMovingRight();
    }
}

void game::Actor::tryStopGoingRight()
{
    m_key_states.reset(game::Actor::ABTN_RIGHT);
    m_key_states.reset(LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_RIGHT);

    if (m_options)
    {
        bool is_going_left = false, is_going_right = false;
        this->computeIsGoingLeftRightFlags(is_going_left, is_going_right);
        if (m_is_floater)
        {
            this->setAngleForFloater();
        }
        if (is_going_left)
        {
            this->startMovingLeft();
        }
        else
        {
            this->stopMovingHorizontally();
        }
    }
}

void game::Actor::onPlatformCollision(const sad::p2d::BasicCollisionEvent & ev)
{
    sad::p2d::BounceSolver* bounce_solver = m_game->bounceSolver();
    if (!bounce_solver->bounce(ev.m_object_1, ev.m_object_2))
    {
        return;
    }
    sad::Point2D current_position_1 = ev.m_object_1->position();
    sad::Point2D next_position_1 = ev.m_object_1->nextPosition();

    sad::Point2D current_position_2 = ev.m_object_2->position();
    sad::Point2D next_position_2 = ev.m_object_2->positionAt(ev.m_object_2->world()->timeStep());

    sad::Rect2D shape_1 = dynamic_cast<sad::p2d::Rectangle*>(ev.m_object_1->currentShape())->rect();
    sad::Rect2D shape_2 = dynamic_cast<sad::p2d::Rectangle*>(ev.m_object_2->currentShape())->rect();

    sad::moveBy(next_position_1 - current_position_1, shape_1);
    sad::moveBy(next_position_2 - current_position_2, shape_2);


    double min_player_y = std::min(shape_1[0].y(), shape_1[2].y());
    double max_platform_y = std::max(shape_2[0].y(), shape_2[2].y());

    sad::p2d::Cutter1D player_part(std::min(shape_1[0].x(), shape_1[2].x()), std::max(shape_1[0].x(), shape_1[2].x()));
    sad::p2d::Cutter1D platform_part(std::min(shape_2[0].x(), shape_2[2].x()), std::max(shape_2[0].x(), shape_2[2].x()));

    if ((sad::is_fuzzy_equal(max_platform_y, min_player_y) || (min_player_y > max_platform_y))
        && (sad::p2d::collides(player_part, platform_part)))
    {
        SL_LOCAL_DEBUG("Making bodies rest", *(this->game()->rendererForMainThread()));
        sad::p2d::Vector v = ev.m_object_2->tangentialVelocity();
        this->restOnPlatform(ev.m_object_2, v);
    }
}

void game::Actor::setGame(Game* game)
{
    m_game = game;
}

Game* game::Actor::game() const
{
    return m_game;
}

void game::Actor::reset()
{
    m_key_states.reset();

    m_sprite = NULL;
    m_body = NULL;
    m_is_resting = false;
    m_is_ducking = false;
    m_is_free_fall = false;
    m_resting_platform = NULL;
    m_own_horizontal_velocity = 0;
    m_old_velocity = sad::p2d::Vector(0, 0);
    m_is_walking_animation_playing = false;
    m_is_jumping_animation_playing = false;
    m_attack_modifier = 0;
    m_defense = 0;
    m_floater_state_counter = 0;

    for (size_t i = 0; i < m_weapons.size(); i++)
    {
        m_weapons[i]->delRef();
    }
    m_weapons.clear();

    if (m_options)
    {
        m_is_floater = m_options->IsFloater;
    }
}

void game::Actor::setVelocity(const sad::p2d::Vector& v) const
{
    m_body->setCurrentTangentialVelocity(v);
}


void game::Actor::init(bool no_sound)
{
    if (!m_sprite || !m_options)
    {
        return;
    }
    bool can_emit_sound = m_options->CanEmitSound;
    if (no_sound)
    {
        m_options->CanEmitSound  = false;
    }
    bool is_going_up = false;
    bool is_going_down = false;
    bool is_going_left = false;
    bool is_going_right = false;

    this->computeIsGoingUpDownFlags(is_going_up, is_going_down);
    this->computeIsGoingLeftRightFlags(is_going_left, is_going_right);

    m_sprite->setAngle(0.0);
    if (m_is_floater)
    {
        this->cancelWalkingAnimation();
        this->cancelJumpingAnimation();
        if (m_options->CanEmitSound)
        {
            m_game->stopWalkingSound();
        }

        setOptionsForSprite(m_options->FloaterSprite);
        correctShape();
        this->disableGravity();
        this->setAngleForFloater();
        double new_velocity_x = 0;
        double new_velocity_y = 0;
        if (is_going_left)
        {
            m_sprite->setFlipX(true);
            new_velocity_x  = m_options->FloaterHorizontalVelocity * - 1;
            if (is_going_up)
            {
                new_velocity_y = m_options->FloaterVerticalVelocity;
                this->disableResting();
            }
            else
            {
                if (is_going_down)
                {
                    new_velocity_y = m_options->FloaterVerticalVelocity * -1;
                }
            }
        }
        else
        {
            m_sprite->setFlipX(false);
            if (is_going_right)
            {
                new_velocity_x  = m_options->FloaterHorizontalVelocity;
                if (is_going_up)
                {
                    new_velocity_y = m_options->FloaterVerticalVelocity;
                    this->disableResting();
                }
                else
                {
                    if (is_going_down)
                    {
                        new_velocity_y = m_options->FloaterVerticalVelocity * -1;
                    }
                }
            }
            else
            {
                if (is_going_up)
                {
                    new_velocity_y = m_options->FloaterVerticalVelocity;
                    this->disableResting();
                }
                else
                {
                    if (is_going_down)
                    {
                        new_velocity_y = m_options->FloaterVerticalVelocity * -1;
                    }
                }
            }
        }

        if (isAffectedByWind() && m_game)
        {
            new_velocity_x += m_game->windSpeed();
        }

        m_body->setCurrentTangentialVelocity(sad::p2d::Vector(new_velocity_x, new_velocity_y));
    }
    else
    {
        if (!m_is_resting)
        {
            this->enableGravity();
        }
        else
        {
            this->testResting();
            // Test vertical collision, since platform can go up
            if (m_is_resting)
            {
                double lower_bound = m_sprite->area()[0].y();
                double upper_bound = dynamic_cast<sad::p2d::Rectangle*>(m_resting_platform->currentShape())->rect()[2].y();                
                if ((lower_bound > upper_bound) && !sad::is_fuzzy_equal(lower_bound, upper_bound, RESTING_DETECTION_PRECISION))
                {
                    // Disable resting
                    this->disableResting();
                    this->enableGravity();
                }
            }
        }
        m_is_ducking = false;
        m_is_free_fall = false;
        if (is_going_up || !is_going_down) 
        {
            if (m_is_resting)
            { 
                if (is_going_up)
                { 
                    this->tryJump();
                }
                else
                {
                    setOptionsForSprite(m_options->StandingSprite);
                }
                this->correctShape();
            }
            else
            {
                if (is_going_up)
                { 
                    this->setVerticalVelocity(m_options->WalkerVerticalVelocity);
                }
                setOptionsForSprite(m_options->JumpingSprite);
                this->correctShape();
                if (m_options->CanEmitSound)
                {
                    m_game->playSound("jump");
                }
            }
        }
        else
        {
            this->setVerticalVelocity(0.0);
            this->startFallingOrDuck();
        }

        if (is_going_left)
        {
            startMovingLeft();
        }
        else
        {
            if (is_going_right)
            {
                startMovingRight();
            }
            else
            {
                setHorizontalVelocity(0.0);
            }
        }
    }

    if (no_sound)
    {
        m_options->CanEmitSound = can_emit_sound;
    }
}


bool game::Actor::isDying() const
{
    return m_is_dying;
}

void game::Actor::setDying(bool value)
{
    m_is_dying = value;
}

bool game::Actor::isFloater() const
{
    return m_is_floater;
}

void game::Actor::setFloaterState(bool is_floater)
{
    m_is_floater = is_floater;
    this->init();
}


bool game::Actor::canJump() const
{
    return isResting();
}

bool game::Actor::isResting() const
{
    return m_is_resting;
}


void game::Actor::setSprite(sad::Sprite2D* sprite)
{
    m_sprite = sprite;
}

void game::Actor::setBody(sad::p2d::Body* body)
{
    m_body = body;
}

void game::Actor::setHorizontalVelocity(double value)
{
    if (!m_options)
    {
        return;
    }
    sad::p2d::Body* b = m_body;
    sad::p2d::Vector v = b->tangentialVelocity();
    m_old_velocity = v;
    if (m_is_resting && !m_is_floater)
    {
        v.setX(m_resting_platform->tangentialVelocity().x());
    } 
    else
    {
        v.setX(0.0);
    }
    v.setX(v.x() + value);
    if (isAffectedByWind() && m_game)
    {
        v.setX(v.x() + m_game->windSpeed());
    }
    this->m_body->setCurrentTangentialVelocity(v);
    
    m_own_horizontal_velocity = value;
    if (m_is_resting && !m_is_ducking && !m_is_floater)
    {
        if (sad::is_fuzzy_zero(m_own_horizontal_velocity))
        {
            this->cancelWalkingAnimation();
            if (m_options->CanEmitSound)
            {
                m_game->stopWalkingSound();
            }
        }
        else
        {
            this->playWalkingAnimation();
            if (m_options->CanEmitSound)
            {
                m_game->playWalkingSound();
            }
        }
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void game::Actor::incrementVerticalVelocity(double value)
{
    sad::p2d::Vector v = m_body->tangentialVelocity();
    if (m_is_resting)
    {
        v.setX(m_own_horizontal_velocity);
        v.setY(0.0);
    }
    this->m_body->setCurrentTangentialVelocity(v + sad::p2d::Vector(0, value));
}

void game::Actor::enableGravity() const
{
    if (!m_options)
    {
        return;
    }
    if (this->m_is_floater)
    {
        Game::disableGravity(m_body);
    }
    else
    {
        Game::enableGravity(m_body);
    }
}

void game::Actor::disableGravity() const
{
    if (!m_options)
    {
        return;
    }
    Game::disableGravity(m_body);
}

void game::Actor::restOnPlatform(sad::p2d::Body* b, const  sad::p2d::Vector& old_velocity)
{
    if (!m_options)
    {
        return;
    }
    if (!b)
    {
        return;
    }
    this->cancelJumpingAnimation();
    this->disableGravity();
    bool already_resting = m_is_resting;
    m_is_resting = true;
    // Do not change any status flags if already resting
    if (!already_resting)
    { 
        m_is_free_fall = false;
        m_is_ducking = false;
    }
    m_resting_platform = b;

    sad::p2d::Vector own_velocity = old_velocity;
    if (m_is_floater)
    {
        own_velocity = this->computeVelocityForFloater();
    }
    else
    { 
        own_velocity.setX(own_velocity.x() + m_own_horizontal_velocity);
        if (isAffectedByWind() && m_game)
        {
            own_velocity.setX(own_velocity.x() + m_game->windSpeed());
        }
    }
    m_body->setCurrentTangentialVelocity(own_velocity);

    if (!m_is_floater)
    { 
        if (!sad::is_fuzzy_zero(m_own_horizontal_velocity))
        {
             if (!already_resting)
             { 
                 setOptionsForSprite(m_options->StandingSprite);
                if (m_options->CanEmitSound)
                {
                    m_game->playWalkingSound();
                }
             }
        } 
        else 
        {
            if (!already_resting)
            { 
                setOptionsForSprite(m_options->WalkingSprite);
                if (m_options->CanEmitSound)
                {
                    m_game->stopWalkingSound();
                }
            }
        }
    }
}

void game::Actor::disableResting()
{
    if (!m_options)
    {
        return;
    }
    this->enableGravity();
    m_is_resting = false;
    m_resting_platform = NULL;
    
    this->cancelWalkingAnimation();
    if (m_options->CanEmitSound && !m_is_floater)
    {
        m_game->stopWalkingSound();
    } 
    if (!m_is_floater)
    {       
        setOptionsForSprite(m_options->JumpingSprite);
    }
}


sad::Rect2D game::Actor::area() const
{
    return m_sprite->area();
}

sad::Point2D game::Actor::middle() const
{
    sad::Rect2D rect = this->area();
    return (rect[0] + rect[2]) / 2.0;
}

void game::Actor::move(const sad::Point2D& p) const
{
    if (m_body->willPositionChange())
    {
        m_body->shedulePosition(m_body->nextPosition() + p);
    }
    else
    {
        m_body->move(p);
    }
}

sad::p2d::Body* game::Actor::body() const
{
    return m_body;
}

sad::Sprite2D* game::Actor::sprite() const
{
    return m_sprite;
}

void game::Actor::testResting()
{
    if (!m_options)
    {
        return;
    }
    if (m_is_resting)
    {
        sad::Rect2D shape_1 = dynamic_cast<sad::p2d::Rectangle*>(m_body->currentShape())->rect();
        sad::Rect2D shape_2 = dynamic_cast<sad::p2d::Rectangle*>(m_resting_platform->currentShape())->rect();

        sad::p2d::Cutter1D player_part(std::min(shape_1[0].x(), shape_1[2].x()), std::max(shape_1[0].x(), shape_1[2].x()));
        sad::p2d::Cutter1D platform_part(std::min(shape_2[0].x(), shape_2[2].x()), std::max(shape_2[0].x(), shape_2[2].x()));
        if (!sad::p2d::collides(player_part, platform_part))
        {
            this->disableResting();
            // Restore speed for floater
            if (m_is_floater)
            {
                sad::p2d::Vector own_velocity = this->computeVelocityForFloater();
                m_body->setCurrentTangentialVelocity(own_velocity);
            }
        } 
        else
        {
            sad::p2d::Vector old_velocity =  m_resting_platform->tangentialVelocity();
            sad::p2d::Vector own_velocity = old_velocity;
            if (m_is_floater)
            {
                own_velocity = this->computeVelocityForFloater();
            }
            else
            {
                own_velocity.setX(own_velocity.x() + m_own_horizontal_velocity);
                if (isAffectedByWind() && m_game)
                {
                    own_velocity.setX(own_velocity.x() + m_game->windSpeed());
                }
            }
            m_body->setCurrentTangentialVelocity(own_velocity);
        }

    }
}
void game::Actor::startMovingLeft()
{
    if (!m_options)
    {
        return;
    }
    double v = ((m_is_floater) ? m_options->FloaterHorizontalVelocity : m_options->WalkerHorizontalVelocity);
    this->startMoving(true, v * -1);
}

void game::Actor::startMovingRight()
{
    if (!m_options)
    {
        return;
    }
    double v = ((m_is_floater) ? m_options->FloaterHorizontalVelocity : m_options->WalkerHorizontalVelocity);
    this->startMoving(false, v);
}

void game::Actor::stopMovingHorizontally()
{
    if (!m_options)
    {
        return;
    }
    if (!m_is_floater)
    {
        if (m_is_resting)
        {
            if (!m_is_ducking)
            {
                setOptionsForSprite(m_options->StandingSprite);
            }
        }
        else
        {
            if (!m_is_free_fall)
            {
                setOptionsForSprite(m_options->JumpingSprite);
            }
        }
    }
    this->setHorizontalVelocity(0);
}

void game::Actor::tryJump()
{
    if (!m_options)
    {
        return;
    }
    if (this->canJump()) 
    {
        this->stopFallingOrStopDucking();
        this->incrementVerticalVelocity(m_options->WalkerVerticalVelocity);
        this->disableResting();
        this->playJumpingAnimation();
        if (m_options->CanEmitSound)
        {
            m_game->playSound("jump");
        }
    }
}

void game::Actor::startFallingOrDuck()
{
    if (!m_options)
    {
        return;
    }
    if (!m_is_resting) {
        m_is_free_fall = true;
        this->cancelJumpingAnimation();
        this->incrementVerticalVelocity(m_options->WalkerVerticalVelocity * -1);
        setOptionsForSprite(m_options->FallingSprite);
        this->correctShape();
    } else {
        this->duck();
    }
}

void game::Actor::stopFallingOrStopDucking()
{
    if (!m_options)
    {
        return;
    }
    if (!m_is_resting) {
        m_is_free_fall = false;
        this->cancelJumpingAnimation();
        this->incrementVerticalVelocity(m_options->WalkerVerticalVelocity);
        setOptionsForSprite(m_options->JumpingSprite);
        this->correctShape();
    } else {
        this->stopDucking();
    }
}

void game::Actor::duck()
{
    if (!m_options)
    {
        return;
    }
    if (!m_is_ducking && m_is_resting)
    {
        m_is_ducking = true;
        this->cancelWalkingAnimation();
        if (m_options->CanEmitSound)
        {
            m_game->stopWalkingSound();
        }
        setOptionsForSprite(m_options->DuckingSprite);
        correctShape();
    }
}

void game::Actor::stopDucking()
{ 
    if (m_is_ducking && m_is_resting)
    {
        m_is_ducking = false;
        bool is_going_left = false, is_going_right = false;
        if (is_going_left || is_going_right)
        {
            setOptionsForSprite(m_options->WalkingSprite);
        }
        else
        {
            setOptionsForSprite(m_options->StandingSprite);
        }
        this->correctShape();

        if (!sad::is_fuzzy_zero(m_own_horizontal_velocity))
        {
            this->playWalkingAnimation();
        }
        correctShape();
    }
}

bool game::Actor::isDucking() const
{
    return m_is_ducking;
}

bool game::Actor::isFreefalling() const
{
    return m_is_free_fall;
}

const sad::p2d::Vector& game::Actor::oldVelocity() const
{
    return m_old_velocity;
}

void game::Actor::moveBy(const sad::p2d::Vector& v) const
{
    m_sprite->moveBy(v);
}

bool game::Actor::canBeRotated()
{
    return true;
}

void game::Actor::rotate(double angle) const
{
    m_sprite->rotate(angle);
}

sad::p2d::Body* game::Actor::restingPlatform() const
{
    return m_resting_platform;
}

bool game::Actor::isLastMovedLeft() const
{
    return m_is_last_moved_left;
}

void game::Actor::toggleInvincibility(bool on)
{
    m_is_invincible = on;
}

bool game::Actor::isInvincible() const
{
    return m_is_invincible;
}

int game::Actor::lives() const
{
    return m_lives;
}

void game::Actor::setLives(int lives)
{
    m_lives = lives;
    if (m_lives <= 0)
    {
        this->fireOnDeathEvents();
    }
}

void game::Actor::incrementLives(int lives)
{
    m_lives += lives;
}

void game::Actor::decrementLives(int lives)
{
    m_lives -= lives;
    if (m_lives <= 0)
    {
        this->fireOnDeathEvents();
    }
}

void game::Actor::tryDecrementLives(int lives)
{
    if (!this->isInvincible())
    {
        m_lives -= lives;
        if (m_lives <= 0)
        {
            this->fireOnDeathEvents();
        }
        else
        {
            this->toggleInvincibility(true);
            sad::animations::Instance* instance = new sad::animations::Instance();
            instance->setAnimation(m_hurt_animation);
            instance->setObject(m_sprite);
            instance->end([=]() { this->toggleInvincibility(false); });

            m_game->rendererForMainThread()->animations()->add(instance);
        }
    }
}


void game::Actor::onDeath(const std::function<void(game::Actor*)>& action)
{
    m_on_death_actions.clear();
    m_on_death_actions.push_back(action);
}

void game::Actor::onBeforeDeath(const std::function<void(game::Actor*)>& action)
{
    m_on_death_actions.insert(m_on_death_actions.begin(), action);
}

void game::Actor::setHurtAnimation(sad::animations::Animation* animation)
{
    m_hurt_animation = animation;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
int game::Actor::modifyDamage(int base_dmg)
{
    return base_dmg + m_attack_modifier;
}

double game::Actor::lookupAngle() const
{
    return m_lookup_angle;
}

void game::Actor::setLookupAngle(double angle)
{
    m_lookup_angle = angle;
}

void game::Actor::pushWeapon(weapons::Weapon* w)
{
    if (w)
    {
        sad::Vector<weapons::Weapon*>::iterator it = std::find(m_weapons.begin(), m_weapons.end(), w);
        if (it == m_weapons.end())
        {
            w->addRef();
            m_weapons.push_back(w);
        }
        else
        {
            m_weapons.erase(it);
            m_weapons.push_back(w);
        }
    }
}

void game::Actor::removeWeapon(weapons::Weapon* w)
{
    if (!m_weapons.empty())
    { 
        for(size_t i = 0; i < m_weapons.size(); ++i)
        {
            if (m_weapons[i] == w)
            {
                if (w)
                {
                    w->delRef();
                }
                m_weapons.removeAt(i);
                return;
            }
        }
    }
}

void game::Actor::removeWeaponWithItem(weapons::Weapon* w, Game* g)
{
    if (this == g->player()->actor())
    {
        g->removeItemFromPlayersInventoryWithWeapon(w);
    }
}

weapons::Weapon* game::Actor::weapon() const
{
    if (!m_weapons.empty())
    {
        return m_weapons[m_weapons.size() - 1];
    }
    return NULL;
}

void game::Actor::tryShoot()
{
    weapons::Weapon* weapon = this->weapon();
    if (weapon)
    {
        weapon->tryShoot(this->game(), this);
    }
}

sad::Point2D game::Actor::pointForProjectileSpawn(double angle) const
{
    while (angle < 0)
    {
        angle += 2 * M_PI;
    }

    while (angle > 2 * M_PI)
    {
        angle -= 2 * M_PI;
    }

    sad::Point2D middle = this->middle();
    sad::p2d::InfiniteLine line = sad::p2d::InfiniteLine::appliedVector(middle, sad::p2d::Vector(cos(angle), sin(angle)));
    double rect_angle = 0;
    if (!sad::is_fuzzy_zero(this->area().width()))
    {
        rect_angle = atan2(this->area().height(), this->area().width());
    }
    sad::p2d::MaybePoint pivot_point;
    if ((angle <= rect_angle) || (angle >= (2 * M_PI - rect_angle)))
    {
        pivot_point = line.intersection(sad::p2d::Cutter2D(this->area()[1], this->area()[2]));
    }
    else
    {
        if (angle <= (M_PI - rect_angle))
        {
            pivot_point = line.intersection(sad::p2d::Cutter2D(this->area()[2], this->area()[3]));
        }
        else
        {
            if (angle < M_PI + rect_angle)
            {
                pivot_point = line.intersection(sad::p2d::Cutter2D(this->area()[0], this->area()[3]));
            }
            else
            {
                pivot_point = line.intersection(sad::p2d::Cutter2D(this->area()[0], this->area()[1]));
            }
        }
    }
    sad::Point2D result_middle;
    if (!pivot_point.exists())
    {
        result_middle = middle;
    }
    else
    {
        result_middle = pivot_point.value();
    }
    return result_middle;
}

sad::Scene* game::Actor::scene() const
{
    return m_sprite->scene();
}

sad::animations::Instance* game::Actor::playDeathAnimation()
{
    sad::Point2D middle = m_sprite->middle();
    sad::Sprite2D* sprite = m_sprite;

    sad::animations::SimpleMovement* movement = new sad::animations::SimpleMovement();
    movement->setStartingPoint(middle);
    movement->setEndingPoint(sad::Point2D(middle.x(), -(m_sprite->area().height())));
    movement->setTime(1000);
    movement->setLooped(false);

    sad::animations::Rotate* rotate = new sad::animations::Rotate();
    rotate->setMinAngle(0);
    rotate->setMaxAngle(4 * M_PI);
    rotate->setTime(1000);
    rotate->setLooped(false);

    sad::animations::Parallel* parallel = new sad::animations::Parallel();
    parallel->add(movement);
    parallel->add(rotate);
    parallel->setTime(1000);
    parallel->setLooped(false);

    sad::animations::Instance* instance = new sad::animations::Instance();
    instance->setObject(m_sprite);
    instance->setAnimation(parallel);
    instance->end([=] { sprite->scene()->removeNode(sprite); });

    m_game->rendererForMainThread()->animations()->add(instance);

    return instance;
}

void game::Actor::die()
{
    this->addRef();
    m_game->killActorWithoutSprite(this);
    this->playDeathAnimation();
    this->delRef();
}

void game::Actor::setShootingStrategy(bots::shootingstrategies::ShootingStrategy* s)
{
    if (m_game)
    {
        m_game->setStrategyForBot(this, s);
    }
}

void game::Actor::toggleFixedSize(bool fixed_size)
{
    m_fixed_size  = fixed_size;
}

bool game::Actor::fixedSize() const
{
    return m_fixed_size;
}

void game::Actor::incrementAttackModifier(int attack_delta)
{
    m_attack_modifier += attack_delta;
}

void game::Actor::decrementAttackModifier(int attack_delta)
{
    m_attack_modifier -= attack_delta;
}

void game::Actor::incrementDefense(int delta)
{
    m_defense += delta;
}

void game::Actor::decrementDefense(int delta)
{
    m_defense -= delta;
}

int game::Actor::defense() const
{
    return m_defense;
}

void game::Actor::takeDamage(int base_dmg)
{
    int dmg = base_dmg - this->defense();
    if (dmg > 0)
    {
        this->tryDecrementLives(dmg);
    }
}

void game::Actor::incrementFloaterStateCounter()
{
    ++m_floater_state_counter;
    if (!this->isFloater()) 
    {
        if (m_floater_state_counter > 0)
        {
            this->setFloaterState(true);
        }
    }
}

void game::Actor::decrementFloaterStateCounter()
{
    --m_floater_state_counter;
    if (this->isFloater())
    { 
        if (m_floater_state_counter <= 0)
        {
            this->setFloaterState(false);
        }
    }
}

void game::Actor::toggleIsAffectedByWind(bool affected)
{
    m_affected_by_wind = affected;
}

bool game::Actor::isAffectedByWind() const
{
    return m_affected_by_wind;
}

void game::Actor::updateHorizontalVelocity()
{
    // Force recompute for horizontal velocity for actor
    setHorizontalVelocity(m_own_horizontal_velocity);
}

void game::Actor::fireOnDeathEvents()
{
    for (auto& m_on_death_action : m_on_death_actions)
    {
        m_on_death_action(this);
    }
}

void game::Actor::pauseWeaponsReloading()
{
    for (size_t i = 0; i < m_weapons.size(); i++)
    {
        if (m_weapons[i])
        { 
            m_weapons[i]->pause();
        }
    }
}

void game::Actor::resumeWeaponsReloading()
{
    for (size_t i = 0; i < m_weapons.size(); i++)
    {
        if (m_weapons[i])
        {
            m_weapons[i]->resume();
        }
    }
}

// ===================================== PRIVATE METHODS =====================================

void game::Actor::computeIsGoingUpDownFlags(bool& is_going_up, bool& is_going_down)
{
    is_going_up = false;
    is_going_down = false;
    if (m_key_states[game::Actor::ABTN_UP] && m_key_states[game::Actor::ABTN_DOWN])
    {
        if (m_key_states[LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_UP])
        {
            is_going_up = true;
        }
        else
        {
            is_going_down = true;
        }
    }
    else
    {
        if (m_key_states[game::Actor::ABTN_UP]) is_going_up = true;
        if (m_key_states[game::Actor::ABTN_DOWN]) is_going_down = true;
    }
}

void game::Actor::computeIsGoingLeftRightFlags(bool& is_going_left, bool& is_going_right)
{
    is_going_left = false;
    is_going_right = false;
    if (m_key_states[game::Actor::ABTN_LEFT] && m_key_states[game::Actor::ABTN_RIGHT])
    {
        if (m_key_states[LAST_KEY_BITSET_OFFSET + game::Actor::ABTN_LEFT])
        {
            is_going_left = true;
        }
        else
        {
            is_going_right = true;
        }
    }
    else
    {
        if (m_key_states[game::Actor::ABTN_LEFT]) is_going_left = true;
        if (m_key_states[game::Actor::ABTN_RIGHT]) is_going_right = true;
    }
}

void game::Actor::setAngleForFloater()
{
    bool is_going_up = false;
    bool is_going_down = false;
    bool is_going_left = false;
    bool is_going_right = false;

    this->computeIsGoingUpDownFlags(is_going_up, is_going_down);
    this->computeIsGoingLeftRightFlags(is_going_left, is_going_right);

    double angle = 0;

    if (is_going_left)
    {
        m_sprite->setFlipX(true);
        if (is_going_up)
        {
            angle = M_PI / -4.0;
        }
        else
        {
            if (is_going_down)
            {
                angle = M_PI / 4.0;
            }
        }
    }
    else
    {
        m_sprite->setFlipX(false);
        if (is_going_right)
        {
            if (is_going_up)
            {
                angle = M_PI / 4.0;
            }
            else
            {
                if (is_going_down)
                {
                    angle = M_PI / -4.0;
                }
            }
        }
        else
        {
            if (is_going_up)
            {
                angle = M_PI / 2.0;
            }
            else
            {
                if (is_going_down)
                {
                    angle = M_PI / -2.0;
                }
            }
        }
    }
    m_sprite->setAngle(angle);
}

sad::p2d::Vector game::Actor::computeVelocityForFloater()
{
    bool is_going_up = false;
    bool is_going_down = false;
    bool is_going_left = false;
    bool is_going_right = false;

    this->computeIsGoingUpDownFlags(is_going_up, is_going_down);
    this->computeIsGoingLeftRightFlags(is_going_left, is_going_right);

    sad::p2d::Vector result(0.0, 0.0);
    if (!m_is_floater)
    {
        throw std::logic_error("Must be floater");
    }
    if (!m_options)
    {
        throw std::logic_error("Must have options");
    }

    if (is_going_left)
    {
        result.setX(-1 * m_options->FloaterHorizontalVelocity);
    }
    else
    {
        if (is_going_right)
        {
            result.setX(m_options->FloaterHorizontalVelocity);
        }
    }

    if (isAffectedByWind() && m_game)
    {
       result.setX(result.x()  + m_game->windSpeed());
    }

    if (is_going_up)
    {
        result.setY(m_options->FloaterVerticalVelocity);
    }
    else
    {
        if (is_going_down)
        {
            result.setY(-1 * m_options->FloaterVerticalVelocity);
        }
    }

    return result;
}

void game::Actor::setVerticalVelocity(double v) const
{
    if (!m_options)
    {
        return;
    }
    sad::p2d::Body* b = m_body;
    sad::p2d::Vector velocity = b->tangentialVelocity();
    velocity.setY(v);
    b->setCurrentTangentialVelocity(velocity);
}

sad::animations::Animations* game::Actor::animations() const
{
    return this->m_sprite->scene()->renderer()->animations();
}

void game::Actor::playWalkingAnimation()
{
    if (!m_options)
    {
        return;
    }

    if (!m_is_walking_animation_playing)
    {
        m_walking_instance->enableStateRestoringOnFinish();
        m_walking_instance->clearFinished();
        m_walking_instance->setObject(m_sprite); 
        this->animations()->add(m_walking_instance);
        m_is_walking_animation_playing = true;
    }
}

void game::Actor::cancelWalkingAnimation()
{
    if (!m_options)
    {
        return;
    }

    if (m_is_walking_animation_playing)
    {
        m_walking_instance->disableStateRestoringOnFinish();
        m_walking_instance->cancel(this->animations());
        this->animations()->remove(m_walking_instance);
        m_is_walking_animation_playing = false;
    }
}

void game::Actor::playJumpingAnimation()
{
    if (!m_options)
    {
        return;
    }
    if (!m_is_jumping_animation_playing)
    {
        m_jumping_instance->clearFinished();
        m_jumping_instance->enableStateRestoringOnFinish();
        m_jumping_instance->setObject(m_sprite); 
        this->animations()->add(m_jumping_instance);
        m_is_jumping_animation_playing = true;
    }
}

void game::Actor::cancelJumpingAnimation()
{
    if (m_is_jumping_animation_playing)
    {
        m_jumping_instance->disableStateRestoringOnFinish();
        m_jumping_instance->cancel(this->animations());
        this->animations()->remove(m_jumping_instance);
        m_is_jumping_animation_playing = false;
    }
}

void game::Actor::startMoving(bool flip_flag, double velocity)
{
    if (!m_options)
    {
        return;
    }
    m_sprite->setFlipX(flip_flag);
    if (!m_is_floater)
    { 
        if (m_is_resting)
        {
            if (!m_is_ducking)
            {
                if (!m_is_walking_animation_playing)
                {
                    setOptionsForSprite(m_options->WalkingSprite);
                }
            }
        }
    }
    this->setHorizontalVelocity(velocity);
}

void game::Actor::correctShape() const
{
    sad::p2d::Rectangle*  shape = dynamic_cast<sad::p2d::Rectangle*>(m_body->currentShape());
    sad::Point2D start_point = shape->rect()[0];
    sad::Point2D width_height(this->m_sprite->area().width(), this->m_sprite->area().height());
    sad::Rect2D corrected_rect(start_point, start_point + width_height);
    this->m_sprite->setArea(corrected_rect);
    shape->setRect(corrected_rect);
}

void game::Actor::setOptionsForSprite(const sad::String& o) const
{
    sad::Rect2D rect = m_sprite->area();
    m_sprite->set(o);
    if (m_fixed_size)
    {
        m_sprite->setArea(rect);
    }
}

void game::exposeActor(void* c)
{
    sad::dukpp03::Context* ctx = reinterpret_cast<sad::dukpp03::Context*>(c);

    sad::dukpp03::ClassBinding* actor_binding = new sad::dukpp03::ClassBinding();

    actor_binding->addMethod("isResting", sad::dukpp03::bind_method::from(&game::Actor::isResting));
    actor_binding->addMethod("area", sad::dukpp03::bind_method::from(&game::Actor::area));
    actor_binding->addMethod("middle", sad::dukpp03::bind_method::from(&game::Actor::middle));
    actor_binding->addMethod("sprite", sad::dukpp03::bind_method::from(&game::Actor::sprite));

    actor_binding->addMethod("lives", sad::dukpp03::bind_method::from(&game::Actor::lives));
    actor_binding->addMethod("setLives", sad::dukpp03::bind_method::from(&game::Actor::setLives));
    actor_binding->addMethod("incrementLives", sad::dukpp03::bind_method::from(&game::Actor::incrementLives));
    actor_binding->addMethod("decrementLives", sad::dukpp03::bind_method::from(&game::Actor::decrementLives));
    actor_binding->addMethod("toggleInvincibility", sad::dukpp03::bind_method::from(&game::Actor::toggleInvincibility));
    actor_binding->addMethod("isInvincible", sad::dukpp03::bind_method::from(&game::Actor::isInvincible));

    actor_binding->addMethod("isFloater", sad::dukpp03::bind_method::from(&game::Actor::isFloater));
    actor_binding->addMethod("setFloaterState", sad::dukpp03::bind_method::from(&game::Actor::setFloaterState));

    actor_binding->addMethod("setVelocity", sad::dukpp03::bind_method::from(&game::Actor::setVelocity));

    actor_binding->addMethod("tryStartGoingUp", sad::dukpp03::bind_method::from(&game::Actor::tryStartGoingUp));
    actor_binding->addMethod("tryStartGoingDown", sad::dukpp03::bind_method::from(&game::Actor::tryStartGoingDown));
    actor_binding->addMethod("tryStartGoingLeft", sad::dukpp03::bind_method::from(&game::Actor::tryStartGoingLeft));
    actor_binding->addMethod("tryStartGoingRight", sad::dukpp03::bind_method::from(&game::Actor::tryStartGoingRight));

    actor_binding->addMethod("tryStopGoingUp", sad::dukpp03::bind_method::from(&game::Actor::tryStopGoingUp));
    actor_binding->addMethod("tryStopGoingDown", sad::dukpp03::bind_method::from(&game::Actor::tryStopGoingDown));
    actor_binding->addMethod("tryStopGoingLeft", sad::dukpp03::bind_method::from(&game::Actor::tryStopGoingLeft));
    actor_binding->addMethod("tryStopGoingRight", sad::dukpp03::bind_method::from(&game::Actor::tryStopGoingRight));
    actor_binding->addMethod("lookupAngle", sad::dukpp03::bind_method::from(&game::Actor::lookupAngle));
    actor_binding->addMethod("setLookupAngle", sad::dukpp03::bind_method::from(&game::Actor::setLookupAngle));

    actor_binding->addMethod("pushWeapon", sad::dukpp03::bind_method::from(&game::Actor::pushWeapon));
    actor_binding->addMethod("removeWeapon", sad::dukpp03::bind_method::from(&game::Actor::removeWeapon));
    actor_binding->addMethod("weapon", sad::dukpp03::bind_method::from(&game::Actor::weapon));
    actor_binding->addMethod("tryShoot", sad::dukpp03::bind_method::from(&game::Actor::tryShoot));

    actor_binding->addMethod("setShootingStrategy", sad::dukpp03::bind_method::from(&game::Actor::setShootingStrategy));
    actor_binding->addMethod("incrementAttackModifier", sad::dukpp03::bind_method::from(&game::Actor::incrementAttackModifier));
    actor_binding->addMethod("decrementAttackModifier", sad::dukpp03::bind_method::from(&game::Actor::decrementAttackModifier));
    actor_binding->addMethod("incrementDefense", sad::dukpp03::bind_method::from(&game::Actor::incrementDefense));
    actor_binding->addMethod("decrementDefense", sad::dukpp03::bind_method::from(&game::Actor::decrementDefense));
    actor_binding->addMethod("defense", sad::dukpp03::bind_method::from(&game::Actor::defense));
    actor_binding->addMethod("incrementFloaterStateCounter", sad::dukpp03::bind_method::from(&game::Actor::incrementFloaterStateCounter));
    actor_binding->addMethod("decrementFloaterStateCounter", sad::dukpp03::bind_method::from(&game::Actor::decrementFloaterStateCounter));

    ctx->addClassBinding("game::Actor", actor_binding);
}
