#include "input/controls.h"

sad::input::Controls::Controls()
: m_wheel_tick_sensitivity(1), m_double_click_sensitivity(500)
{

}


sad::input::Controls::~Controls()
{
    this->performQueuedActions();
    this->clearNow();
}

sad::input::AbstractHandler* sad::input::Controls::add(
    const sad::input::HandlerTypeAndConditions & tac, 
    sad::input::AbstractHandler * h
)
{
    this->ControlsImmutableBase::add(sad::Pair<
            sad::input::HandlerTypeAndConditions, 
            sad::input::AbstractHandler*
        >(tac, h)		
    );
    return h;
}

void sad::input::Controls::postEvent(EventType type, const sad::input::AbstractEvent & e)
{
    HandlerList & handlerforevents = m_handlers[(unsigned int)(type)];
    for(unsigned int i = 0; i < handlerforevents.count(); i++)
    {
        sad::input::Controls::tryInvokeHandler(handlerforevents[i], e);
    }
}


void sad::input::Controls::startReceivingEvents()
{
    performQueuedActions();
    lockChanges();
}

void sad::input::Controls::finishReceivingEvents()
{
    unlockChanges();
    performQueuedActions();
}

void sad::input::Controls::setWheelTickSensitivity(double delta)
{
    m_wheel_tick_sensitivity = delta;
}

double sad::input::Controls::wheelTickSensitivity() const
{
    return m_wheel_tick_sensitivity;
}

void sad::input::Controls::setDoubleClickSensitivity(double sensitivity)
{
    m_double_click_sensitivity = sensitivity;
}

double sad::input::Controls::doubleClickSensitivity() const
{
    return m_double_click_sensitivity;
}

void sad::input::Controls::clearNow()
{
    for(unsigned int i = 0; i < SAD_INPUT_EVENT_TYPE_COUNT; i++) 
    {
        for(unsigned int j = 0; j < m_handlers[i].size(); j++) 
        {
            sad::input::Controls::freeHandlerAndConditions(m_handlers[i][j]);
        }
        m_handlers[i].clear();
    }
}

void sad::input::Controls::addNow(
        sad::Pair<
            sad::input::HandlerTypeAndConditions, 
            sad::input::AbstractHandler*
        > o
)
{
    // Get list by type name
    HandlerList & handlerforevents = m_handlers[(unsigned int)(o.p1().p1())];
    sad::input::Controls::HandlerAndConditions a;
    // Copy conditions
    a.set1(o.p1().p2());
    // Copy handler
    a.set2(o.p2());
    handlerforevents << a;
}


void sad::input::Controls::removeNow(sad::input::AbstractHandler* o)
{
    for(unsigned int i = 0; i < SAD_INPUT_EVENT_TYPE_COUNT; i++) 
    {
        for(unsigned int j = 0; j < m_handlers[i].size(); j++) 
        {
            if (m_handlers[i][j].p2() == o)
            {
                sad::input::Controls::freeHandlerAndConditions(m_handlers[i][j]);
                m_handlers[i].removeAt(j);
                --j;
            }
        }
    }
}




void sad::input::Controls::freeHandlerAndConditions(sad::input::Controls::HandlerAndConditions & o)
{
    delete o.p2();
    o.set2(nullptr);
    for(unsigned int i = 0; i < o._1().size(); i++) 
    {
        delete o._1()[i];
    }
    o._1().clear();
}

void sad::input::Controls::tryInvokeHandler(const HandlerAndConditions & o, const sad::input::AbstractEvent & e)
{
    bool conditionaremet = true;
    for(unsigned int i = 0; i < o.p1().size(); i++) 
    {
        conditionaremet = conditionaremet && o.p1()[i]->check(e);
    }
    if (conditionaremet)
    {
        o.p2()->invoke(e);
    }
}
