/*! \file worldsteptask.h
	\author HiddenSeeker

	A world step task can perform world step, basing on rendering intervals
 */
#include "world.h"
#include "../input.h"
#include "../renderer.h"

namespace p2d
{

/*! A world step task can perform world step, basing on rendering intervals
 */
class WorldStepTask: public sad::RepeatingTask
{
 protected:
	 sad::Renderer * m_renderer; //!< A renderer
	 p2d::World * m_world;    //!< A stepped world
	 bool         m_enabled;   //!< Whether task is enabled
 public:
	 /*! Constructs new step task
		 \param[in] w world
		 \param[in] r renderer
	  */
	 WorldStepTask(p2d::World * w, sad::Renderer * r = sad::Renderer::ref());
	 /*! Enables task
	  */
	 void enable();
	 /*! Distables task
	  */
	 void disable();
	 /*! Tests, whether task is enabled
		 \return whether task is enabled
	  */
	 bool enabled() const;
	 /*! Steps a world
	  */
	 virtual void perform();
};

}
