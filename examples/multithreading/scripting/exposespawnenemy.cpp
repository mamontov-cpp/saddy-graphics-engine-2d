#include "exposespawnenemy.h"

#include <dukpp-03/context.h>


#include <animations/animationsinstance.h>
#include <animations/animationsoptionlist.h>

#include "../game.h"
#include "../game/actoroptions.h"

#include "../bots/jsbot.h"
#include "../bots/divingfloaterbot.h"
#include "../bots/followplayerfloater.h"
#include "../bots/directionbot.h"
#include "../bots/platformpatrolbot.h"
#include "../bots/wayfloater.h"


void scripting::exposeSpawnEnemy(void* c, Game* game)
{
    sad::dukpp03::Context* ctx = reinterpret_cast<sad::dukpp03::Context*>(c);

    std::function<game::Actor*(const sad::String&,
                       const sad::Point2D&,
                       const sad::dukpp03::CompiledFunction&,
                       const sad::Hash<sad::String, sad::db::Variant>&)
    > spawn_enemy_walker =  [=](const sad::String& optname,
            const sad::Point2D& middle,
            const sad::dukpp03::CompiledFunction& function,
            const sad::Hash<sad::String, sad::db::Variant>& state
    ) {
        game::Actor* actor = game->makeEnemy(optname, middle);
        if (actor)
        {
            game->addActor(actor, new bots::JSBot(function, state, ctx));
        }
        return actor;
    };
    std::function<game::Actor*(const sad::String&,
        const sad::Point2D&,
        const sad::String&)
    > spawn_enemy_walker2 = [=](const sad::String& optname,
        const sad::Point2D& middle,
        const sad::String& botname
    ) {
        if (game->hasBotInRegistry(botname))
        {
            game::Actor* actor = game->makeEnemy(optname, middle);
            if (actor)
            {
                game->addActor(actor, game->getFromRegistry(botname)->clone());
            }
            return actor;
        }
        game::Actor* result = NULL;
        return result;
    };
    std::function<game::Actor*(const sad::String&,
        const sad::Point2D&,
        double from,
        double to,
        double dive_height
    )
    > spawn_animated_floater = [=](const sad::String& optname,
        const sad::Point2D& middle,
        double from,
        double to,
        double dive_height
        ) {
        game::Actor* actor = game->makeEnemy(optname, middle);
        if (actor)
        {
            game->addActor(actor, new bots::DivingFloaterBot(from, to, middle.y(), middle.y() - dive_height));

            sad::animations::OptionList* list = new sad::animations::OptionList();
            list->setList(game->getActorOptions(optname)->FloaterFlyAnimationOptions);
            list->setTime(200);
            list->setLooped(true);

            sad::animations::Instance* i = new sad::animations::Instance();
            i->setAnimation(list);
            i->setObject(actor->sprite());

            game->addToMainRenderer(i);
        }
        return actor;
    };
    std::function<game::Actor*(const sad::String&,
        const sad::Point2D&
    )
    > spawn_follow_player_floater = [=](const sad::String& optname,
        const sad::Point2D& middle
    ) {
        game::Actor* actor = game->makeEnemy(optname, middle);
        if (actor)
        {
            game->addActor(actor, new bots::FollowPlayerFloater());
        }
        return actor;
    };
    std::function<game::Actor*(const sad::String&,
        const sad::Point2D&,
        int,
        int
    )
    > spawn_enemy_in_direction = [=](const sad::String& optname,
        const sad::Point2D& middle,
        int h, int v
    ) {
        game::Actor* actor = game->makeEnemy(optname, middle);
        if (actor)
        {
            game->addActor(actor, new bots::DirectionBot(h, v));
        }
        return actor;
    };
    std::function<game::Actor*(const sad::String&,
        const sad::Point2D&
    )
    > spawn_platform_patrol = [=](const sad::String& optname,
        const sad::Point2D& middle
    ) {
        game::Actor* actor = game->makeEnemy(optname, middle);
        if (actor)
        {
            game->addActor(actor, new bots::PlatformPatrolBot());
        }
        return actor;
    };
    std::function<game::Actor*(const sad::String&,
        const sad::Point2D&,
        double time,
        const sad::String&
    )
    > spawn_way_floater = [=](const sad::String& optname,
        const sad::Point2D& middle,
        double time,
        const sad::String& way
        ) {
        game::Actor* actor = game->makeEnemy(optname, middle);
        if (actor)
        {
            game->addActor(actor, new bots::WayFloater(time, way));
        }
        return actor;
    };
    sad::dukpp03::MultiMethod* spawn_enemy_walker_at = new sad::dukpp03::MultiMethod();
    spawn_enemy_walker_at->add(sad::dukpp03::make_lambda::from(spawn_enemy_walker));
    spawn_enemy_walker_at->add(sad::dukpp03::make_lambda::from(spawn_enemy_walker2));
    ctx->registerCallable("spawnEnemyWalkerAt", spawn_enemy_walker_at);
    ctx->registerCallable("spawnAnimatedFloater", sad::dukpp03::make_lambda::from(spawn_animated_floater));
    ctx->registerCallable("spawnFollowPlayerFloater", sad::dukpp03::make_lambda::from(spawn_follow_player_floater));
    ctx->registerCallable("spawnEnemyInDirection", sad::dukpp03::make_lambda::from(spawn_enemy_in_direction));
    ctx->registerCallable("spawnPlatformPatrol", sad::dukpp03::make_lambda::from(spawn_platform_patrol));
    ctx->registerCallable("spawnWayFloater", sad::dukpp03::make_lambda::from(spawn_way_floater));
}
