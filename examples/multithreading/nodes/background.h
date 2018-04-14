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
     */
    Background();
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
    virtual void setScene(sad::Scene * scene);
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
    /*! A main texture rectangle
     */
    sad::Rect2D m_main_texture_rect;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(nodes::Background)