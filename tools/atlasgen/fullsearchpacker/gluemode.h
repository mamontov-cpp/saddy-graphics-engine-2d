/*! \file gluemode.h
    

    Describes, how images should be merged
 */
#pragma once

namespace fullsearchpacker 
{

enum class GlueMode: int
{
    /*! Images should be merged horizontally
     */
    HORIZONTAL = 1,
    /*! Images should be merged vertically
     */
    VERTICAL = 2
};

}
