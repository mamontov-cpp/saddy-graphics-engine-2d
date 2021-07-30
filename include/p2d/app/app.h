/*! \file world.h
    

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

/*! A main class of application, which uses physical engine
 */
class App
{
public:
    /*! Creates a new application
     */
    App();
    /*! Quits an application 
     */
    virtual void quit();
    /*! Removes a game object from game
        \param[in] o object
     */
    virtual void removeObject(p2d::app::Object * o);
    /*! Runs an application
     */
    virtual void run();
    /*! Adds a game object
        \param[in] o game object
        \param[in] additional_group an additional group, where data should be stored
     */
    void addObject(p2d::app::Object * o, const sad::String& additional_group = "");
    /*! Returns world, used in application
     */
    sad::p2d::World* world() const;
    /*! Creates application
     */
    virtual ~App();
protected:
    p2d::World   * m_world;          //!< A physical engine world
    p2d::WorldStepTask * m_step_task; //!< A main step task, used to work with world
    unsigned int         m_layer;    //!< A layer of scene, which should layer work with
    sad::Renderer* m_renderer; //!< A renderer fir application;
protected:
    /*! Returns current in-game scene. A helper function for renderer's scene
        \return scene from renderer
     */
    sad::Scene * scene();
    /*! Creates new physical world for working with optional bodies
     */
    virtual void createWorld();
    /*! Initializes application
        \param[in] layer a layer of scene, where world will be attached
        \param[in] r renderer (nullptr for global renderer)
     */
    virtual void initApp(unsigned int layer = 0, sad::Renderer * r = nullptr);
};

}

}

}
