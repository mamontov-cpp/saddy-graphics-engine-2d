#include <sadthreadexecutablefunction.h>

sad::AbsractThreadExecutableFunction::~AbsractThreadExecutableFunction()
{

}

sad::AbsractThreadExecutableFunction * sad::util::EmptyThreadExecutableFunction::clone() const
{
	return new sad::util::EmptyThreadExecutableFunction();
}

int sad::util::EmptyThreadExecutableFunction::execute()
{
	return 0;
}


int sad::util::FreeZeroArgVoidExecutableFunction::execute()
{
	m_f();
	return 0;
}

sad::AbsractThreadExecutableFunction * sad::util::FreeZeroArgVoidExecutableFunction::clone() const
{
	return new sad::util::FreeZeroArgVoidExecutableFunction(*this);
}

int sad::util::FreeZeroArgIntExecutableFunction::execute()
{
	return m_f();
}

sad::AbsractThreadExecutableFunction * sad::util::FreeZeroArgIntExecutableFunction::clone() const
{
	return new sad::util::FreeZeroArgIntExecutableFunction(*this);
}
