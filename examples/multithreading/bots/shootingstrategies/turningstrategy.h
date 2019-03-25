/*! \file turningstrategy.h
 *
 *   A strategy, that consists of enemy turning with fixed speed and shooting
 */
#pragma once
#include "shootingstrategy.h"

namespace bots
{

namespace shootingstrategies
{

/*! A strategy, that consists of enemy turning with fixed speed and shooting
 */
class TurningStrategy : public bots::shootingstrategies::ShootingStrategy
{
SAD_OBJECT
public:
    /*! Constructs default strategy
     */
    TurningStrategy();
    /*! Constructs default strategy with full time
     *  \param[in] full_turn_time a full time
     */
    TurningStrategy(double full_turn_time);
    /*! Could be inherited
     */
    virtual ~TurningStrategy();
    /*! Sets a time
     *  \param[in] time a time
     */
    void setFullTurnTime(double time);
    /*! Returns a time
     *  \return time
     */
    double fullTurnTime() const;
    /*! Tries to perform shooting actor
     *  \param[in] actor an actor
     */
    virtual void tryShoot(game::Actor* actor);
protected:
    /*! Performs actual shooting for actor
     *  \param[in] actor an actor
     */
    virtual void shoot(game::Actor* actor);
    /*! A time for full turn time
     */
    double m_full_turn_time;
};

/*! Expose turning strategy
 *  \param[in] c context
 */
void exposeTurningStrategy(void* c);

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(bots::shootingstrategies::TurningStrategy)