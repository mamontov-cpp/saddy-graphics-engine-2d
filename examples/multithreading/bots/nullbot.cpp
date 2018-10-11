#include "nullbot.h"

bots::NullBot::NullBot()
{
    
}

bots::NullBot::~NullBot()
{

}

bots::AbstractBot* bots::NullBot::clone() const
{
    return new bots::NullBot();
}

void bots::NullBot::perform(Game*, game::Actor*)
{
    
}

