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
        \param[in] old_area old area for options
        \param[in] old_value old value of font
        \param[in] new_value new value of font
      */
    ChangeOptions(
        sad::SceneNode* d, 
        const sad::Rect2D& old_area,
        const sad::String& old_value, 
        const sad::String& new_value
    );
    /*! Erases link to a node
      */
    virtual ~ChangeOptions();
    /*! Rolls back changed by options
        \param[in] e editor
     */
    virtual void rollback(core::Editor* e = nullptr) override;
protected:
    /*! Updates UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */ 
    virtual void updateUI(core::Editor* e, const sad::String& value) override;
    /*! Old area for options
     */
    sad::Rect2D m_old_area;
};

}

}
