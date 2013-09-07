#include <p2d/app/object.h>
#include <orthocamera.h>
#include <texturemanager.h>
#include <input.h>
#include <extra/background.h>
#include <p2d/elasticforce.h>

#include "world.h"

#include "gridnode.h"
#include "gridnodedge.h"


World::World()
{
	this->initApp();
	/*m_world->setDetector(new p2d::MultisamplingCollisionDetector(3))*/;
	m_walls = new p2d::Walls();
	m_find = new p2d::FindContactPoints();
}


World::~World()
{
	delete m_walls;
	delete m_find;
}




void World::run()
{
	// Init new empty scene with orthographic projection
	sad::Scene * sc= new sad::Scene();
	sc->setCamera(new OrthoCamera());
	sc->add(new sad::Background("background"));
	sad::Renderer::ref()->setCurrentScene(sc);
	sad::Renderer::ref()->setWindowTitle("Physics stress test");

	sad::Renderer::ref()->controls()->setMouseMoveHandler( new sad::MethodEventHandler<sad::Event, World>(this, &World::onMouseMove));

	// Load textures to videocard memory and build mipmaps
	sad::Renderer::ref()->textures()->buildAll();
	SL_MESSAGE("Mips built successfully");	
	
	// Set window size to be fixed
	sad::Renderer::ref()->toggleFixedOn();

	m_steptask = new p2d::WorldStepTask(m_world);
	sad::Input::ref()->addPostRenderTask(m_steptask);


	// SETUP WORLD CALLBACKS HERE!!!
	m_world->addHandler(this, &World::onWallNode);
	m_world->addHandler(this, &World::onNodeNode);

	// Add walls
	hst::vector<p2d::Body *> bodies = m_walls->bodies();
	for(int i = 0; i < bodies.count(); i++)
	{
		m_world->add(bodies[i]);
	}

	// Build elastic grid with 9 nodes

	// Produce a nodes of grids
	GridNode * g[9];
	for(int i = 0; i < 9; i++)
	{
		g[i] = new GridNode();
		g[i]->setPosition(p2d::Point(300 + (i % 3) * 100, 400 - (i / 3) * 100));
		// Add a gravity force
		if (i != 0 && i != 2) 
		{
			g[i]->body()->tangentialForces().add( new p2d::TangentialForce(p2d::Vector(0, -60) ) );
		}
		else
		{
			// Make bodies unmovable
			g[i]->body()->weight().setValue(1E+6);
		}
	}

	// Sets forces, between nodes	
	int forces[20][2] =  {
		{0, 1}, {2, 1}, {4, 1},
		{0, 3}, {4, 3}, {6, 3},
		{3, 4}, {1, 4}, {5, 4}, {7, 4},
		{2, 5}, {4, 5}, {8, 5},
		{3, 6}, {7, 6},
		{6, 7}, {4, 7}, {8, 7},
		{5, 8}, {7, 8}
	};
	for(int i = 0; i < 20; i++)
	{
		int  f = forces[i][0];
		int  s = forces[i][1];
		g[s]->body()->tangentialForces().add( new p2d::ElasticForce(g[f]->body(), g[s]->body()) );
	}
	int graphic[12][2] = { 
		{0, 1}, {1, 2}, {0, 3}, {1, 4}, {3, 4}, {2, 5},
		{4, 5}, {3, 6}, {4, 7}, {6, 7}, {5, 8}, {7, 8}
	};
	// Build grafical representaion for elastic grid
	for(int i = 0; i < 12; i++)
	{
		sc->add( new GridNodeEdge(g[graphic[i][0]]->body(), g[graphic[i][1]]->body()) );
	}
    
	// Add elastic grid nodes to scene
	for(int i = 0 ; i < 9; i++)
	{
		this->addObject(g[i]);
	}

	// Add ball to scene
	GridNode * ball = new GridNode();
	ball->setPosition(p2d::Point(20, 300));
	ball->body()->tangentialForces().add( new p2d::TangentialForce(p2d::Vector(0, -30) ) );
	ball->body()->setCurrentTangentialVelocity(p2d::Vector(140, 120));
	this->addObject(ball);
	
	// Add FPS counter
	sc->add( new World::FPSLabel() );


	// Run an engine, starting a main loop
	SL_MESSAGE("Will start now");	

	sad::Renderer::ref()->run();
}


void World::quit()
{
	sad::Renderer::ref()->quit();
}


/*! Handled this type of collision with manual from
	http://alexandr4784.narod.ru/sdvmpdf1/smgl04_28.pdf
 */
void World::onWallNode(const p2d::CollisionEvent<GridNode, p2d::Wall> & ev)
{
	p2d::BoundType bt = ev.m_object_2->type();
	double x = fabs(ev.m_object_1->body()->tangentialVelocity().x());
	double y = fabs(ev.m_object_1->body()->tangentialVelocity().y());
	
	p2d::Vector av1 = ev.m_object_1->body()->averageChangeIndependentTangentialVelocity();
	p2d::SetOfPointsPair pairs = m_find->invoke(ev.m_object_1->body()->currentShape(),
												av1,
												ev.m_object_2->body()->currentShape(),
												p2d::Vector(0,0)
											   );
	if (pairs.size() > 0)
	{
		double time = 0;
		double x1 = pairs[0].p1().x();
		double x2 = pairs[0].p2().x();
		double y1 = pairs[0].p1().y();
		double y2 = pairs[0].p2().y();
		if (non_fuzzy_zero(av1.x()) )
		{
			time = (x1 - x2) / (- (av1.x()));  
		}
		else
		{
			time = (y1 - y2) / (- (av1.y()));  
		}
		p2d::Point p = ev.m_object_1->body()->currentShape()->center();
		p += av1 * time;
		ev.m_object_1->body()->shedulePosition(p);
	}

	if (ev.m_object_2->type() == p2d::BT_LEFT)
	{
		ev.m_object_1->body()->sheduleTangentialVelocity(p2d::Vector(x,ev.m_object_1->body()->tangentialVelocity().y()) );
	}
	if (ev.m_object_2->type() == p2d::BT_RIGHT)
	{
		ev.m_object_1->body()->sheduleTangentialVelocity(p2d::Vector(-x,ev.m_object_1->body()->tangentialVelocity().y()) );
	
	}
	if (ev.m_object_2->type() == p2d::BT_UP)
	{
		ev.m_object_1->body()->sheduleTangentialVelocity(p2d::Vector(ev.m_object_1->body()->tangentialVelocity().x(), -y) );	
	}
	if (ev.m_object_2->type() == p2d::BT_DOWN)
	{
		ev.m_object_1->body()->sheduleTangentialVelocity(p2d::Vector(ev.m_object_1->body()->tangentialVelocity().x(), y) );	
	}
}


World::FPSLabel::FPSLabel()
{
	// Extract font with label
	this->font()=sad::Renderer::ref()->fonts()->get("times_lg");
	this->point()=pointf(0,sad::Renderer::ref()->settings().height());
	// Set rendering point to current point
	this->render_point()=this->point();
}

void World::FPSLabel::render()
{
	string() = hst::string::number(sad::Renderer::ref()->fps());
	this->Label::render();
}


void World::onNodeNode(const p2d::CollisionEvent<GridNode, GridNode> & ev)
{
	p2d::Vector av1 = ev.m_object_1->body()->averageChangeIndependentTangentialVelocity();
	p2d::Vector av2 = ev.m_object_2->body()->averageChangeIndependentTangentialVelocity();

	p2d::SetOfPointsPair pairs = m_find->invoke(ev.m_object_1->body()->currentShape(),
												av1,
												ev.m_object_2->body()->currentShape(),
												av2
											   );

	if (pairs.size() > 0)
	{
		// Compute time of impact
		double x1 = pairs[0].p1().x();
		double y1 = pairs[0].p1().y();
		double x2 = pairs[0].p2().x();
		double y2 = pairs[0].p2().y();

		double avx1 = av1.x();
		double avy1 = av1.y();
		double avx2 = av2.x();
		double avy2 = av2.y();

		double time = 0;
		if (non_fuzzy_zero(avx2 - avx1))
		{
			time = (x1 - x2) / (avx2 - avx1);
		}
		else
		{
			time = (y1 - y2) / (avy2 - avy1);
		}
				
		time -= 1.0E-7;

		double m1 = ev.m_object_1->body()->weight().value();
		double m2 = ev.m_object_2->body()->weight().value();

		

		p2d::Point normal1 = pairs[0].p1();
		normal1 -= ev.m_object_1->body()->currentShape()->center();
		normal1 = p2d::unit(normal1);

		p2d::Point normal2 = pairs[0].p2();
		normal2 -= ev.m_object_2->body()->currentShape()->center();
		normal2 = p2d::unit(normal2);

		p2d::Vector v1 = ev.m_object_1->body()->tangentialVelocityAt(time);
		double project1 = p2d::scalar(v1, normal1);
		p2d::Vector normalPart1 = normal1;
		normalPart1 *= project1;

		p2d::Vector tangentialPart1 = v1;
		tangentialPart1 -= normalPart1;

		p2d::Vector v2 = ev.m_object_2->body()->tangentialVelocityAt(time);
		double project2 = p2d::scalar(v2, normal2);
		p2d::Vector normalPart2 = normal2;
		normalPart2 *= project2;

		p2d::Vector tangentialPart2 = v2;
		tangentialPart2 -= normalPart2;

		p2d::Vector cachedNormal1 = normalPart1;
		normalPart1 *= -1;
		normalPart1 += (cachedNormal1 * m1 + normalPart2 * m2) / (m1 + m2);

		

		if (ev.m_object_1->body()->willTangentialVelocityChange())
		{
			// Merge two impulses into one
			p2d::Vector impulse = ev.m_object_1->body()->nextTangentialVelocity();
			impulse -= ev.m_object_1->body()->tangentialVelocity();

			impulse += (normalPart1 + tangentialPart1) - ev.m_object_1->body()->tangentialVelocity();
			
			impulse /= 2.0;
			// Here sum of speeds is computed
			impulse +=  ev.m_object_1->body()->tangentialVelocity();
			ev.m_object_1->body()->sheduleTangentialVelocity(impulse);
		}
		else
		{
			ev.m_object_1->body()->sheduleTangentialVelocity(normalPart1 + tangentialPart1);
		}
		
		
		
		if (ev.m_object_1->body()->willPositionChange())
		{
			p2d::Vector position1 = av1;
			position1 *= time;
			position1 += ev.m_object_1->body()->nextPosition() - ev.m_object_1->body()->position();
			position1 /= 2.0;
			ev.m_object_1->body()->shedulePosition(ev.m_object_1->body()->position() + position1);
		}
		
		else
		{
			p2d::Vector position1 = ev.m_object_1->body()->currentShape()->center();
			position1 += av1 * time;
			ev.m_object_1->body()->shedulePosition(position1);
		}

		p2d::Vector cachedNormal2 = normalPart2;
		normalPart2 *= -1;
		normalPart2 += (cachedNormal1 * m1 + cachedNormal2 * m2) / (m1 + m2);
	

		if (ev.m_object_2->body()->willTangentialVelocityChange())
		{
			// Merge two impulses into one
			p2d::Vector impulse = ev.m_object_2->body()->nextTangentialVelocity();
			impulse -= ev.m_object_2->body()->tangentialVelocity();

			impulse += (normalPart2 + tangentialPart2) - ev.m_object_2->body()->tangentialVelocity();
			impulse /= 2.0;
			// Here sum of speeds is computed
			impulse +=  ev.m_object_2->body()->tangentialVelocity();
			ev.m_object_2->body()->sheduleTangentialVelocity(impulse);
		}
		else
		{
			ev.m_object_2->body()->sheduleTangentialVelocity(normalPart2 + tangentialPart2);
		}

		if (ev.m_object_2->body()->willPositionChange())
		{
			p2d::Vector position2 = av2;
			position2 *= time;
			position2 += ev.m_object_2->body()->nextPosition() - ev.m_object_2->body()->position();
			position2 /= 2;
			ev.m_object_2->body()->shedulePosition(ev.m_object_2->body()->position() + position2);
		}
		else
		{
			p2d::Vector position2 = ev.m_object_2->body()->currentShape()->center();
			position2 += av2 * time;
			ev.m_object_2->body()->shedulePosition(position2);
		}

		
	}
	else
	{
		hst::string tpl = "Cannot find a contact points. Performing an object dump\n";
		tpl <<  "1st body:\ncenter at ({0},{1})\nradius {2}\n";
		tpl <<  "velocity ({3},{4})\n";
		tpl <<  "2nd body:\ncenter at ({5},{6})\nradius {7}\n";
		tpl <<  "velocity ({8},{9})\n";
		
		p2d::Point center1 = ev.m_object_1->body()->currentShape()->center();
		double radius1 = static_cast<p2d::Circle*>(ev.m_object_1->body()->currentShape())->radius();

		p2d::Point center2 = ev.m_object_2->body()->currentShape()->center();
		double radius2 = static_cast<p2d::Circle*>(ev.m_object_2->body()->currentShape())->radius();

		SL_CRITICAL(fmt::Format(tpl) << center1.x() << center1.y() << radius1
									 << av1.x() << av1.y()
									 << center2.x() << center2.y() << radius2
									 << av2.x() << av2.y()
			       );
	}
}


void World::onMouseMove(const sad::Event & ev)
{
	//SL_DEBUG(fmt::Format("{0} {1}") << ev.x << ev.y);
}

