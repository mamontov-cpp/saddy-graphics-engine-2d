/*!  \file staticobjectcontainer.h
 *
 * Defines a container for static objects, which should be stored for different room-like structure to implement seamless level-streaming
 */
#pragma once
#include <p2d/body.h>
#include <sprite2d.h>
#include <sadvector.h>


namespace game
{

/*! Defines a container for static objects
 */
class StaticObjectContainer
{
public:
    /*! A list of sprites for platforms
     */
    sad::Vector<sad::Sprite2D*> PlatformSprites;
    /*! A list of sprites for coins
     */
    sad::Vector<sad::Sprite2D*> CoinSprites;
    /*! A list of bodies for platforms
     */
    sad::Vector<sad::p2d::Body*> PlatformBodies;
    /*! A list of bodies for coins
     */
    sad::Vector<sad::p2d::Body*> CoinBodies;
    /*! Makes empty container
     */
    StaticObjectContainer();
    /*! Container stores weak links for platforms
     */
    ~StaticObjectContainer();
};

}
