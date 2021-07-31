/*! \file renderways.h
    

    Defines a simple process to render some ways from database
 */
#pragma once
#include <pipeline/pipelineprocess.h>
#include <sprite2d.h>
#include <sadcolor.h>
#include <orthographiccamera.h>

namespace core
{
class Editor;
}

namespace gui
{

/*! A process to render some ways from database
 */
class RenderWays: public sad::pipeline::AbstractProcess
{
public:
    /*! Construct new delegate for rendering
        \param[in] editor editor
     */
    RenderWays(core::Editor* editor);
    /*! Enables rendering of ways
     */
    void enable() override;
    /*! Can be inherited
     */
    virtual ~RenderWays() override;
protected:
    /*! A scene
     */
    sad::Scene* m_scene;
    /*! A panel for rendering ways
     */
    core::Editor * m_editor;
    /*! Whether sprites are init
     */
    bool m_init;
    /*! A default sprite to render points
     */
    sad::Sprite2D* m_default_sprite;
    /*! A red sprite to render points
     */
    sad::Sprite2D* m_red_sprite;
    /*! A default color for selected point
     */
    sad::AColor m_default;
    /*! A red color
     */
    sad::AColor m_red;
    /*! Invokes a delegate inside of process
     */ 
    virtual void _process() override;
    /*! Renders arrow from begin to end
        \param[in] scene a scene to be used
        \param[in] begin a beginning of arrow
        \param[in] end an ending of arrow
        \param[in] c color
     */
    void renderArrow(
        sad::Scene* scene,
        const sad::Point2D& begin, 
        const sad::Point2D& end, 
        const sad::AColor& c
    );
};

}
