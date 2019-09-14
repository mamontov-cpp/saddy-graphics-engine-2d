/*! \file resizehotspot.h
    

    Defines a hotspot for resizing elements in editor
 */
#pragma once

#include <sprite2d.h>

#include <sadpair.h>

#include <p2d/vector.h>

namespace core
{

class Shared;

namespace borders
{

/*! A hotspot for item resizing
 */
class ResizeHotspot
{
public:	
    /*! Creates default hotspot
        \param[in] pivot1 a first pivot point index for adjacent elements
        \param[in] pivot2 a second pivot point index for adjacent elements
        \param[in] adjacent an adjancent point index to first point
        \param[in] p point
     */
    ResizeHotspot(int pivot1, int pivot2, int adjacent, const sad::p2d::Vector& p);
    /*! Could be inherited
     */
    virtual ~ResizeHotspot();
    /*! A scene to be used for rendering
        \param[in] scene a scene
     */
    void setScene(sad::Scene* scene) const;
    /*! Tries to render hotspot
        \param[in] region a bounding region for object
     */
    void render(const sad::Rect2D& region);
    /*! Whether point is within hotspot
        \param[in] p point
     */
    bool isWithin(const sad::Point2D& p) const;
    /*! Returns direction vector for resizing
        \param[in] r rectangle
     */
    sad::p2d::Vector directionVector(const sad::Rect2D& r) const;
    /*! Returns default direction vector for resizing
        \return default direction vector
     */
    sad::p2d::Vector defaultDirectionVector() const;
    /*! A points for resizing objects in hotspot
        \return a points for resizing in hotspots
     */
    const sad::Pair<int, int>& resizingIndexes() const;
    /*! A padding between source point and center of sprite 
     */
    static const int Padding;
protected:
    /*! Inner sprite to be rendered
     */
    sad::Sprite2D* m_sprite;
    /*! Whether sprite options are set
     */
    bool m_sprite_options_set;
    /*! A pivot pair of points for elements
     */
    sad::Pair<int, int> m_pivot_pair;
    /*! An adjacent pair index to first of pivot pair
     */
    int m_adjacent_pair_index;
    /*! A default vector to be returned, when pivot and adjacent pair are next to one
     */
    sad::p2d::Vector m_default_vector;
};

}

}
