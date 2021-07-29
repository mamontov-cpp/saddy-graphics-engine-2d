#include "layouts/lengthvalue.h"

sad::layouts::LengthValue::LengthValue()
: Unit(sad::layouts::Unit::LU_Pixels), Value(0)
{
    
}

sad::layouts::LengthValue::LengthValue(sad::layouts::Unit unit, double value)
: Unit(unit), Value(value) 
{
    
}
