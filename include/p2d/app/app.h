/*! \file world.h
	\author HiddenSeeker

	Describes a main application world
 */
#pragma once
#include "../worldsteptask.h"
#include "../world.h"
#include "../../scene.h"
#include "../../periodicalevent.h"

namespace sad
{

class Renderer;

namespace p2d
{

namespace app
{

class Object;

/*! A main class of game
 */
class App
{
protected:
	p2d::World   * m_world;          //!< A physical engine world
	p2d::WorldStepTask * m_steptask; //!< A main step task, used to work with world
protected:
	/*! Returns current in-game scene. A helper function for renderer's scene
		\return scene from renderer
	 */
	sad::Scene * scene();
	/*! Creates new physical world for working with optional bodies
	 */
	virtual void createWorld();
	/*! Inits application
		\param[in] r renderer (NULL for global renderer)
	 */
	virtual void initApp(sad::Renderer * r = NULL);
public:
	/*! Creates a new application
	 */
	App();
	/*! Quits a game 
	 */
	virtual void quit();
	/*! Removes a game object from game
		\param[in] o object
	 */
	virtual void removeObject(p2d::app::Object * o);
	/*! Runs a game
	 */
	virtual void run();
	/*! Adds a game object
		\param[in] o game object
	 */
	void addObject(p2d::app::Object * o);
	/*! Creates application
	 */
	virtual ~App();
};

}

}

}
