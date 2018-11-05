/*! \file playerlocatingstrategy.h
 *
 *   A strategy, that consists of shooting to player's direction
 */
#pragma once
#include "shootingstrategy.h"

namespace bots
{

namespace shootingstrategies
{

/*! A strategy, that forces actor to shoot to player's direction
 */
class PlayerLocatingStrategy : public bots::shootingstrategies::ShootingStrategy
{
SAD_OBJECT
public:
    /*! Constructs default strategy
     */
    PlayerLocatingStrategy();
    /*! Could be inherited
     */
    virtual ~PlayerLocatingStrategy();
protected:
    /*! Performs actual shooting for actor
     */
    virtual void shoot(game::Actor* actor);
};

/*! Expose player location strategy
 *  \param[in] c context
 */
void exposePlayerLocationStrategy(void* c);

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(bots::shootingstrategies::PlayerLocatingStrategy)
