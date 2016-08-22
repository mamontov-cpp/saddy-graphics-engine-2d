#include "scriptablelengthvalue.h"

#include "../fromvalue.h"
#include "../tovalue.h"

#include "../scripting.h"


scripting::layouts::ScriptableLengthValue::ScriptableLengthValue(sad::layouts::Unit unit, double value, scripting::Scripting* s)
: m_value(sad::layouts::LengthValue(unit, value)), m_scripting(s)
{
    
}

scripting::layouts::ScriptableLengthValue::ScriptableLengthValue(const sad::layouts::LengthValue& value, scripting::Scripting* s)
: m_value(value), m_scripting(s)
{
    
}

sad::layouts::LengthValue scripting::layouts::ScriptableLengthValue::toValue() const
{
    sad::layouts::LengthValue value = m_value;
    if (value.Unit == sad::layouts::LU_Pixels)
    {
        if (value.Value < 0)
        {
            value.Value = 0;
        }
    }

    if (value.Unit == sad::layouts::LU_Percents)
    {
        if (value.Value < 0)
        {
            value.Value = 0;
        }

        if (value.Value > 100)
        {
            value.Value = 100;
        }
    }
    
    return value;
}

QString scripting::layouts::ScriptableLengthValue::toString() const
{
    QString unit;
    switch(m_value.Unit)
    {
    case sad::layouts::LU_Auto:      unit = "sad::layouts::LU_Auto"; break;
    case sad::layouts::LU_Percents:  unit = "sad::layouts::LU_Percents"; break;
    case sad::layouts::LU_Pixels:    unit = "sad::layouts::LU_Pixels"; break;
    };
    return QString("ScriptableLengthValue(%1, %2)").arg(unit).arg(m_value.Value);
}

scripting::layouts::ScriptableLengthValue::~ScriptableLengthValue()
{
    
}

double scripting::layouts::ScriptableLengthValue::value() const
{
    return m_value.Value;
}

void scripting::layouts::ScriptableLengthValue::setValue(double v)
{
    m_value.Value = v;
}

QScriptValue scripting::layouts::ScriptableLengthValue::unit() const
{
    return scripting::FromValue<sad::layouts::Unit>::perform(m_value.Unit, m_scripting->engine());
}

void scripting::layouts::ScriptableLengthValue::setUnit(QScriptValue v)
{
    sad::Maybe<sad::layouts::Unit> mu_maybe = scripting::ToValue<sad::layouts::Unit>::perform(v);
    if (mu_maybe.exists())
    {
        m_value.Unit = mu_maybe.value();
    }
    else
    {
        m_scripting->engine()->currentContext()->throwError("ScriptableLengthValue::setUnit(): argument is not valid unit");
    }
}
