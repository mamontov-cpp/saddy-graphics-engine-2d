/*! \file opticksupport.h
 *
 *  A support for Optick profiler
 */
#pragma once

#ifdef _USE_OPTICK
#include <optick.h>

#define PROFILER_EVENT OPTICK_EVENT()

#define PROFILER_MAIN_LOOP OPTICK_FRAME("MainThread", Optick::FrameType::CPU)

#else

#define PROFILER_EVENT  // NOLINT(clang-diagnostic-unused-macros)

#define PROFILER_MAIN_LOOP  // NOLINT(clang-diagnostic-unused-macros)

#endif
