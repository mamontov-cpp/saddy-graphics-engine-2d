/*! \file scripting/layouts/scriptablelengthvalue.h

    A grid length value scripting bindings should be placed here
 */
#pragma once
#include <layouts/grid.h>

#include <QObject>
#include <QMetaType>

namespace scripting
{

class Scripting;

namespace layouts
{

/*! A wrapper for a reference to a length value
 */
class ScriptableLengthValue: public QObject
{
    Q_OBJECT
public:
    /*! A helper constructor for metatype
     */
    inline ScriptableLengthValue() : m_scripting(NULL)
    {

    }

    /*! A helper constructor for metatype
        \param[in] o value
     */
    inline ScriptableLengthValue(const ScriptableLengthValue& o) : m_value(o.m_value), m_scripting(o.m_scripting)
    {

    }

    /*! Initializes a length value
        \param[in] value a numeric value for scriptable length
        \param[in] unit a unit for value
        \param[in] s scripting object
     */
    ScriptableLengthValue(
        sad::layouts::Unit unit,
        double value,
        scripting::Scripting* s
    );
    /*! Initializes a length value
        \param[in] value a real underlying value
        \param[in] s scripting object
     */
    ScriptableLengthValue(
        const sad::layouts::LengthValue& value,
        scripting::Scripting* s
    );
    /*! Returns real underlying value, cropping it to be always valid
        in terms of cell size
        \return real value
     */
    sad::layouts::LengthValue toValue() const;
    /*! A destructor for grid cell
     */
    virtual ~ScriptableLengthValue();
public slots:
    /*! Converts object to string representation
        \return object to string
     */
    virtual QString toString() const;
    /*! Returns underlying length value size
     */
    double value() const;
    /*! Returns underlying length value size
        \return v new value
     */
    void setValue(double v);
    /*! Returns unit value
        \return unit value
     */
    int unit() const;
    /*! Sets unit for underlying size
        \param[in] v size
     */
    void setUnit(unsigned int v);
protected:
    /*! A real length value
     */
    sad::layouts::LengthValue m_value;
    /*! A main scripting object
     */
    scripting::Scripting* m_scripting;
};

}

}

