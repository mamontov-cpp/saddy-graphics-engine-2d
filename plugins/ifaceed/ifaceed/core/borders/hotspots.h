/*! \file hotspots.h
	\author HideenSeeker

	Describes hotspots enumeration for rendering special buttons near borders
 */
#pragma once

namespace core
{
	
namespace borders
{
	
/*! Describes hotspots enumeration for rendering special buttons near borders
 */ 
enum HotSpots
{	
	HS_LEFT = 0,    //!< Left resize object button
	HS_RIGHT = 1,   //!< Right resize object button
	HS_TOP = 2,     //!< Top resize object button
	HS_BOTTOM = 3,  //!< Bottom resize object button
	HS_REMOVE = 4	//!< A remove item button
};

}

}
