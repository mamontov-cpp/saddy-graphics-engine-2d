#include "gridnodedge.h"

DECLARE_SOBJ_INHERITANCE(GridNodeEdge, sad::SceneNode)

GridNodeEdge::GridNodeEdge(sad::p2d::Body * b1, sad::p2d::Body * b2)
{
    m_b1 = b1;
    m_b2 = b2;
    m_ad = new sad::Sprite2D(nullptr, sad::Rect2D(), sad::Rect2D());
    const sad::Sprite2D::Options options(
        "objects",
        sad::Rect2D(sad::Point2D(89, 89), sad::Point2D(176, 111)),
        sad::Rect2D(sad::Point2D(-43, -11), sad::Point2D(43, 11))
    );
    m_ad->set(options);
}


void GridNodeEdge::render()
{
    const sad::p2d::Point p1 = m_b1->currentShape()->center();
    const sad::p2d::Point p2 = m_b2->currentShape()->center();
    this->m_ad->makeSpanBetweenPoints(sad::Rect2D(sad::Point2D(-43, -11), sad::Point2D(43, 11)), p1, p2);
    this->m_ad->render();
}

void GridNodeEdge::setScene(sad::Scene * o)
{
    this->m_ad->setScene(o);
}

void GridNodeEdge::rendererChanged()
{
    this->m_ad->rendererChanged();
}

GridNodeEdge::~GridNodeEdge()
{
    delete m_ad;
}
