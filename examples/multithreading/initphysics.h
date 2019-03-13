/*! \file initphysics.h
 *  
 *  Defines a physics initialization code
 */
#pragma once
#include <p2d/world.h>
#include <db/dbdatabase.h>

#include <scene.h>

#include "game/movingplatformregistry.h"
#include "game/unanimatedcoins.h"

class Game;
namespace game
{
class StaticObjectContainer;
}

/*! Inits physics world
 *  \param[in] world a main world
 */
void initPhysicsWorld(sad::p2d::World* world);

/*! Inits physics platforms
 *  \param[in] world a main world
 *  \param[in] main_scene a scene
 *  \param[in] registry a registry for platforms
 *  \param[in] container a static object container
 */
void initPhysicsPlatforms(sad::p2d::World* world, sad::Scene* main_scene, game::MovingPlatformRegistry* registry, game::StaticObjectContainer* container);

/*! Inits coins
 *  \param[in] game a game
 *  \param[in] world a main world
 *  \param[in] db database
 *  \param[in] r renderer
 *  \param[in] coins a coins
 *  \param[in] container a static object container
 */
void initCoins(Game* game, sad::p2d::World* world, sad::db::Database* db, sad::Renderer* r, game::UnanimatedCoins* coins, game::StaticObjectContainer* container);
