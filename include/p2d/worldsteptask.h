/*! \file worldsteptask.h
	\author HiddenSeeker

	A world step task can perform world step, basing on rendering intervals
 */
#pragma once
#include "world.h"
#include "../pipeline/pipelineprocess.h"
#include "../renderer.h"

namespace sad
{

namespace p2d
{

/*! A world step task can perform world step, basing on rendering intervals
 */
class WorldStepTask: public sad::pipeline::AbstractProcess
{
 protected:
	 sad::Renderer * m_renderer; //!< A renderer
	 p2d::World * m_world;    //!< A stepped world
 public:
	 /*! Constructs new step task
		 \param[in] w world
		 \param[in] r renderer
	  */
	 WorldStepTask(p2d::World * w, sad::Renderer * r = sad::Renderer::ref());
	 /*! Sets a world
		 \param[in] world a world
	  */
	 void setWorld(p2d::World * world);
protected:
	/*! Invokes a delegate inside of process
	 */ 
	virtual void _process();
};

}

}
