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

/*! Inits physics world
 *  \param[in] world a main world
 */
void initPhysicsWorld(sad::p2d::World* world);

/*! Inits physics platforms
 *  \param[in] world a main world
 *  \param[in] main_scene a scene
 *  \param[in] registry a registry for platforms
 */
void initPhysicsPlatforms(sad::p2d::World* world, sad::Scene* main_scene, game::MovingPlatformRegistry* registry);

/*! Inits coins
 *  \param[in] world a main world
 *  \parma[in] db database
 *  \param[in] r renderer
 *  \param[in] coins a coins
 */
void initCoins(sad::p2d::World* world, sad::db::Database* db, sad::Renderer* r, game::UnanimatedCoins* coins);