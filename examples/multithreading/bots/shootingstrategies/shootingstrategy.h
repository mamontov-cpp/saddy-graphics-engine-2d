/*! \file shootingstrategy.h
 *
 *  Defines shooting strategy for a bot's actor
 */
#pragma once
#include <object.h>
#include <pausabletimer.h>

namespace game
{

class Actor;

}

namespace bots
{ 

namespace shootingstrategies
{

/*! Defines a simple abstract shooting strategy
 */
class ShootingStrategy: public sad::Object
{
SAD_OBJECT
public:
    /*! Default shooting strategy
     */
    ShootingStrategy();
    /*! Must be inherited
     */
    virtual ~ShootingStrategy();
    /*! Pauses working shooting strategy
     */
    void pause();
    /*! Resumes working shooting strategy
     */
    void resume();
    /*! Tries to perform shooting actor
     *  \param[in] actor an actor
     */
    virtual void tryShoot(game::Actor* actor);
    /*! Sets interval for strategy
     *  \param[in] interval an interval
     */
    void setInterval(double interval);
    /*!  Returns interval of shooting for strategy
     *   \return interval
     */
    double interval() const;
protected:
    /*! Performs actual shooting for actor
     *  \param[in] actor an actor
     */
    virtual void shoot(game::Actor* actor) = 0;
private:
    /*! Interval between bullets
     */
    double m_interval;
    /*! A local timer
     */
    sad::PausableTimer m_timer;
};

/*! Expose shooting strategy
 *  \param[in] c context
 */
void exposeShootingStrategy(void* c);

}

}



DECLARE_TYPE_AS_SAD_OBJECT_ENUM(bots::shootingstrategies::ShootingStrategy)