#include "input/handlers.h"
#include "opticksupport.h"

sad::input::AbstractHandler::~AbstractHandler()
{
    PROFILER_EVENT;

}

void sad::input::VoidFreeFunctionHandler::invoke(
    const sad::input::AbstractEvent & e
)
{
    PROFILER_EVENT;
    m_f();
}

void sad::input::VoidStdFunctionHandler::invoke(
    const sad::input::AbstractEvent & e
)
{
    PROFILER_EVENT;
    m_f();
}
