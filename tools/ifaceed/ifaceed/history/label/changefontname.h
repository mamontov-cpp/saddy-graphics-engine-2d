/*! \file changefontname.h
    

    Describe a command, which change scene label's font
 */
#pragma once
#include "../scenenodes/scenenodeschangeproperty.h"

namespace history
{
    
namespace label
{
/*! A command, which change scene label's font
 */
class ChangeFontName: public history::scenenodes::ChangeProperty<sad::String>
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] old_value old value of font
        \param[in] new_value new value of font
      */
    ChangeFontName(sad::SceneNode* d, const sad::String& old_value, const sad::String& new_value);
    /*! Erases link to a node
      */
    virtual ~ChangeFontName() override;
protected:
    /*! Updates UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */ 
    virtual void updateUI(core::Editor* e, const sad::String& value) override;
};

}

}
