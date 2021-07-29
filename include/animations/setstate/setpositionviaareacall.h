/*! \file animations/setstate/setpositionviaareacall.h
    

    Sets area via method call
 */
#pragma once
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
class SetPositionViaMethodCall: public sad::animations::setstate::TypedCommand<sad::Point2D>
{
public:
    /*! Constructs new empty cached call
        \param[in]  o object object, which method should be called upon
     */
    inline SetPositionViaMethodCall(sad::db::Object* o) : m_o(o) {}
    /*! Could be inherited
     */
    virtual ~SetPositionViaMethodCall() override {}
    /*! Clones command
        \return command
     */
    virtual sad::animations::setstate::AbstractSetStateCommand* clone() const override
    {
        return new sad::animations::setstate::SetPositionViaMethodCall<_Object>(m_o);
    }
    /*! Sets a position for argument
        \param[in] a argument
     */
    virtual void call(const sad::Point2D& a) override
    {
        sad::Rect2D area = static_cast<_Object*>(m_o)->area();
        const sad::Point2D center = (area[0] + area[2]) / 2.0;

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
