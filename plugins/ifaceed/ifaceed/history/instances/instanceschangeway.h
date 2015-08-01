/*! \file instanceschangeway.h
    

    Describe a command, which changes way for animation instance
 */
#pragma once
#include "instanceschangeproperty.h"

namespace history
{

namespace instances
{
/*! A command, which changes way for animation instance
 */
class ChangeWay: public history::instances::ChangeProperty<unsigned long long>
{
public:
     /*! Constructs new command for animation
        \param[in] d a node
        \param[in] oldvalue old text
        \param[in] newvalue new text
      */
    ChangeWay(sad::animations::Instance* d, unsigned long long oldvalue, unsigned long long newvalue);
    /*! Erases link to an animation
      */
    virtual ~ChangeWay();
protected:
	/*! Updates object in main list
		\param[in] e editor
		\param[in] value a value
	 */
	void updateItem(core::Editor* e, unsigned long long value);
    /*!
     * Updates current text in field with object name
     * \param[in] e editor
     * \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const unsigned long long& value);
};

}

}
