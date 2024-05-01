#include "layouts/lengthvalue.h"
#include "opticksupport.h"

sad::layouts::LengthValue::LengthValue()
: Unit(sad::layouts::Unit::LU_Pixels), Value(0)
{
    PROFILER_EVENT;
    
}

sad::layouts::LengthValue::LengthValue(sad::layouts::Unit unit, double value)
: Unit(unit), Value(value) 
{
    PROFILER_EVENT;
    
}
