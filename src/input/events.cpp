#include "input/events.h"

// A maximal value of enum element for arrays
const int sad::input::AbstractEvent::EventTypeCount = 13;

sad::input::AbstractEvent::~AbstractEvent()
{

}

sad::input::NullEvent::NullEvent()
{

}

sad::input::NullEvent::~NullEvent()
{

}

sad::input::KeyEvent::~KeyEvent()
{

}

sad::input::MouseCursorEvent::~MouseCursorEvent()
{

}

sad::input::MouseEvent::~MouseEvent()
{

}

sad::input::WheelEvent::~WheelEvent()
{

}

sad::input::ResizeEvent::~ResizeEvent()
{

}
