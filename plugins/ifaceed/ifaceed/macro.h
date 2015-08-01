/*! \file macro.h
	

	A definitions for presetted defaults and other macro are stored here.
 */
#pragma once
/*! A default width for a saddy renderer's window
 */
#define WINDOW_WIDTH 800
/*! A default height for a saddy renderer's window
 */
#define WINDOW_HEIGHT 600
/*! A minimal window width is taken old resolution, which saw in some console games in emulators
 */
#define MINIMUM_WINDOW_WIDTH 320
/*! A maximal window width is taken from resolution of UHDTV (Progressive scan)
 */
#define MAXIMUM_WINDOW_WIDTH 15360
/*! A minimal window height is taken old resolution, which saw in some console games in emulators
 */
#define MINIMUM_WINDOW_HEIGHT 240
/*! A maximal window width is taken from resolution of UHDTV (Progressive scan)
 */
#define MAXIMUM_WINDOW_HEIGHT 8640
/** Time, all rotation wheel events within can be counted as one (in milliseconds)
 */
#define MAX_ROTATION_TIME 1500
/** Rotation angle step
 */
#define ROTATION_ANGLE_STEP 0.07
/**	A resource file, where icons are placed.
	Must be loaded to "icons" tree of renderer in core::Editor::start()
 */
#define ICONS_PATH  "resources/resources.json"
