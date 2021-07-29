/*! \file verticalalignment.h
    
    Describes a types of vertical alignment of cell
 */
#pragma once 

namespace sad
{

namespace layouts
{

/*! Describes a types of vertical alignment of cell
 */ 
enum class VerticalAlignment: int
{
/*! Place objects on top of cell
 */	
LVA_Top = 0,
/*! Place objects in middle of cell
 */
LVA_Middle = 1,
/*! Place objects in bottom of cell
 */
LVA_Bottom = 2	
};

}	
    
}
