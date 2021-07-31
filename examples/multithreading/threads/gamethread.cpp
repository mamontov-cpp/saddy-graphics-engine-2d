#include "gamethread.h"

#include <sadsleep.h>

#include <freetype/font.h>

#include <log/consoletarget.h>
#include <log/filetarget.h>

#include <pipeline/pipeline.h>

#include <util/free.h>

#include <irrklang/sound.h>


threads::GameThread::GameThread() : m_state(threads::GameState::GS_UNITIALIZED), m_killed(false), m_need_to_wait(false), m_renderer_started(false)
{
    m_renderer = new sad::Renderer();
}

threads::GameThread::~GameThread()
{
    delete m_renderer;
}

void threads::GameThread::needsToBeNotifiedFromLater()
{
    m_need_to_wait_lock.lock();
    m_need_to_wait = true;
    m_need_to_wait_lock.unlock();
}

void threads::GameThread::waitForNotify()
{
    bool need_to_wait = true;
    while (need_to_wait)
    {
        sad::sleep(100);
        m_need_to_wait_lock.lock();
        need_to_wait = m_need_to_wait;
        m_need_to_wait_lock.unlock();
    }
}

void threads::GameThread::notify()
{
    m_need_to_wait_lock.lock();
    m_need_to_wait = false;
    m_need_to_wait_lock.unlock();
}

bool threads::GameThread::hasErrors() const
{
    return m_error.exists();
}

const sad::String& threads::GameThread::errors() const
{
    return m_error.value();
}

bool threads::GameThread::isKilled() const
{
    return m_killed;
}

void threads::GameThread::tryInitialize(
    const sad::String& log_prefix,
    const sad::String& log_name,
    const sad::String& config_name,
    const sad::String& window_title
)
{
    /*! Create and set scene for renderer;
     */
    sad::Scene * scene = new sad::Scene();
    m_renderer->setScene(scene);
    m_renderer->tree()->factory()->registerResource<sad::freetype::Font>();
    m_renderer->tree()->factory()->registerResource<sad::irrklang::Sound>();
    m_renderer->tree()->factory()->registerDefaultFileTypeFor<sad::irrklang::Sound>();

    sad::String console_format = "{0},";
    console_format += log_prefix;
    console_format += ": [{1}] {3}{2}{4}";
    sad::log::ConsoleTarget* ct = new sad::log::ConsoleTarget(console_format);
    ct->setColorForPriorityAndColoredOutput(sad::log::Priority::DEBUG, sad::log::Color::WHITE);
    m_renderer->log()->addTarget(ct);

    /* Setup the logging. We redirect all messages to a file, passed as parameter to thread
       variable
     */
    sad::log::FileTarget* fl = new sad::log::FileTarget();
    bool b = fl->open(log_name.c_str());
    if (!b)
    {
        sad::String error_result = "Failed to open log file \"";
        error_result += log_name;
        error_result += "\"";
        SL_LOCAL_DEBUG(error_result, *m_renderer);
        m_error.setValue(error_result);
        return;
    }
    m_renderer->log()->addTarget(fl);

    /* Create 800x600 window in windowed mode and toggle a fixed size of window
     */
    m_renderer->init(sad::Settings(800,600, false));
    m_renderer->makeFixedSize();



    /* Bind built-ing scene to renderer
     */
    scene->setRenderer(m_renderer);

    /*! Load resources
     */
    sad::Vector<sad::resource::Error *> errors = m_renderer->loadResources(config_name);
    if (!errors.empty())
    {
        sad::String error_result = sad::resource::format(errors);
        SL_LOCAL_FATAL(error_result, *m_renderer);
        sad::util::free(errors);
        m_error.setValue(error_result);
    }

    m_renderer->setWindowTitle(window_title);
}

void threads::GameThread::sendKillSignalFrom(threads::GameThread* other)
{
    if (m_renderer_started) {
        sad::Renderer* renderer = m_renderer;
        m_renderer->pipeline()->appendTask([renderer]() -> void {
            renderer->quit();
        });
    } else {
        m_killed = true;
        other->notify();
    }
}


void threads::GameThread::sendResumeSignalFrom(threads::GameThread* other)
{
    if (m_renderer_started) {
        // DO NOTHING
    } else {
        m_killed = false;
        other->notify();
    }
}

sad::Renderer* threads::GameThread::renderer() const
{
    return m_renderer;
}

void threads::GameThread::markAsRendererStarted()
{
    m_renderer_started = true;
}

void threads::GameThread::setState(threads::GameState state)
{
    m_state = state;
}

threads::GameState threads::GameThread::state() const
{
    return m_state;
}
