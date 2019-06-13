/*! \file shader.h
 *
 *  Interface of simple shader
 */
#pragma once

#include "sadstring.h"

#include "object.h"

#include <functional>


namespace sad
{

class Renderer;

namespace os
{

class ShaderImpl;

}

/*! A simple shader interface
 */
class Shader: public sad::Object
{
SAD_OBJECT
public:
    /*! Makes new implementation
     */
    Shader();
    /*! Copies shader implementation
     *  \param[in] o other implementation
     */
    Shader(const sad::Shader& o);
    /*! Copies other shader implementation
     *  \param[in] o other implementation
     *  \return self-reference
     */
    sad::Shader& operator=(const sad::Shader& o);
    /*! Frees an implementation
     */
    ~Shader();
    /*! Sets renderer for data
     *  \param[in] r renderer
     */
    void setRenderer(sad::Renderer* r) const;
    /*! Returns renderer for data
     *  \return renderer
     */
    sad::Renderer* renderer() const;
    /*! Sets vertex program for shader
     *  \param[in] vertexProgram a vertex program
     */
    void setVertexProgram(const sad::String& vertexProgram) const;
    /*! Sets fragment program for shader
     *  \param[in] fragmentProgram a fragment program
     */
    void setFragmentProgram(const sad::String& fragmentProgram) const;
    /*! Tries to load vertex program from file
     *  \param[in] fileName a path to file
     *  \return true on success, false otherwise
     */
    bool loadVertexProgramFromFile(const sad::String& fileName) const;
    /*! Tries to load vertex program from file
     *  \param[in] fileName a path to file
     *  \return true on success, false otherwise
     */
    bool loadFragmentProgramFromFile(const sad::String& fileName) const;
    /*! Tries to upload shader on GPU
     */
    void tryUpload() const;
    /*! Uses shader implementation
     */
    void use() const;
    /*! Disables shader
     */
    void disable() const;
    /*! Sets callback, which will be called, when shader is used
     *  \param[in] cb callback
     */
    void setOnUseCallback(const std::function<void()>& cb);
    /*! Sets callback, which will be called, when shader is deleted
     *  \param[in] cb callback
     */
    void setOnDestroyCallback(const std::function<void()>& cb);
    /*! Returns callback, which will be called, when shader is used
     *  \return callback
     */
    const std::function<void()>& onUseCallback() const;
    /*! Returns callback, which will be called, when shader is deleted
     *  \return callback
     */
    const std::function<void()>& onDestroyCallback() const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     */
    void setUniformVariable(const sad::String& loc_name, int v0) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     */
    void setUniformVariable(const sad::String& loc_name, unsigned int v0) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     */
    void setUniformVariable(const sad::String& loc_name, float v0) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     */
    void setUniformVariable(const sad::String& loc_name, int v0, int v1) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     */
    void setUniformVariable(const sad::String& loc_name, unsigned int v0, unsigned int v1) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     */
    void setUniformVariable(const sad::String& loc_name, float v0, float v1) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     */
    void setUniformVariable(const sad::String& loc_name, int v0, int v1, int v2) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     */
    void setUniformVariable(const sad::String& loc_name, unsigned int v0, unsigned int v1, unsigned int v2) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     */
    void setUniformVariable(const sad::String& loc_name, float v0, float v1, float v2) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     *  \param[in] v3 a fourth argument
     */
    void setUniformVariable(const sad::String& loc_name, int v0, int v1, int v2, int v3) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     *  \param[in] v3 a fourth argument
     */
    void setUniformVariable(const sad::String& loc_name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) const;
    /*! Sets uniform variable
     *  \param[in] loc_name location name
     *  \param[in] v0 a first argument
     *  \param[in] v1 a second argument
     *  \param[in] v2 a third argument
     *  \param[in] v3 a fourth argument
     */
    void setUniformVariable(const sad::String& loc_name, float v0, float v1, float v2, float v3) const;
    /*! Methods for freeing resources on GPU
     */
    void tryDestroy() const;
private:
    /*! Implementation of shader
     */
    sad::os::ShaderImpl* m_impl;
    /*! On use callback
     */
    std::function<void()> m_on_use;
    /*! On destroy callback
     */
    std::function<void()> m_on_destroy;
};

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::Shader)