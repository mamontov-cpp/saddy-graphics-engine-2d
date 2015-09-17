#include "duktape/duktapefunction.h"
#include "duktape/context.h"

sad::duktape::VoidFunction0::VoidFunction0(Function f) : m_callee(f)
{
    
}

sad::duktape::DuktapeCallable* sad::duktape::VoidFunction0::clone()
{
    return new sad::duktape::VoidFunction0(m_callee);
}

int sad::duktape::VoidFunction0::requiredArguments()
{
    return 0;
}

int sad::duktape::VoidFunction0::call(sad::duktape::Context* c)
{
    if (c->getTop() != 0)
    {
        c->throwError(sad::String("Function receives 0 arguments: ") + sad::String::number(c->getTop()) + " given");
        return 0;
    }

    try
    {
        m_callee();
    }
    catch(...)
    {
        c->throwError("Caught exception while calling function");
        return 0;
    }
    return 0;
}

sad::duktape::VoidFunction0::~VoidFunction0()
{
    
}

