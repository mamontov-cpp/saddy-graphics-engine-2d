#include "layouts/serializablecell.h"

DECLARE_COMMON_TYPE(sad::layouts::SerializableCell);

sad::layouts::SerializableCell::SerializableCell()
: RowSpan(1),
ColSpan(1), 
VAlign(sad::layouts::LVA_Middle), 
HAlign(sad::layouts::LHA_Middle),
StackingType(sad::layouts::LST_Horizontal),
PaddingTop(0),
PaddingBottom(0),
PaddingLeft(0),
PaddingRight(0),
Row(0),
Col(0)
{
    
}

