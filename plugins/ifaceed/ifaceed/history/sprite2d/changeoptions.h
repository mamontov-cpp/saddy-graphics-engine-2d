/*! \file changeoptions.h
	

	Describe a command, which change scene sprite options
 */
#pragma once
#include "../scenenodes/scenenodeschangeproperty.h"

namespace history
{
	
namespace sprite2d
{
/*! A command, which change scene sprite's options
 */
class ChangeOptions: public history::scenenodes::ChangeProperty<sad::String>
{
public:
	 /*! Constructs new command for node
		\param[in] d a node
		\param[in] oldarea old area for options
		\param[in] oldvalue old value of font
		\param[in] newvalue new value of font
	  */
	ChangeOptions(
		sad::SceneNode* d, 
		const sad::Rect2D& oldarea,
		const sad::String& oldvalue, 
		const sad::String& newvalue
	);
	/*! Erases link to a node
	  */
	virtual ~ChangeOptions();
	/*! Rolls back changed by options
		\param[in] e editor
	 */
	virtual void rollback(core::Editor* e = NULL);
protected:
    /*! Updates UI in case if node is selected
		\param[in] e editor
		\param[in] value a value
	 */ 
    virtual void updateUI(core::Editor* e, const sad::String& value);
	/*! Old area for options
	 */
	sad::Rect2D m_oldarea;
};

}

}
