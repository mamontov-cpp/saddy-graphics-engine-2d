/*! \file directionbot.h

    A bot, which goes into one direction
 */
#pragma once
#include "abstractbot.h"

namespace bots
{

/*! A bot, that does nothing
 */
class DirectionBot : public bots::AbstractBot
{
public:
/*! A horizontal direction
 */
enum class HDir: int
{
    DBHD_RIGHT = 0,
    DBHD_LEFT = 1,
    DBHD_NONE = 2
};
/*! A vertical direction
 */
enum class VDir: int
{
    DBVD_UP = 0,
    DBVD_DOWN = 1,
    DBVD_NONE = 2
};
    /*! A new bot
        \param[in] horizontal_direction a horizontal direction (value from bots::DirectionBot::HDir enum)
        \param[in] vertical_direction a vertical direction (value from bots::DirectionBot::VDir enum)
     */
    DirectionBot(int horizontal_direction, int vertical_direction);
    /*! Frees all data
     */
    virtual ~DirectionBot() override;
    /*! Copies bot's state and returns a new copy
       \return clone of current bot
     */
    virtual bots::AbstractBot* clone() const override;
    /*! Performs actors actions on each bot
        \param[in] game a game
        \param[in] actor an actor to be called
     */
    virtual void perform(Game* game, game::Actor* actor) override;
private:
    /*! A horizontal direction
     */
    bots::DirectionBot::HDir m_horizontal_direction;
    /*! A vertical direction
     */
    bots::DirectionBot::VDir m_vertical_direction;
};

}
