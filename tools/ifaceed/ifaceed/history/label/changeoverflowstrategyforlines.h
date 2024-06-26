/*! \file changeoverflowstrategyforlines.h
    

    Describe a command, which change how should be exceeding part in text of label handled
 */
#pragma once
#include "../scenenodes/scenenodeschangeproperty.h"

namespace history
{

namespace label
{
/*! A command, which change, how should be exceeding part in line of label handled
 */
class ChangeOverflowStrategyForLines: public history::scenenodes::ChangeProperty<unsigned int>
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] old_value old value of property
        \param[in] new_value new value of property
      */
    ChangeOverflowStrategyForLines(sad::SceneNode* d, unsigned int old_value, unsigned int new_value);
    /*! Erases link to a node
      */
    virtual ~ChangeOverflowStrategyForLines() override;
protected:
    /*! Tries to update UI in case if node is selected
        \param[in] e editor
        \param[in] value a value
     */
    virtual void updateUI(core::Editor* e, const unsigned int& value) override;
};

}

}
