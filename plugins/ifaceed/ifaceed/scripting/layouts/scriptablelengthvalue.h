/*! \file scripting/layouts/scriptablelengthvalue.h

    A grid length value scripting bindings should be placed here
 */
#pragma once
#include "../classwrapper.h"

#include <layouts/grid.h>

#include <QScriptValue>

namespace scripting
{

class Scripting;

namespace layouts
{

/*! A wrapper for a reference to a length value
 */
class ScriptableLengthValue: public scripting::ClassWrapper
{
    Q_OBJECT
public:
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
    /*! Converts object to string representation
        \return object to string
     */
    virtual QString toString() const;
    /*! A destructor for grid cell
     */
    virtual ~ScriptableLengthValue();
public slots:
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
	QScriptValue unit() const;
	/*! Sets unit for underlying size
		\param[in] v size
	 */
	void setUnit(QScriptValue v);
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

