/*! \file world.h
	\author HiddenSeeker

	Describes a main physics simulation
 */
#pragma once
#include <extra/periodicalevent.h>
#include <scene.h>
#include <p2d/app/app.h>
#include <p2d/findcontactpoints.h>
#include <p2d/walls.h>
#include <label.h>
class GridNode;
class Ball;
class BounceSolver;
class UncoloredBullet;
/*! Main class for application
 */
class World: public p2d::app::App
{
protected:
	p2d::FindContactPoints * m_find;
	p2d::Walls * m_walls;
	BounceSolver * m_solver; //!< A solver for bouncing objects
	/*! Makes node moving in opposite direction, of a wall
		\param[in] ev event
	 */
	void onWallBall(const p2d::CollisionEvent<Ball, p2d::Wall> & ev);
	/*! Makes node moving in opposite direction, of a wall
		\param[in] ev event
	 */
	void onBallNode(const p2d::CollisionEvent<Ball, GridNode> & ev);
	/*! Makes node moving in opposite direction, of a wall
		\param[in] ev event
	 */
	void onWallNode(const p2d::CollisionEvent<GridNode, p2d::Wall> & ev);
	/*! Tests a collision between node and node
		\param[in] ev event
	 */
	void onNodeNode(const p2d::CollisionEvent<GridNode, GridNode> & ev);
	/*! Makes node moving in opposite direction, of a wall
		\param[in] ev event
	 */
	void onWallUncoloredBullet(const p2d::CollisionEvent<UncoloredBullet, p2d::Wall> & ev);
	/*! Makes node moving in opposite direction, of a wall
		\param[in] ev event
	 */
	void onBallUncoloredBullet(const p2d::CollisionEvent<UncoloredBullet, Ball> & ev);
	/*! Makes node moving in opposite direction, of a wall
		\param[in] ev event
	 */
	void onNodeUncoloredBullet(const p2d::CollisionEvent<UncoloredBullet, GridNode> & ev);
public:

	World();

	~World();
	/*! Runs a simulation
	 */
	void run();
	/*! Quits a simulation
	 */
	void quit();

	void onMouseMove(const sad::Event & ev);
	
};


