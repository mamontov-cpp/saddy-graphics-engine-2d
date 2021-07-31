/*! \file randomstrategy.h
 *
 *   A strategy, that consists of random shooting
 */
#pragma once
#include "shootingstrategy.h"

namespace bots
{

namespace shootingstrategies
{

/*! A strategy, that forces actor to shoot randomly
 */
class RandomStrategy : public bots::shootingstrategies::ShootingStrategy
{
SAD_OBJECT
public:
    /*! Constructs default strategy
     */
    RandomStrategy();
    /*! Could be inherited
     */
    virtual ~RandomStrategy();
protected:
    /*! Performs actual shooting for actor
     */
    virtual void shoot(game::Actor* actor) override;
};

/*! Expose player's random strategy
 *  \param[in] c context
 */
void exposeRandomStrategy(void* c);

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(bots::shootingstrategies::RandomStrategy)
