#include "os/systemeventdispatcher.h"
#include "os/keydecoder.h"
#include <cstring>

sad::os::SystemEventDispatcher::SystemEventDispatcher()
: m_renderer(NULL), m_keys(new sad::os::KeyDecoder())
{

}

sad::os::SystemEventDispatcher::~SystemEventDispatcher()
{
	delete m_keys;
}

void sad::os::SystemEventDispatcher::setRenderer(sad::Renderer * renderer)
{
	m_renderer = renderer;
}

sad::Renderer * sad::os::SystemEventDispatcher::renderer() const
{
	return m_renderer;
}

