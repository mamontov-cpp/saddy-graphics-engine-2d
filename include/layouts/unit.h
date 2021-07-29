/*! \file unit.h
    
    Describes a type of values, which is used to describe, how width or height of cell is measured
 */
#pragma once 

namespace sad
{

namespace layouts
{

/*! Describes a type of values, which is used to describe, how width or height of cell is measured
 */ 
enum class Unit: int
{
/*! Width or height should be computed automatically
 */	
LU_Auto = 0,
/*! Width or height should be computed requested as specified size in pixels
 */
LU_Pixels = 1,
/*! Width or height should be computed requested as specified size in percents to a grid
 */
LU_Percents = 2	
};

}	
    
}
