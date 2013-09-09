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
#include <p2d/bouncesolver.h>
#include <label.h>

class GridNode;
class Ball;
class Platform;
class UncoloredBullet;
class Shooter;
class ColoredBullet;
/*! Main class for application
 */
class World: public p2d::app::App
{
protected:
	p2d::FindContactPoints * m_find;
	p2d::Walls * m_walls;
	p2d::BounceSolver * m_solver; //!< A solver for bouncing objects

	int m_hit_count; //!< A hit count
	void onWallBall(const p2d::CollisionEvent<Ball, p2d::Wall> & ev);
	void onBallNode(const p2d::CollisionEvent<Ball, GridNode> & ev);
	void incrementHitCount(const p2d::CollisionEvent<UncoloredBullet, Shooter> & ev);
	void eraseBullets(const p2d::CollisionEvent<ColoredBullet, UncoloredBullet> & ev);

	template<typename _O1, typename _O2>
	void performBounce(const p2d::CollisionEvent<_O1, _O2> & ev)
	{
		m_solver->bounce(ev.m_object_1->body(), ev.m_object_2->body());
	}
	template<typename _O1, typename _O2>
	void removeFirst(const p2d::CollisionEvent<_O1, _O2> & ev)
	{
		this->removeObject(ev.m_object_1);
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
	
	/*! Returns count, how much times player is hit
	 */
	int hitCount();
};


