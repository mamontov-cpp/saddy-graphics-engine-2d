#include "actoroptions.h"

#include <dukpp-03/context.h>


DECLARE_COMMON_TYPE(game::ActorOptions)


void game::exposeActorOptions(void* c)
{
    sad::dukpp03::Context* ctx = reinterpret_cast<sad::dukpp03::Context*>(c);

    sad::dukpp03::ClassBinding* binding = new sad::dukpp03::ClassBinding();
    binding->addConstructor("ActorOptions", new ::dukpp03::Constructor0<sad::dukpp03::BasicContext, game::ActorOptions>());
    binding->addAccessor(
        "IsFloater",
         sad::dukpp03::getter::from(&game::ActorOptions::IsFloater),
         sad::dukpp03::setter::from(&game::ActorOptions::IsFloater)
    );
    binding->addAccessor(
        "CanEmitSound",
         sad::dukpp03::getter::from(&game::ActorOptions::CanEmitSound),
         sad::dukpp03::setter::from(&game::ActorOptions::CanEmitSound)
    );
    binding->addAccessor(
        "WalkerHorizontalVelocity",
         sad::dukpp03::getter::from(&game::ActorOptions::WalkerHorizontalVelocity),
         sad::dukpp03::setter::from(&game::ActorOptions::WalkerHorizontalVelocity)
    );
    binding->addAccessor(
        "WalkerVerticalVelocity",
         sad::dukpp03::getter::from(&game::ActorOptions::WalkerVerticalVelocity),
         sad::dukpp03::setter::from(&game::ActorOptions::WalkerVerticalVelocity)
    );
    binding->addAccessor(
        "FloaterHorizontalVelocity",
         sad::dukpp03::getter::from(&game::ActorOptions::FloaterHorizontalVelocity),
         sad::dukpp03::setter::from(&game::ActorOptions::FloaterHorizontalVelocity)
    );
    binding->addAccessor(
        "FloaterVerticalVelocity",
         sad::dukpp03::getter::from(&game::ActorOptions::FloaterVerticalVelocity),
         sad::dukpp03::setter::from(&game::ActorOptions::FloaterVerticalVelocity)
    );
    binding->addAccessor(
        "WalkingAnimationOptions",
         sad::dukpp03::getter::from(&game::ActorOptions::WalkingAnimationOptions),
         sad::dukpp03::setter::from(&game::ActorOptions::WalkingAnimationOptions)
    );
    binding->addAccessor(
        "WalkingAnimationTime",
         sad::dukpp03::getter::from(&game::ActorOptions::WalkingAnimationTime),
         sad::dukpp03::setter::from(&game::ActorOptions::WalkingAnimationTime)
    );
    binding->addAccessor(
        "JumpingAnimationOptions",
         sad::dukpp03::getter::from(&game::ActorOptions::JumpingAnimationOptions),
         sad::dukpp03::setter::from(&game::ActorOptions::JumpingAnimationOptions)
    );
    binding->addAccessor(
        "JumpingAnimationTime",
         sad::dukpp03::getter::from(&game::ActorOptions::JumpingAnimationTime),
         sad::dukpp03::setter::from(&game::ActorOptions::JumpingAnimationTime)
    );
    binding->addAccessor(
        "StandingSprite",
         sad::dukpp03::getter::from(&game::ActorOptions::StandingSprite),
         sad::dukpp03::setter::from(&game::ActorOptions::StandingSprite)
    );
    binding->addAccessor(
        "WalkingSprite",
         sad::dukpp03::getter::from(&game::ActorOptions::WalkingSprite),
         sad::dukpp03::setter::from(&game::ActorOptions::WalkingSprite)
    );
    binding->addAccessor(
        "JumpingSprite",
         sad::dukpp03::getter::from(&game::ActorOptions::JumpingSprite),
         sad::dukpp03::setter::from(&game::ActorOptions::JumpingSprite)
    );
    binding->addAccessor(
        "FallingSprite",
         sad::dukpp03::getter::from(&game::ActorOptions::FallingSprite),
         sad::dukpp03::setter::from(&game::ActorOptions::FallingSprite)
    );
    binding->addAccessor(
        "DuckingSprite",
         sad::dukpp03::getter::from(&game::ActorOptions::DuckingSprite),
         sad::dukpp03::setter::from(&game::ActorOptions::DuckingSprite)
    );
    binding->addAccessor(
        "FloaterSprite",
         sad::dukpp03::getter::from(&game::ActorOptions::FloaterSprite),
         sad::dukpp03::setter::from(&game::ActorOptions::FloaterSprite)
    );
    binding->addAccessor(
        "FloaterFlyAnimationOptions",
        sad::dukpp03::getter::from(&game::ActorOptions::FloaterFlyAnimationOptions),
        sad::dukpp03::setter::from(&game::ActorOptions::FloaterFlyAnimationOptions)
    );
    ctx->addClassBinding("game::ActorOptions", binding);
}
