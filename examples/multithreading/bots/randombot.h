/*! \file randombot.h
 
    A random jumping bot for jumping
 */
#pragma once
#include "abstractbot.h"

namespace bots
{

/*! A bot, which randomly jumps and walks everywhere
 */
class RandomBot : public bots::AbstractBot
{
public:
    /*! Constructs a bot maximal right offset
        \param[in] min_left_offset a minimal left offset
        \param[in] max_right_offset an offset
     */
    RandomBot(double min_left_offset, double max_right_offset);
    /*! Frees all data
     */
    virtual ~RandomBot() override;
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
    /*! A minimal left offset
     */
    double m_min_left_offset;
    /*! A real left offset
     */
    double m_real_left_offset;
    /*! A max right offset
     */
    double m_max_right_offset;
    /*! A max offset to move
     */
    double m_real_right_offset;
    /*! Whether we should walk left
     */
    bool m_left;
};

}
