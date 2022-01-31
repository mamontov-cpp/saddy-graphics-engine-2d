/*! \file primitiverenderer.h
    

    A special part of renderer, which renders lines and rectangles
 */
#pragma once
#include "sadrect.h"
#include "sadcolor.h"

namespace sad
{
class Scene;
class ShaderFunction;

/*! \class PrimitiveRenderer
    
    A special part of renderer, which response to line rendering and rectangles
 */
class PrimitiveRenderer
{
public:
    /*! Constructs default primitive renderer
     */
    PrimitiveRenderer();
    /*! You can override primitive render to render own primitives in a way, you like it
     */
    virtual ~PrimitiveRenderer();
    /*! Renders a line with specified color
        \param[in] scene a scene
        \param[in] p1 a first point of renderer
        \param[in] p2 a second point of renderer
        \param[in] c  a color, with which line should be rendered
        \param[in] fun a shading function to apply (nullptr for default)
        \param[in] no_shader_fun do not apply shader function
     */
    virtual void line(
        sad::Scene* scene,
        const sad::Point2D& p1,
        const sad::Point2D& p2,
        const sad::AColor& c,
        sad::ShaderFunction* fun = nullptr,
        bool no_shader_fun = false
    );
    /*! Renders a rectangle with specified color
        \param[in] scene a scene
        \param[in] r rectangle to be rendered
        \param[in] c a color, with which line should be rendered
        \param[in] fun a shading function to apply (nullptr for default)
     */
    virtual void rectangle(
        sad::Scene* scene,
        const sad::Rect2D & r,
        const sad::AColor & c,
        sad::ShaderFunction* fun = nullptr
    );
    /*! Renders a circle with specifier color as multiple-segment line. If segment amount less than 3, circle won't be drawn
     *  \param[in] scene a scene
     *  \param[in] center a center
     *  \param[in] radius a radius of circle
     *  \param[in] c color
     *  \param[in] segments amount of segment to draw
     *  \param[in] fun a shading function to apply (nullptr for default)
     */
    virtual void circle(
        sad::Scene* scene,
        const sad::Point2D& center,
        double radius,
        const sad::AColor & c,
        size_t  segments = 12,
        sad::ShaderFunction* fun = nullptr
    );
};

}
