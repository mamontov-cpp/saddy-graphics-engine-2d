#include "opengl.h"
#include "window.h"
#include "glcontext.h"
#include "renderer.h"
#include "fuzzyequal.h"
#include "db/dbtypename.h"


#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <3rdparty/glext/glext.h>

#include "os/extensionfunctions.h"


sad::OpenGL::OpenGL()
: m_renderer(NULL), m_fetched(false), m_major(0), m_minor(0), m_extension_functions(new sad::os::ExtensionFunctions())
{
    m_extension_functions->setParent(this);
}


sad::OpenGL::~OpenGL()
{
    delete m_extension_functions;
}

void sad::OpenGL::setRenderer(sad::Renderer * r)
{
    m_renderer = r;
}

sad::Renderer * sad::OpenGL::renderer() const
{
    return m_renderer;
}

sad::Pair<int, int> sad::OpenGL::version()
{
    tryFetchStrings();
    return sad::Pair<int, int>(m_major, m_minor);
}

double sad::OpenGL::versionAsDouble()
{
    tryFetchStrings();
    double result = (double)m_major;
    double fractionalpart = (double)m_minor;
    while(fractionalpart > 1 || sad::is_fuzzy_equal(fractionalpart, 1))
    {
        fractionalpart /= 10;
    }
    result += fractionalpart;
    return result;
}


const sad::String & sad::OpenGL::vendor()
{
    tryFetchStrings();
    return m_vendor;
}

const sad::String & sad::OpenGL::versionAsString()
{
    tryFetchStrings();
    return m_version_string;
}

const sad::String & sad::OpenGL::rendererString()
{
    tryFetchStrings();
    return m_renderer_string;
}

const sad::String & sad::OpenGL::glslShadingLanguageVersion()
{
    tryFetchStrings();
    return m_glsl_version_string;
}

const sad::String & sad::OpenGL::extensions()
{
    tryFetchStrings();
    return m_extensions;
}

bool sad::OpenGL::supportsExtension(const sad::String & extension)
{
    tryFetchStrings();
    return m_extensions.getOccurence(extension) != -1;
}

sad::os::ExtensionFunctions* sad::OpenGL::extensionFunctions() const
{
    return m_extension_functions;
}


void sad::OpenGL::tryFetchStrings()
{
    if (!m_fetched)
    {
        bool hasvalidcontext = false;
        bool owncontext = false;
        sad::Window * win;
        sad::GLContext * ctx;
        if (m_renderer)
        {
            hasvalidcontext = m_renderer->hasValidContext();			
        }

        // Create window and context if needed
        if (hasvalidcontext == false)
        {
            owncontext = true;
            win = new sad::Window();
            win->setRenderer(m_renderer);
            ctx = new sad::GLContext();
            // Try to create window and context
            if (win->create())
            {
                if (ctx->createFor(win))
                {
                    hasvalidcontext = true;
                }
            }
            // If we got here, we obviously can't do anything
            if (hasvalidcontext == false)
            {
                if (win->valid())
                    win->destroy();
                delete win;
                delete ctx;
                return;
            }
        }

        m_fetched = true;
        
        trySetString(m_version_string, GL_VERSION);
        trySetString(m_vendor, GL_VENDOR);
        trySetString(m_renderer_string, GL_RENDERER);
        trySetString(m_glsl_version_string, GL_SHADING_LANGUAGE_VERSION);
        trySetString(m_extensions, GL_EXTENSIONS);

        if (m_version_string.data())
        {
            sscanf(m_version_string.data(), "%d.%d", &m_major, &m_minor);
        }

        // Destroy allocated resources if needed
        if (owncontext)
        {
            ctx->destroy();
            win->destroy();
            delete ctx;
            delete win;
        }
    }
}

void sad::OpenGL::trySetString(sad::String & s, unsigned int e)
{
    const char * tmp = NULL;
    tmp = (const char *)glGetString((GLenum)e);
    if (tmp)
        s = tmp;
}

DECLARE_COMMON_TYPE(sad::OpenGL)
