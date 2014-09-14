/*! \file selectionborder.h
	\author HiddenSeeker

	Defines an selection border, which is used for highlighting a selected object
 */
#pragma once
#include "border.h"

namespace core
{
	
namespace borders
{

/*! Defines an selection border, which is used, when item is selected
 */
class SelectionBorder: public core::borders::Border
{
public:
	/*! Creates new border, with shared data
		\param[in] data specified data
	 */
	SelectionBorder(core::Shared* data);
	/*! Can be inherited
	 */
	virtual ~SelectionBorder();	
protected:
 	 /*! Renders a selected object border as render border
	  */
	 void _process();
};

}

}
