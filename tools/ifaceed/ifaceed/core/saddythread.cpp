#include "core/saddythread.h"

#include "core/editor.h"
#include "core/synchronization.h"

#include <renderer.h>

core::SaddyThread::SaddyThread(core::Editor * editor) : m_editor(editor)
{
    
}

void core::SaddyThread::run() 
{
    unsigned int window_width = core::SaddyThread::tryParseWidthOrHeight(
        this->m_editor->parsedArgs()->single("width"),
        WINDOW_WIDTH,
        MINIMUM_WINDOW_WIDTH,
        MAXIMUM_WINDOW_WIDTH
    );
    unsigned int window_height = core::SaddyThread::tryParseWidthOrHeight(
        this->m_editor->parsedArgs()->single("height"),
        WINDOW_HEIGHT,
        MINIMUM_WINDOW_HEIGHT,
        MAXIMUM_WINDOW_HEIGHT
    );
    sad::Renderer::ref()->init(sad::Settings(window_width, window_height, false));
    sad::Renderer::ref()->setWindowTitle("Saddy Interface Editor");
    sad::Renderer::ref()->makeFixedSize();
    // Clear scenes
    sad::Renderer::ref()->clear();

    this->m_editor->synchronization()->awakeMainThread();

    this->m_editor->runSaddyEventLoop();
}

unsigned int core::SaddyThread::tryParseWidthOrHeight(
    const sad::Maybe<sad::String> & maybe_value,
    unsigned int default_value,
    unsigned int min_value,
    unsigned int max_value
)
{
    unsigned int result = default_value;
    if (maybe_value.exists())
    {
        bool ok = sscanf(maybe_value.value().c_str(), "%u", &result) == 1;
        if (ok)
        {
            if (result < min_value || result > max_value)
            {
                result = default_value;
            }
        }
        else
        {
            result = default_value;
        }
    }
    return result;
}
