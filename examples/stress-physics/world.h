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
class Platform;
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
	void onWallBall(const p2d::CollisionEvent<Ball, p2d::Wall> & ev);
	void onBallNode(const p2d::CollisionEvent<Ball, GridNode> & ev);
	void onWallUncoloredBullet(const p2d::CollisionEvent<UncoloredBullet, p2d::Wall> & ev);
	void onBallUncoloredBullet(const p2d::CollisionEvent<UncoloredBullet, Ball> & ev);
	

	template<typename _O1, typename _O2>
	void performBounce(const p2d::CollisionEvent<_O1, _O2> & ev)
	{
		m_solver->bounce(ev.m_object_1->body(), ev.m_object_2->body());
	}
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


