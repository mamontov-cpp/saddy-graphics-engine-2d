#include "rotationprocess.h"

#include "../../history/history.h"

#include "../../history/scenenodes/scenenodeschangeangle.h"

gui::RotationProcess::RotationProcess()
: m_node(NULL), m_history(NULL), m_pending(false)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerExpired()));
}

gui::RotationProcess::~RotationProcess()
{
	
}

void gui::RotationProcess::start(
    sad::SceneNode* node,
    float oldvalue,
    float newvalue
)
{
    if (m_pending == false)
    {
        m_node = node;
        m_oldvalue = oldvalue;
        m_newvalue = newvalue;
        m_timer.setInterval(gui::RotationProcess::TIMEOUT);
        m_timer.setSingleShot(true);
        m_pending = true;
        m_timer.start();
    }
    else
    {
        if (m_node != node)
        {
            // Commit old changes
            this->timerExpired();
            // Restart process
            this->start(node, oldvalue, newvalue);
        }
        else
        {
            // Update new value
            m_newvalue = newvalue;

            // Restart timer
            m_timer.stop();
            m_timer.setInterval(gui::RotationProcess::TIMEOUT);
            m_timer.setSingleShot(true);
            m_timer.start();
        }
    }
}

void gui::RotationProcess::setHistory(history::History* h)
{
    m_history = h;
}

const int gui::RotationProcess::TIMEOUT = 800;

// ========================== PUBLIC SLOTS METHODS ==========================

void gui::RotationProcess::timerExpired()
{
    if (m_history)
    {
        m_history->add(new history::scenenodes::ChangeAngle(m_node, m_oldvalue, m_newvalue));
        m_timer.stop();
        m_pending = false;
    }
}
