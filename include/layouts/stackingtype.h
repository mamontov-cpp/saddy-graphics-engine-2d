/*! \file stackingtype.h
    
    Describes, how should inner objects be stacked in cell
 */
#pragma once 

namespace sad
{

namespace layouts
{

/*! Describes, how inner objects should be stacked in cell
 */ 
enum class StackingType: int
{
/*! Forces objects to be stacked horizontally
 */	
LST_Horizontal = 0,
/*! Forces objects to be stacked vertically
 */
LST_Vertical = 1,
/*! Forces objects to be placed on top of each other, according to 
    cell alignment
 */
LST_NoStacking = 2
};

}	
    
}
