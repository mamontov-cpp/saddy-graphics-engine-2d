#ifdef _MSC_VER
    #pragma warning(disable: 4506)
#endif

#include "rotationprocess.h"

#include "../closuremethodcall.h"

#include "../history/history.h"

#include "../history/scenenodes/scenenodeschangeangle.h"


// ============================ PUBLIC METHODS ============================

gui::RotationProcess::RotationProcess()
: m_pending(false), m_editor(nullptr), m_node(nullptr)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerExpired()));
}

gui::RotationProcess::~RotationProcess()
{
    
}

void gui::RotationProcess::start(
    sad::SceneNode* node,
    float old_value,
    float new_value
)
{
    if (m_pending == false)
    {
        m_node = node;
        m_old_value = old_value;
        m_new_value = new_value;

        m_editor->emitClosure(bind(this, &gui::RotationProcess::setTimeoutAndStart));
    }
    else
    {
        if (m_node != node)
        {
            // Commit old changes
            this->timerExpired();
            // Restart process
            this->start(node, old_value, new_value);
        }
        else
        {
            // Update new value
            m_new_value = new_value;

            // Restart timer
             m_editor->emitClosure(bind(this, &gui::RotationProcess::restart));
        }
    }
}

void gui::RotationProcess::setEditor(core::Editor* e)
{
    m_editor = e;
}

const int gui::RotationProcess::TIMEOUT = 800;

// ========================== PUBLIC SLOTS METHODS ==========================

void gui::RotationProcess::timerExpired()
{
    if (m_editor)
    {
        m_editor->history()->add(new history::scenenodes::ChangeAngle(m_node, m_old_value, m_new_value));
        stopLocalTimer();
        m_pending = false;
    }
}

// ========================== PROTECTED METHODS ==============================


void  gui::RotationProcess::restart()
{
    m_timer.stop();
    m_timer.setInterval(gui::RotationProcess::TIMEOUT);
    m_timer.setSingleShot(true);
    m_timer.start();
}

void gui::RotationProcess::setTimeoutAndStart()
{
    m_timer.setInterval(gui::RotationProcess::TIMEOUT);
    m_timer.setSingleShot(true);
    m_pending = true;
    m_timer.start();
}

void  gui::RotationProcess::startLocalTimer()
{
    m_editor->emitClosure( bind(this, &gui::RotationProcess::startLocalTimerNow) );
}

void gui::RotationProcess::startLocalTimerNow()
{
    m_timer.start();
}

void  gui::RotationProcess::stopLocalTimer()
{
    m_editor->emitClosure( bind(&m_timer, &QTimer::stop) );
}
