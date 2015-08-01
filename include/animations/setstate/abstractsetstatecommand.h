/*! \file animations/setstate/abstractsetstatecommand.h
	

	Defines a command for setting a state for an object in animation process
 */
#pragma once

namespace sad
{
	
namespace animations
{
	
namespace setstate
{
	

class AbstractSetStateCommand
{
public:	
    /*! Clones command
        \return command
     */
    virtual sad::animations::setstate::AbstractSetStateCommand* clone() const = 0;
	/*! Must be inherited
	 */
	virtual ~AbstractSetStateCommand();
};

}

}

}
