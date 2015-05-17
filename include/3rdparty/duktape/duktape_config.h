/*! \file duktape_config.h
	\author HiddenSeeker

	Defines a built-in timeout checker for sandboxing Duktape
 */
#pragma once

#ifndef DUK_OPT_INTERRUPT_COUNTER
	#define DUK_OPT_INTERRUPT_COUNTER 1
#endif

#ifndef DUK_OPT_EXEC_TIMEOUT_CHECK

namespace sad
{

namespace duktape
{
	
/*! Checks for timeout of duktape
	\param[in] ptr pointer
	\return non-zero value if timeout is reached
 */
int ____check_timeout(void* ptr);

}

}

#define DUK_OPT_EXEC_TIMEOUT_CHECK(udata)  (sad::duktape::____check_timeout(udata))

#endif