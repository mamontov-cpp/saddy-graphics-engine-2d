#include "input/handlers.h"

sad::input::AbstractHandler::~AbstractHandler()
{

}

void sad::input::VoidFreeFunctionHandler::invoke(
	const sad::input::AbstractEvent & e
)
{
	m_f();
}

