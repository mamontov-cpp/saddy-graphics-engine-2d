/*! \file core\borders\deletehotspot.h
    

    A hotspot for object removal
 */
#pragma once

#include <sprite2d.h>

namespace core
{

class Shared;

namespace borders
{

/*! A hotspot for item removal
 */
class DeleteHotspot
{
public:	
    /*! Creates default hotspot
     */
    DeleteHotspot();
    /*! Could be inherited
     */
    virtual ~DeleteHotspot();
    /*! A scene to be used for rendering
     *  \param[in] scene a scene
     */
    void setScene(sad::Scene* scene) const;
    /*! Tries to render hotspot
     *  \param[in] region a bounding region for object
     */
    void render(const sad::Rect2D& region);
    /*! Whether point is within hotspot
     *  \param[in] p point
     */
    bool isWithin(const sad::Point2D& p) const;
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
};

}

}
