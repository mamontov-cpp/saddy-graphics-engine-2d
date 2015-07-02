/*! \file animationspoint2dsetter.h
	\author HiddenSeeker

	Defines a generic setter for setting a property of animations, which is stored as sad::Point2D
    and displayed, as two QDoubleSpinBox
 */
#pragma once
#include "../abstractsetter.h"

#include "../scripting.h"

#include "../../core/editor.h"


namespace scripting
{
	
namespace animations
{

/*! A common setter for setting a property of animation, which is stored as sad::Point2D and
    displayed as two QDoubleSpinBox
 */
template<
	typename _AnimationType,
	typename _CommandType
>
class Point2DSetter: public scripting::AbstractSetter<_AnimationType*,sad::Point2D>
{
public:
    /*! Construct new setter for property
		\param[in] e engine
		\param[in] name a name for property
        \param[in] xwidget a widget for storing x coordinate
		\param[in] ywidget a widget for storing y coordinate
	 */
	Point2DSetter(
		QScriptEngine* e,
		const QString& name,
        QDoubleSpinBox* xwidget,
		QDoubleSpinBox* ywidget
	) : scripting::AbstractSetter<_AnimationType*,sad::Point2D>(e, "set"), m_xwidget(xwidget), m_ywidget(ywidget)
	{
		if (name.length()) {
			this->addMatched(name);
		}
	}
    /*! Could be inherited
	 */ 
	virtual ~Point2DSetter()
	{
		
	}

	/*! Performs actually setting property
		\param[in] obj an object to be set
		\param[in] propertyname a property for object
		\param[in] oldvalue old value 
		\param[in] newvalue new value
	 */
	virtual void setProperty(_AnimationType* obj, const sad::String& propertyname, sad::Point2D oldvalue,  sad::Point2D newvalue)
	{
		QScriptValue main = this->engine()->globalObject().property("---");
		scripting::Scripting* e = static_cast<scripting::Scripting*>(main.toQObject());
		core::Editor* editor =  e->panel()->editor();

		history::Command* c =  new _CommandType(obj, propertyname, oldvalue, newvalue, m_xwidget, m_ywidget);							
		editor->currentBatchCommand()->add(c);
		c->commit(editor);
	}
protected:
    /*! A widget for displaying x coordinate
     */
    QDoubleSpinBox* m_xwidget;
    /*! A widget for displaying y coordinate
     */
    QDoubleSpinBox* m_ywidget;
};

}

}
