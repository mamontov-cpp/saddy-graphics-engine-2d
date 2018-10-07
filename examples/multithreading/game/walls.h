/*! \file walls.h
 
    A walls for game
 */
#pragma once
#include <p2d/body.h>

namespace sad
{
    
namespace p2d
{
    class World;
}

}

namespace game
{

/*! A walls for game, for implementing boundary collisions
 */
class Walls
{
public:
    /*! A const data for length of walls
     */
    static const int LENGTH;
    /*! Default initialization for walls
     */
    Walls();
    /* Copies walls settings
       \param[in] walls other walls
     */
    Walls(const game::Walls& walls);
    /*! Copies walls state from other walls
        \param[in] walls other walls
        \return self-reference
     */
    game::Walls& operator=(const game::Walls& walls);
    /*! Destructor for walls
     */
    ~Walls();
    /*! Inits walls with bounds
        \param[in] left a left bound
        \param[in] right a right bound
        \param[in] top a top bound
        \param[in] bottom a bottom bound
     */
    void init(double left, double right, double top, double bottom);
    /*! Frees all shapes from walls
     */
    void free();
    /*! Returns left wall
        \return left wall
     */
    sad::p2d::Body* leftWall() const;
    /*! Returns right wall
        \return right wall
     */
    sad::p2d::Body* rightWall() const;
    /*! Returns top wall
        \return top wall
     */
    sad::p2d::Body* topWall() const;
    /*! Returns bottom wall
        \return bottom wall
     */
    sad::p2d::Body* bottomWall() const;
    /*! Sets left bound for walls
        \param[in] bound a bound value
     */
    void setLeftBound(double bound);
    /*! Sets right bound for walls
        \param[in] bound a bound value
     */
    void setRightBound(double bound);
    /*! Sets top bound for walls
        \param[in] bound a bound value
     */
    void setTopBound(double bound);
    /*! Sets bottom bound for walls
        \param[in] bound a bound value
     */
    void setBottomBound(double bound);
    /*! Adds walls to specific world into "walls" group
     */
    void addToWorld(sad::p2d::World* world) const;
private:
    /*! Inits shapes for walls
     */
    void initShapes() const;
    /*! Destroys old shapes
     */
    void destroyOldShapes();
    /*! A left boundary collision bound
     */
    double m_left_bound;
    /*! A right boundary collision bound
     */
    double m_right_bound;
    /*! A top boundary collision bound
     */
    double m_top_bound;
    /*! A bottom boundary collision bound
     */
    double m_bottom_bound;
    /*! A left wall data
     */
    sad::p2d::Body* m_left_wall;
    /*! A right wall data
     */
    sad::p2d::Body* m_right_wall;
    /*! A bottom wall data
     */
    sad::p2d::Body* m_bottom_wall;
    /*! A top wall data
     */
    sad::p2d::Body* m_top_wall;
};

}