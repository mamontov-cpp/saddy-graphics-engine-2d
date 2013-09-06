#include "gridnodedge.h"

DECLARE_SOBJ_INHERITANCE(GridNodeEdge, sad::BasicNode)

GridNodeEdge::GridNodeEdge(p2d::Body * b1, p2d::Body * b2)
{
	m_b1 = b1;
	m_b2 = b2;
	m_ad = new Sprite2DAdapter(NULL, hRectF(), hRectF());
	Sprite2DAdapter::Options * o = p2d::app::Constants<GridNodeEdge>::sprite();
	this->m_ad->set(*o);
    delete o;
}


void GridNodeEdge::render()
{
	p2d::Point p1 = m_b1->currentShape()->center();
	p2d::Point p2 = m_b2->currentShape()->center();
	hRectF r(hPointF(0, -11), hPointF(0, 11));
	double distance = p1.distanceTo(p2) / 2;
	r[0].setX(r[0].x() - distance );
	r[2].setX(r[2].x() + distance );
	r[1].setX(r[1].x() + distance );
	r[3].setX(r[3].x() - distance );

	this->m_ad->rotate(this->m_ad->angle() * -1);
	this->m_ad->setRect(r);
	this->m_ad->move( (p1 +p2) / 2);
	double angle = atan2(p2.y() - p1.y(), p2.x() - p1.x());
	this->m_ad->rotate(angle);
	this->m_ad->render();
}
