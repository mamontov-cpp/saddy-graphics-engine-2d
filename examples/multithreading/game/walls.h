/*! \file walls.h
 
    A walls for game
 */
 #pragma once
 #include <p2d/body.h>

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
private:
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