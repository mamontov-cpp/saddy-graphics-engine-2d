/*! \file scenesadd.h
    

    Describes a command, used to add an scene to scenes list
 */
#pragma once
#include "../command.h"
#include <scene.h>


namespace history
{

namespace scenes
{

/*! A command to add scene to scene list
 */
class Add: public history::Command
{
public:
    /*! Adds a scene, which was added. Command links to scene
        \param[in] s a scene
     */
    Add(sad::Scene * s);
    /*! Deletes reference to a scene
     */
    ~Add() override;
    /*! Applies changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Reverts changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! Describes an added scene
     */
    sad::Scene* m_scene;
};

}

}
