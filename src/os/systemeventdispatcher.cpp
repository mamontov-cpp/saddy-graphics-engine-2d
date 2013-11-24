#include "os/systemeventdispatcher.h"
#include <cstring>

sad::os::SystemEventDispatcher::SystemEventDispatcher()
{
	m_renderer = NULL;
}

void sad::os::SystemEventDispatcher::setRenderer(sad::Renderer * renderer)
{
	m_renderer = renderer;
}

sad::Renderer * sad::os::SystemEventDispatcher::renderer() const
{
	return m_renderer;
}

