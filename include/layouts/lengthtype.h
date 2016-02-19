/*! \file lengthtype.h
	
	Describes a type of values, which is used to describe, how width or height of cell is measured
 */
#pragma once 

namespace sad
{

namespace layouts
{

/*! Describes a type of values, which is used to describe, how width or height of cell is measured
 */ 
enum LengthType
{
/*! Width or height should be computed automatically
 */	
LLT_Auto = 0,
/*! Width or height should be computed requested as specified size in pixels
 */
LLT_Pixels = 1,
/*! Width or height should be computed requested as specified size in percents to a grid
 */
LLT_Percents = 2	
};

}	
	
}
