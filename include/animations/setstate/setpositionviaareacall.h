/*! \file animations/setstate/setpositionviaareacall.h
	\author HiddenSeeker

	Sets area via method call
 */
#include "typedcommand.h"

#include "../../sadrect.h"
#include "../../geometry2d.h"

#include "../../db/dbobject.h"

namespace sad
{

namespace animations
{

namespace setstate
{

/*! Defines a call command, Defines a call command, as setting position via getting area by method call and shifting it to center
 */
template<
	typename _Object
>
class SetPositionViaMethodCall : public sad::animations::setstate::TypedCommmand<sad::Point2D>
{
public:
	/*! Constructs new empty cached call
		\param[in]  o object object, which method should be called upon
	 */
	inline SetPositionViaMethodCall(sad::db::Object* o) : m_o(o) {}
	/*! Could be inherited
	 */
	virtual ~SetPositionViaMethodCall() {}
	/*! Sets a position for argument
		\param[in] a argument
	 */
	virtual void call(const sad::Point2D& a)
	{
		sad::Rect2D area = static_cast<_Object*>(m_o)->area();
		sad::Point2D center = (area[0] + area[2]) / 2.0;

		sad::moveBy(a - center, area);
		static_cast<_Object*>(m_o)->setArea(area);
	}
protected:
	/*! An object link for fast call
	 */
	sad::db::Object* m_o;
};

}

}

}
