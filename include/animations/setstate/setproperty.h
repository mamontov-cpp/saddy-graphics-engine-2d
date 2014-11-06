/*! \file animations/setstate/setproperty.h
	\author HiddenSeeker

	Defines a command, which sets property on an object
 */
#include "typedcommand.h"

#include "../../db/dbobject.h"

namespace sad
{

namespace animations
{

namespace setstate
{

/*! Defines a call command, as setting a property of object
 */
template<
	typename _Argument
>
class SetProperty : public sad::animations::setstate::TypedCommmand<_Argument>
{
public:
	/*! Constructs new empty cached call
		\param[in]  o object object, which method should be called upon
		\param[in]  prop a property name to be set
	 */
	inline SetProperty(sad::db::Object* o, const sad::String& prop) : m_o(o), m_property_name(prop) { }
	/*! Calls an animation delegate for specified argument
		\param[in] a argument
	 */
	virtual void call(const _Argument& a)
	{
		m_o->setProperty(m_property_name, a);
	}
	virtual ~SetProperty() { }
protected:
	/*! An object link for fast call
	 */
	sad::db::Object* m_o;
	/*! A method to be called on object
	 */
	sad::String m_property_name;
};

}

}

}
