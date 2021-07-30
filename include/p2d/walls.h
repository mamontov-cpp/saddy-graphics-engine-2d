/*! \file walls.h
    

    Describes a bounding walls, that teleports collided player to other wall
    and destroy other objects
 */
#pragma once
#include "body.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "circle.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "line.h"
#include "bounds.h"

#include "../object.h"

namespace sad
{

namespace p2d
{
class Wall;
/*! Describes walls as in-game bounding container, when object
    within walls behaves normally, and when out of walls - it'll be destroyed
    or teleported (if this is player)
 */
class Walls: public sad::Object
{
SAD_OBJECT
public:
    /*! Creates bounding walls with specified padding, which defines how far
        walls are from edges of window. If we pass here 14, that the bounding
        rectangle will be 14 points larger than screen bounds.
        \param[in] padding a padding value
     */
    Walls(double padding = 0);
    /*! Creates bounding walls with specified padding within window [0..width], [0..height].
        For example, if we pass 14 in padding we will get walls of [14..width-14], 
        [14..height-14]
        \param[in] width basic width of walls
        \param[in] height basic height of walls
        \param[in] padding a padding for walls
     */
    Walls(double width, double height, double padding = 0);
    /*! Returns physical bodies of walls to add them into a game world
     */
    sad::Vector<p2d::Body *> bodies() const;
    /*! Destroys walls
     */
    virtual ~Walls();
protected:
    /*! A non-re-entrant constructor, which creates walls, populating lists of 
        walls and their bodies. Note, that padding must be already pre-set before call
        of this method.
        \param[in] width width of walls
        \param[in] height height of walls
     */
    void makeWalls(double width, double height);

    sad::Vector<sad::p2d::Wall *> m_walls;  //!< A bounding walls
    sad::Vector<p2d::Body *> m_bodies; //!< A bodies

    
    double m_padding; //!< A padding for walls and bodies
};

/*! Describes a static non-moving wall
 */
class Wall: public sad::Object
{
SAD_OBJECT
public:
    /*! Creates an empty wall as object
        \param[in] padding a padding between scene rectangle and wall positions
     */
    Wall(double padding = 0.0);
    /*! Tries to teleport an object
        \param[in] b body
     */
    // ReSharper disable once CppInconsistentNaming
    void tryTeleport(p2d::Body * b);
    /*! Returns a body
        \return body
     */
    inline p2d::Body * body() const { return m_body; }
    /*! Sets new body
        \param[in] b body
     */
    void setBody(p2d::Body * b);
    /*! Returns body for opposite wall
        \return opposite body
     */
    inline p2d::Body * oppositeBody() const { return m_opposite_body; }
    /*! Sets body for opposite wall
        \param[in] b opposite body
       */
    void setOppositeBody(p2d::Body * b);
    /*! Returns type of a wall
        \return type of wall
     */
    p2d::BoundType  type() const;
    /*! Returns type of a wall as the integral value
        \return type of wall
     */
    int typeAsIntegralValue() const;
    /*! Frees all referenced bodies
     */
    ~Wall();
protected:
    /*! A padding for opposite wall
     */
    double m_padding;
    /*! A type of wall
     */
    p2d::Body  * m_opposite_body;
    /*! A body
     */
    p2d::Body * m_body;
};

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::p2d::Walls)
DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::p2d::Wall)
