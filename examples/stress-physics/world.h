/*! \file game.h
	\author HiddenSeeker

	Describes a game state with all references and other stuff
 */
#pragma once
#include <extra/periodicalevent.h>
#include <scene.h>
#include <p2d/worldsteptask.h>
#include <p2d/world.h>
#include "wall.h"

class WorldObject;
class GridNode;
/*! Main class for application
 */
class World
{
protected:
	p2d::World * m_world;
	p2d::WorldStepTask * m_steptask;
	Walls * m_walls;
	/*! Increases score of player and decrements hp of enemy
		\param[in] ev event
	 */
	void onWallNode(const p2d::CollisionEvent<GridNode, Wall> & ev);
public:

	World();

	~World();
	/*! Runs a simulation
	 */
	void run();
	/*! Quits a simulation
	 */
	void quit();
	/*! Adds a game object
		\param[in] o game object
	 */
	void addObject(WorldObject * o);
};
