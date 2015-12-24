/*! \file scenenodeschangepropertywhichlinkedtocheckbox.h
    

    Describes a property change for property, which referenced in QCheckBox 
 */
#pragma once
#include "scenenodeschangeproperty.h"
#include <QCheckBox>

namespace history
{
    
namespace scenenodes
{

/*! Describes a property change, for property, which referenced in QCheckBox
 */
class ChangePropertyWhichLinkedToCheckbox:public history::scenenodes::ChangeProperty<bool>
{
public:
    /*! Constructs new command for node
        \param[in] cb a checkbox
        \param[in] d a node
        \param[in] property a property name
        \param[in] oldvalue old value of property
        \param[in] newvalue new value of property
      */
    ChangePropertyWhichLinkedToCheckbox(
        QCheckBox* cb,
        sad::SceneNode* d,
        const sad::String& property,
        bool oldvalue,
        bool newvalue
    );

    /*! Could be inherited
     */
    virtual ~ChangePropertyWhichLinkedToCheckbox();
protected:
    /*! A checkbox to be set
     */ 
    QCheckBox* m_cb;
     /*!
     * Updates a checkbox with new value
     * \param e editor
     * \param value a value of property
     */
    virtual void updateUI(core::Editor* e, const bool& value);
};

}

}
