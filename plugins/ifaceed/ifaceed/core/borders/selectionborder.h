/*! \file selectionborder.h
	\author HiddenSeeker

	Defines an selection border, which is used for highlighting a selected object
 */
#pragma once
#include "border.h"
#include "deletehotspot.h"

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
	/*! Returns a hot spot for removal of object
		\return hotspot
	 */
	core::borders::DeleteHotspot* deleteHotspot();
protected:
 	 /*! Renders a selected object border as render border
	  */
	 void _process();
	 /*! A delete hotspot for deletion
	  */
	 core::borders::DeleteHotspot* m_delete_hotspot;
};

}

}
