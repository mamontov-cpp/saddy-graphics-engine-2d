/*! \file movingsmile.h
    

    Describes a moving smile
 */
#pragma once
#include <way.h>
#include <phy52d/object.h>
#pragma once

/*! As a default, grid node is a node
 */
class MovingSmile: public sad::phy52d::Object
{
 SAD_OBJECT
 protected:
     sad::Way* m_way;
     double m_time;
 public:
     MovingSmile(sad::Way * o);
     void step(double time);

     virtual void render() override;
};
