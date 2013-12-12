#include "input/controls.h"

sad::input::Controls::Controls()
{

}

sad::input::Controls::~Controls()
{
	this->performQueuedActions();
	for(unsigned int i = 0; i < SAD_INPUT_EVENTTYPE_COUNT; i++) 
	{
		for(unsigned int j = 0; j < m_handlers[i].size(); j++) 
		{
			sad::input::Controls::freeHandlerAndConditions(m_handlers[i][j]);
		}
		m_handlers[i].clear();
	}
}

void sad::input::Controls::add(
	const sad::input::HandlerTypeAndConditions & tac, 
	sad::input::AbstractHandler * h
)
{
	HandlerList & handlerforevents = m_handlers[(unsigned int)(tac.p1())];
	sad::input::Controls::HandlerAndConditions a;
	a.set1(tac.p2());
	a.set2(h);
	handlerforevents << a;
}

void sad::input::Controls::postEvent(EventType type, const sad::input::AbstractEvent & e)
{
	HandlerList & handlerforevents = m_handlers[(unsigned int)(type)];
	for(unsigned int i = 0; i < handlerforevents.count(); i++)
	{
		sad::input::Controls::tryInvokeHandler(handlerforevents[i], e);
	}
}


void sad::input::Controls::freeHandlerAndConditions(sad::input::Controls::HandlerAndConditions & o)
{
	delete o.p2();
	o.set2(NULL);
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
