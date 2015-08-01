/*! \file tpunitw.h
	

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

/*! A macro, which runs all registered tests
 */ 
#define MAIN_RUNS_ALL_REGISTERED_TESTS     \
	int main(int argc, char ** argv)       \
    {									   \
        int result = tpunit::Tests::Run(); \
        return 0;                          \
    }
