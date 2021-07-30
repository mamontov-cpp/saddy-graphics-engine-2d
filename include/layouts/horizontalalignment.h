/*! \file horizontalalignment.h
    
    Describes a types of horizontal alignment of cell
 */
#pragma once 

namespace sad
{

namespace layouts
{

/*! Describes a types of horizontal alignment of cell
 */ 
enum class HorizontalAlignment: int
{
/*! Left alignment
 */	
LHA_Left = 0,
/*! Alignment on middle
 */
LHA_Middle = 1,
/*! Right alignment
 */
LHA_Right = 2
};

}
    
}
