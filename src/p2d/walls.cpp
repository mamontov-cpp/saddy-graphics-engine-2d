#include <p2d/walls.h>
#include <p2d/bounds.h>
#include <renderer.h>

DECLARE_SOBJ(Wall);

Wall::Wall(double padding) :m_padding(padding)
{
	m_body = NULL;
	m_opposite_body = NULL;
}

void Wall::tryTeleport(p2d::Body * b)
{
  p2d::Bound * bound = static_cast<p2d::Bound *>(this->m_opposite_body->currentShape());
  p2d::Vector  n = bound->normal();
  p2d::CollisionShape & s = b->at(b->timeStep());
  p2d::Cutter1D projection = s.project(n);
  double center = p2d::scalar(s.center(), n);

  // TODO: Teleport to opposite wall

}


Walls::Walls(double padding) : m_padding(padding)
{
	double w = sad::Renderer::ref()->settings().width();
	double h = sad::Renderer::ref()->settings().height();
	
	hst::vector< minimal_t > pairs;
	pairs << minimal_t( p2d::BT_LEFT, -padding );
	pairs << minimal_t( p2d::BT_RIGHT, w + padding);
	pairs << minimal_t( p2d::BT_UP, h + padding );
	pairs << minimal_t( p2d::BT_DOWN, -padding );

	for(size_t i = 0; i < pairs.size(); i++)
	{
		p2d::Bound * bound = new p2d::Bound();
		bound->setPosition(pairs[i].p2());
		bound->setType(pairs[i].p1());
		
		p2d::Body * b = new p2d::Body();
		b->setShape(bound);
		m_bodies << b;

		Wall * w = new Wall(padding);
		b->setUserObject(w);
		w->setBody(b);
		m_walls << w;
	}

	m_walls[0]->setOppositeBody(m_walls[1]->body());
	m_walls[1]->setOppositeBody(m_walls[0]->body());
	m_walls[2]->setOppositeBody(m_walls[3]->body());
	m_walls[3]->setOppositeBody(m_walls[2]->body());
}

const hst::vector<p2d::Body *> Walls::bodies() const
{
	return m_bodies;
}


Walls::~Walls()
{
	for(size_t i = 0; i < m_walls.count(); i++)
	{
		delete m_walls[i];
	}
}
