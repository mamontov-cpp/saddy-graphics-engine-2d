/*! \file changefontname.h
	\author HiddenSeeker

	Describe a command, which change scene label's font
 */
#pragma once
#include "../scenenodes/scenenodespropertychanged.h"

namespace history
{
	
namespace label
{
/*! A command, which change scene label's font
 */
class ChangeFontName: public history::scenenodes::PropertyChanged<sad::String>
{
public:
	 /*! Constructs new command for node
		\param[in] d a node
		\param[in] oldvalue old value of font
		\param[in] newvalue new value of font
	  */
	ChangeFontName(sad::SceneNode* d, const sad::String& oldvalue, const sad::String& newvalue);
	/*! Erases link to a node
	  */
	virtual ~ChangeFontName();
protected:
    /*! Updates UI in case if node is selected
		\param[in] e editor
		\param[in] value a value
	 */ 
    virtual void updateUI(core::Editor* e, const sad::String& value);
};

}

}
