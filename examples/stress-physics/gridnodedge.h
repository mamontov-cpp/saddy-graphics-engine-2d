/*! \file gridnodedge.h
    

    Describes a basic grid node edge
 */
#pragma once
#include <scene.h>
#include <p2d/body.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <p2d/app/constants.h>
#include <sprite2d.h>
#pragma once

/*! As a default, grid node is a node
 */
class GridNodeEdge: public sad::SceneNode
{
 SAD_OBJECT
 protected:
     sad::p2d::Body * m_b1;
     sad::p2d::Body * m_b2;
     sad::Sprite2D * m_ad;
 public:
     GridNodeEdge(sad::p2d::Body * b1, sad::p2d::Body * b2);

     virtual void render() override;
     virtual void setScene(sad::Scene * o) override;
     virtual void rendererChanged() override;

     virtual ~GridNodeEdge();
};
