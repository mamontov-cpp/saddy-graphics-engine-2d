/*! \file scenenodeschangename.h
    

    Describe a command, which change name of scene node
 */
#pragma once
#include "scenenodeschangeproperty.h"

namespace history
{

namespace scenenodes
{
/*! A command, which change name of scene node
 */
class ChangeName: public history::scenenodes::ChangeProperty<sad::String>
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] oldvalue old text
        \param[in] newvalue new text
      */
    ChangeName(sad::SceneNode* d, const sad::String& oldvalue, const sad::String& newvalue);
    /*! Erases link to a node
      */
    virtual ~ChangeName();
protected:
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    virtual void tryUpdateUI(core::Editor* e, const sad::String& value);
    /*!
     * Updates current text in field of object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const sad::String& value);
	/*! Updates dependent names and data
		\param[in] e editor
	 */
	void updateDependent(core::Editor * e);
};

}

}
