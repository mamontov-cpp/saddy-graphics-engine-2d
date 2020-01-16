/*! \file fontshaderfunction.h
    
    Describes shader function, that can be used for fonts
 */
#pragma once
#include "shaderfunction.h"

namespace sad
{

class FontShaderFunction : public sad::ShaderFunction
{
public:
    /*! A default shader function
     */
    FontShaderFunction();
    /*! Copies a shader function
     *  \param[in] fun function
     */
    FontShaderFunction(const sad::ShaderFunction& fun);
    /*! Copies state
     *  \param[in] fun function
     *  \return self-reference
     */
    sad::FontShaderFunction& operator=(const sad::FontShaderFunction& fun);
    /*! An inner function, which should be called when rendering node, @see sad::Label for implementation
     * \param[in] node node, on which shader should be applied
     * \param[in] tex texture, which should be applied
     * \param[in] clr color, which should be used in blending
     * \param[in] center center
     * \param[in] double angle
     */
    virtual void apply(sad::SceneNode* node, sad::Bindable* tex, const sad::AColor* clr, const sad::Point2D& center, double angle);
    /*! An inner function, which should be called when rendering node lines, @see sad::Label for implementation
     * \param[in] tex texture, which should be applied
     * \param[in] clr color, which should be used in blending
     * \param[in] center center
     * \param[in] double angle
     */
    virtual void apply(sad::SceneNode* node, const sad::AColor* clr, const sad::Point2D& center, double angle);
    /*! Describes if can be used for fonts
     */
    virtual bool canBeUsedForFonts() const;
    /*! Can be inherited
     */
    virtual ~FontShaderFunction();
protected:
    /*! Tries to cache some locations
     */
    virtual void tryCacheLocations();
    /*! A center location id
     */
    int m_center_loc_id;
    /*! An angle location id
     */
    int m_angle_loc_id;
};


}