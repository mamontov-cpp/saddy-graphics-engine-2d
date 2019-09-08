/*! \file shaderfunction.h
 *
 * Describes a simple shader function for implementing stateful shader interaction
 */
#pragma once
#include "shader.h"
#include "refcountable.h"

namespace sad
{

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
    /*! Sets texture for shader
     */
    virtual void setTexture(sad::Texture* tex);
    /*! An inner function, which should be called when rendering node, @see sad::Sprite2D for implementation
     */
    virtual void apply(sad::SceneNode* node);
    /*! Disables shader function
     */
    virtual void disable();
    /*! Can be inherited
     */
    virtual ~ShaderFunction();
private:
    /*! An inner shader
     */
    sad::Shader* m_shader;
    /*! A texture data
     */
    sad::Texture* m_texture;
};

}