#include "layouts/serializablecell.h"
#include "opticksupport.h"

DECLARE_COMMON_TYPE(sad::layouts::SerializableCell);

sad::layouts::SerializableCell::SerializableCell()
: RowSpan(1),
ColSpan(1), 
VAlign(sad::layouts::VerticalAlignment::LVA_Middle), 
HAlign(sad::layouts::HorizontalAlignment::LHA_Middle),
StackingType(sad::layouts::StackingType::LST_Horizontal),
PaddingTop(0),
PaddingBottom(0),
PaddingLeft(0),
PaddingRight(0),
Row(0),
Col(0)
{
    PROFILER_EVENT;
    
}
