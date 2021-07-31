/*! \file activeborder.h
    

    Defines an active border, which is used when placing an item
 */
#pragma once
#include "border.h"

namespace core
{
    
namespace borders
{

/*! Defines an active border, which is used, when placing an item
 */
class ActiveBorder: public core::borders::Border
{
public:
    /*! Creates new border, with shared data
        \param[in] data specified data
     */
    ActiveBorder(core::Shared* data);
    /*! Can be inherited
     */
    virtual ~ActiveBorder();
protected:
     /*! Renders a selected object border as render border
      */
     void _process() override;
};

}

}
