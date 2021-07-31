/*! \file border.h
    

    Describes a border, used to render different buttons around sprite and a border
 */
#pragma once
#include <sadvector.h>
#include <scenenode.h>
#include <sadpoint.h>
#include <sadrect.h>

#include <pipeline/pipelineprocess.h>

#include "../shared.h"


namespace core
{
    
namespace borders
{

/*! A border, used to render different buttons around sprite and a render
 */
class Border: public sad::pipeline::AbstractProcess
{
public:
    /*! Creates new border, linked to specified shared data
        \param[in] data a shared data, where all border data is stored
     */
    Border(core::Shared* data);
    /*! This class could be inherited
     */
    virtual ~Border() override;
protected:
    /*! A linked data for a border, where selected or active object is stored
     */
    core::Shared* m_data;
};

}

}
