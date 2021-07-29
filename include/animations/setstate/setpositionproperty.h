/*! \file animations/setstate/setpositionproperty.h
    

    Defines a command, which sets position by it's area
 */
#pragma once
#include "typedcommand.h"

#include "../../db/dbobject.h"

namespace sad
{

namespace animations
{

namespace setstate
{

/*! Defines a call command, as setting position via getting area property and shifting it to center
 */
class SetPositionProperty : public sad::animations::setstate::TypedCommand<sad::Point2D>
{
    
public:
    /*! Constructs new empty cached call
        \param[in]  o object object, which method should be called upon
     */
    SetPositionProperty(sad::db::Object* o);
    /*! Clones a command
        \return copy of object
     */
    sad::animations::setstate::AbstractSetStateCommand* clone() const override;
    /*! Could be inherited
     */
    virtual ~SetPositionProperty() override;
    /*! Sets a position for argument
        \param[in] a argument
     */
    virtual void call(const sad::Point2D& a) override;
protected:
    /*! An object link for fast call
     */
    sad::db::Object* m_o;
};

}

}

}
