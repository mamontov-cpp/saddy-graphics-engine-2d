/*! \file tpunitw.h
	\author HiddenSeeker

	A cleaner version of including tpunit++, since there were a lot of overhead
	for it, due warnings in C++ or printf declaration
 */
#pragma once

#pragma warning(push)
#pragma warning(disable: 4273)
#pragma warning(disable: 4351)
#include <cstdio>
#include "tpunit++.hpp"
#pragma warning(pop)
