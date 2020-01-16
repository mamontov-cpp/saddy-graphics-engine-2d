/*! \file opengl.h
    

    Determines a main class for getting an OpenGL implementation data.
    Note, that this class does not require valid context to get 
    OpenGL specific data (it tries to create one by itself, if can)
 */
#pragma once
#include "sadstring.h"
#include "sadpair.h"

namespace sad
{
class Renderer;
namespace os
{
class ExtensionFunctions;
}


/*! A main class, for getting OpenGL implementation data.
    Note, that this class does not require valid context to get 
    OpenGL specific data (it tries to create one by itself, if can)
 */
class OpenGL
{
public:
    /*! Creates a default, non-attached implementation. Note, that
        implementation MUST BE ATTACHED to renderer (use sad::Renderer::openGL 
        instead).
     */
    OpenGL();
    /*! Destroys related memory objects
     */
    ~OpenGL();
    /*! Attaches an implementation to renderer
        \param[in] r renderer
     */
    void setRenderer(sad::Renderer * r);
    /*! Returns renderer, attached to implementation
     */
    sad::Renderer * renderer() const;
    /*! Returns a pair of major and minor version
        \return version of OpenGL
     */
    sad::Pair<int, int> version();
    /*! Returns a version of OpenGL as double
        \return version of OpenGL as double, where integral part is major version
                and fractional part is a minor version
     */
    double versionAsDouble();
    /*! Returns a vendor string from OpenGL
        \return a vendor string from OpenGL
     */
    const sad::String & vendor();
    /*! Returns a version string from OpenGL
        \return version string from OpenGL
     */
    const sad::String & versionAsString();
    /*! Returns a renderer string from OpenGL
        \return OpenGL renderer string
     */
    const sad::String & rendererString();
    /*! Returns shading language version
        \return shading language version
     */
    const sad::String & glslShadingLanguageVersion();
    /*! Returns an extensions string from OpenGL
        \return OpenGL extensions string
     */
    const sad::String & extensions();
    /*! Test, whether OpenGL extension is supported
        \param[in] extension a tested extension
        \return whether OpenGL extension is presented
     */
    bool supportsExtension(const sad::String & extension);
    /*! Returns related extension functions
     *  \return extensionFunctions related functions
     */
    sad::os::ExtensionFunctions* extensionFunctions() const;
protected:
    /*! Tries to fetch OpenGL strings via glGetString
     */
    void tryFetchStrings();
    /*! Tries to set a by fetching string with enum value via glGetString
        \param[out] s string
        \param[in]  e enum value 
     */
    void trySetString(sad::String & s, unsigned int e);
    /*! An attached renderer to implementation
     */
    sad::Renderer* m_renderer;
    /*! Whether data is fetched (tryFetchStrings finished successfully)
     */
    bool m_fetched;
    /*! A major version of OpenGL implementations
     */
    int m_major;
    /*! A minor version of OpenGL implementations
     */
    int m_minor;
    /*! A version string for OpenGL implementation
     */
    sad::String m_version_string;
    /*! A vendor string
     */
    sad::String m_vendor;
    /*! A renderer string
     */
    sad::String m_renderer_string;
    /*! A GLSL version string
     */
    sad::String m_glsl_version_string;
    /*! An extensions string
     */
    sad::String m_extensions;
    /*! An extension functions for opengl
     */
    sad::os::ExtensionFunctions* m_extension_functions;
};

}
