/*! \file background.h
    
    Determines a background for scene node
 */
#pragma once
#include <sprite2d.h>

namespace nodes
{
    
class Background: public sad::SceneNode
{
SAD_OBJECT
    /*! Makes new background, initializing a data
        \param[in] should_show whether we should show label
     */
    Background(bool should_show);
    /*! Frees memory from all sprites
     */
    virtual ~Background();


    /*! Renders a sprites, setting a blend functions
     */
    virtual void render();
    /*! Called, when renderer for scene is changed
     */
    virtual void rendererChanged();
    /*! Updates scene for all of background
        \param[in] scene a scene, which will render a node
    */
    virtual void setScene(sad::Scene* scene);
private: 
    /*! Background is non-copyable, to make implementation simpler
        \param[in] b other background
     */
    Background(const nodes::Background& b);
    /*! Background is non-copyable, to make implementation simpler
        \param[in] b other background
        \return background
     */
    nodes::Background& operator=(const nodes::Background& b);
    /*! A basic label
     */
    sad::Sprite2D* m_base_label;
    /*! A first rainbow texture to fit for label
     */
    sad::Sprite2D* m_rainbow_label1;
    /*! A second rainbow texture to fit for label
     */
    sad::Sprite2D* m_rainbow_label2;
    /*! An outline label
     */
    sad::Sprite2D* m_outline_label;
    /*! A main background 
     */
    sad::Sprite2D* m_main_background;
    /*! A hills background
     */
    sad::Sprite2D* m_hills_background;
    /*! An inner tile for drawing floor
     */
    sad::Sprite2D* m_inner_tile;
    /*! A top tile for drawing floor
    */
    sad::Sprite2D* m_top_tile;

    /*! A main texture rectangle
     */
    sad::Rect2D m_main_texture_rect;
    /*! An inner time for background
     */
    double m_t;
    /*! A second period time
     */
    double m_t2;
    /*! A third period time
     */
    double m_t3;
    
    /*! Whether we should show label
     */
    bool m_show_label;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(nodes::Background)