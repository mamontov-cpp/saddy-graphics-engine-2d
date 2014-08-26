#include "state.h"

#include "../../core/editor.h"
#include "../../core/editorbehaviour.h"
#include "../../core/shared.h"

#include <unused.h>

core::State::State()
{
	m_behaviour = NULL;
}

core::State::~State()
{
}

void core::State::enter()
{
}

void core::State::leave()
{
}

void core::State::onMouseMove(UNUSED const sad::input::MouseMoveEvent & ev)
{

}

void core::State::onMouseDown(UNUSED const sad::input::MousePressEvent & ev)
{

}

void core::State::onMouseUp(UNUSED const sad::input::MouseReleaseEvent & ev)
{

}

void core::State::onWheel(UNUSED const sad::input::MouseWheelEvent & ev)
{

}


void core::State::onKeyDown(UNUSED const sad::input::KeyPressEvent & ev)
{

}

void core::State::onKeyUp(UNUSED const sad::input::KeyReleaseEvent & ev)
{

}

core::Editor* core::State::editor()
{
	return static_cast<core::Editor *>(this->behaviour()->parent());
}

core::Shared* core::State::shdata()
{
	return this->editor()->shdata();
}
