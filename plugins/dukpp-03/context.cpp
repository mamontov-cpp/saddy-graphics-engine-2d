#include "dukpp-03/context.h"

#include <renderer.h>
#include <util/fs.h>

sad::dukpp03::Context::Context() : m_renderer(NULL)
{
    
}

bool sad::dukpp03::Context::evalFromFile(
    const std::string& path, 
    bool clean_heap,
    std::string* error
)
{
    // Test if file exists
    sad::String mpath = path.c_str();
    if (sad::util::fileExists(mpath.c_str()) == false)
    {
        sad::Renderer* r = m_renderer;
        if (!r)
        {
            r = sad::Renderer::ref();
        }
        if (sad::util::isAbsolutePath(mpath) == false)
        {
             mpath = util::concatPaths(r->executablePath(), mpath);
             if (sad::util::fileExists(mpath.c_str()) == false)
             {
                if (error) 
                {
                    *error = "File \"";
                    *error += path;
                    *error += "\" does not exist";
                }
                return false;
             }
        }
        else
        {
            if (error) 
            {
                *error = "File \"";
                *error += path;
                *error += "\" does not exist";
            }
            return false;
        }
    }
    m_running = true;
    startEvaluating();
    bool result = false;
    if (duk_peval_file(m_context, mpath.c_str()) != 0) 
    {
        if (error)
        {
            *error = duk_safe_to_string(m_context, -1);
        }
        duk_pop(m_context);
    } 
    else 
    {
        if (error)
        {
            *error = "";
        }
        result = true;
        if (clean_heap)
        {
            duk_pop(m_context);
        }
    }
    m_running = false;
    return result;
    
}

void sad::dukpp03::Context::setRenderer(sad::Renderer* r)
{
    m_renderer = r;
}

sad::Renderer* sad::dukpp03::Context::renderer() const
{
    return m_renderer;
}

sad::dukpp03::Context::~Context()
{
    
}
