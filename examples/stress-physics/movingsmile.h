/*! \file movingsmile.h
	

	Describes a moving smile
 */
 #include <p2d/app/way.h>
#include <p2d/app/object.h>
#include <p2d/app/constants.h>
#pragma once

/*! As a default, grid node is a node
 */
class MovingSmile: public sad::p2d::app::Object
{
 SAD_OBJECT
 protected:
	 sad::p2d::app::Way::WayLink m_link;
 public:
	 MovingSmile(sad::p2d::app::Way * o);
	 void step(double time);

	 virtual void render();
};
