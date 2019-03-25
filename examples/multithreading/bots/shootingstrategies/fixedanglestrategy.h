/*! \file fixedanglestrategy.h
 *   
 *   A strategy, that consists of enemy shooting with fixed angle
 */
#pragma once
#include "shootingstrategy.h"

namespace bots
{

namespace shootingstrategies
{

/*! A strategy, that forces actor to shoot in same direction (angle)
 */
class FixedAngleStrategy: public bots::shootingstrategies::ShootingStrategy
{
SAD_OBJECT
public:
    /*! Constructs default strategy
     */
    FixedAngleStrategy();
    /*! Constructs default strategy with fixed angle
     *  \param[in] angle an angle
     */
    FixedAngleStrategy(double angle);
    /*! Could be inherited
     */
    virtual ~FixedAngleStrategy();
    /*! Sets an angle
     *  \param[in] angle angle for strategy
     */
    void setAngle(double angle);
    /*! Returns an angle
     *  \return angle
     */
    double angle() const;
protected:
    /*! Performs actual shooting for actor
     */
    virtual void shoot(game::Actor* actor);
    /*! An angle
     */
    double m_angle;
};

/*! Expose shooting strategy
 *  \param[in] c context
 */
void exposeFixedAngleStrategy(void* c);

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(bots::shootingstrategies::FixedAngleStrategy)
