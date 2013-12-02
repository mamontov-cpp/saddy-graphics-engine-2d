#include "input/controls.h"

sad::input::Controls::Controls()
{

}

void sad::input::Controls::postEvent(EventType type, const sad::input::AbstractEvent & e)
{
	HandlerList & handlerforevents = m_handlers[(unsigned int)(type)];
	for(unsigned int i = 0; i < handlerforevents.count(); i++)
	{
		sad::input::AbstractHandler * h = handlerforevents[i];
		h->invoke(e);
	}
}
