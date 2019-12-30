/*! \file shaderfunction.h
 *
 * Describes a simple shader function for implementing stateful shader interaction
 */
#pragma once
#include "shader.h"
#include "refcountable.h"

namespace sad
{

class Scene;
class SceneNode;
class Texture;

/*! A shader function stores the state and the shader, that can be shared across nodes
 *  and also have it's own state to animate shader
 */
class ShaderFunction: public sad::RefCountable
{
public:
    /*! A default shader function
     */
    ShaderFunction();
    /*! Copies a shader function
     *  \param[in] fun function
     */
    ShaderFunction(const sad::ShaderFunction& fun);
    /*! Copies state
     *  \param[in] fun function
     *  \return self-reference
     */
    sad::ShaderFunction& operator=(const sad::ShaderFunction& fun);
    /*! Sets shader for applying function
     *  \param[in] shader a shader data
     */
    virtual void setShader(sad::Shader* shader);
    /*! Returns shader a shader function
     *  \return shader data
     */
    sad::Shader* shader() const;
    /*! An inner function, which should be called when rendering node, @see sad::Sprite2D for implementation
     * \param[in] node node, on which shader should be applied
     * \param[in] tex texture, which should be applied
     * \param[in] clr color, which should be used in blending
     */
    virtual void apply(sad::SceneNode* node, sad::Texture* tex, const sad::AColor* clr = NULL);
    /*! An inner function, which should be called when rendering node, @see sad::Sprite2D for implementation
     * \param[in] scene a scene, where shader is applied
     * \param[in] tex texture, which should be applied
     * \param[in] clr color, which should be used in blending
     */
    virtual void apply(sad::Scene* scene, sad::Texture* tex, const sad::AColor* clr = NULL);
    /*! An inner function, which should be called when rendering node, @see sad::Sprite2D for implementation
     * \param[in] node node, on which shader should be applied
     * \param[in] clr color, which should be used in blending
     */
    virtual void apply(sad::SceneNode* node, const sad::AColor* clr = NULL);
    /*! An inner function, which should be called when rendering node, @see sad::Sprite2D for implementation
     * \param[in] scene a scene, where shader is applied
     * \param[in] clr color, which should be used in blending
     */
    virtual void apply(sad::Scene* scene, const sad::AColor* clr = NULL);
    /*! Disables shader function
     */
    virtual void disable();
    /*! Can be inherited
     */
    virtual ~ShaderFunction();
private:
    /*! Tries to cache some locations
     */
    void tryCacheLocations();
    /*! An inner shader
     */
    sad::Shader* m_shader;
    /*! Locations are cached
     */
    bool m_locations_are_cahed;
    /*! A camera location block id
     */
    int m_gl_camera_info_loc_id;
    /*! A texture location id
     */
    int m_tex_loc_id;
    /*! A color location id
     */
    int m_clr_loc_id;
};

}