#include "followplayerfloater.h"

#include "../game/actor.h"
#include "../game/player.h"
#include "../game.h"

bots::FollowPlayerFloater::FollowPlayerFloater()
{

}

bots::FollowPlayerFloater::~FollowPlayerFloater()
{

}


bots::AbstractBot* bots::FollowPlayerFloater::clone() const
{
    return new bots::FollowPlayerFloater();
}

void bots::FollowPlayerFloater::perform(Game* game, game::Actor* actor)
{
    sad::Point2D pmiddle = game->player()->actor()->middle();
    sad::Point2D middle = actor->middle();
    if (middle.x() > pmiddle.x())
    {
        actor->tryStopGoingRight();
        actor->tryStartGoingLeft();
    }
    else
    {
        actor->tryStopGoingLeft();
        actor->tryStartGoingRight();
    }

    if (middle.y() > pmiddle.y())
    {
        actor->tryStopGoingUp();
        actor->tryStartGoingDown();
    }
    else
    {
        actor->tryStopGoingDown();
        actor->tryStartGoingUp();
    }
}
