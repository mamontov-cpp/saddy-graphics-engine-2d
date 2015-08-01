/*! \file ../include/sadthreadexecutablefunction.h
	

	Defines a simple function, which can be executed in thread
 */
#pragma once

namespace sad
{

/*! A basic interface, which can be executed in thread
 */
class AbsractThreadExecutableFunction
{
public:
	/*! Executes a function in thread
		\return code, which will be returned in thread
	 */
	virtual int execute() = 0;
	/*! Cretes a clone of executable function
		\returns exact copy of current thread executable function
	 */
	virtual AbsractThreadExecutableFunction * clone() const = 0;
	/*! Kept for purpose of inheritance
	 */
	virtual ~AbsractThreadExecutableFunction();
};

}

#include "util/sadthreadexecutablefunction.h"
