#include <sadthreadexecutablefunction.h>

sad::AbsractThreadExecutableFunction::~AbsractThreadExecutableFunction()
{

}

sad::AbsractThreadExecutableFunction * sad::EmptyThreadExecutableFunction::clone() const
{
	return new sad::EmptyThreadExecutableFunction();
}

int sad::EmptyThreadExecutableFunction::execute()
{
	return 0;
}


int sad::FreeZeroArgVoidExecutableFunction::execute()
{
	m_f();
	return 0;
}

sad::AbsractThreadExecutableFunction * sad::FreeZeroArgVoidExecutableFunction::clone() const
{
	return new sad::FreeZeroArgVoidExecutableFunction(*this);
}

int sad::FreeZeroArgIntExecutableFunction::execute()
{
	return m_f();
}

sad::AbsractThreadExecutableFunction * sad::FreeZeroArgIntExecutableFunction::clone() const
{
	return new sad::FreeZeroArgIntExecutableFunction(*this);
}
