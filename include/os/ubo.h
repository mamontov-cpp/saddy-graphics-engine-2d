/*! \file ubo.h
    
    Uniform buffer objects, used for storing data between shaders are stored here
 */
#pragma once
#include "glheaders.h"

namespace sad
{
class Renderer;

namespace os
{
class ExtensionFunctions;

/*! Defines uniform buffer object, used for storing data between shaders
 */
class UBO
{
public:
    /*! Constructs new uniform buffer object
        \param[in] renderer a renderer object
        \param[in] buffer_size buffer size
     */
    UBO(sad::Renderer* renderer, size_t buffer_size);
    /*! Copies uniform buffer object
        \param[in] ubo other object
     */
    UBO(const sad::os::UBO& ubo);
    /*! Copies uniform buffer object
        \param[in] ubo other object
        \return self-reference
     */
    sad::os::UBO& operator=(const sad::os::UBO& ubo);
    /*! Tries load UBO to GPU
     */
    void tryLoadToGPU();
    /*! Unloads UBO to GPU
     */
    void tryUnload();
    /*! Sets sub data in UBO
        \param[in] offset offset parameter in UBO
        \param[in] size size of buffer
        \param[in] data a data parameter
     */
    void setSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data);
    /*! Sets user data for UBO
        \param[in] user_data data
     */
    void setUserData(void* user_data);
    /*! Returns user data
        \return user data
     */
    void* userData() const;
    /*! Binds buffer
        \param[in] offset offset data
        \param[in] uniformBlockBinding binding point
     */
    void bind(GLintptr offset, GLuint uniformBlockBinding);
    /*! Returns id an id of binding
     */
    GLuint id() const;
    /*! Destroys uniform buffer object
     */
    ~UBO();
    /*! Tries to log OpenGL error if occurred
     *  \param[in] op log info
     */
    void tryLogGlError(const char* op);
private:
    /*! A renderer for geometry
     */
    sad::Renderer* m_renderer;
    /*! An if for buffer 
     */
    GLuint m_id;
    /*! A size for buffer
     */
    size_t m_buffer_size;
    /*! Functions
     */
    sad::os::ExtensionFunctions* m_f;
    /*! Last specific user data for checked application
     */
    void* m_user_data;
};

}

}
