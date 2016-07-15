/*! \file sceneschangename.h
    

    Describes a command, used to change name for a scene
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
class ChangeName: public history::Command
{
public:
    /*! Adds a scene, which was added. Command links to scene
        \param[in] s a scene
        \param[in] oldname old name of a scene
        \param[in] newname new name of a scene
     */
    ChangeName(sad::Scene * s, const sad::String& oldname, const sad::String& newname);
    /*! Deletes reference to a scene
     */
    ~ChangeName();
    /*! Applies changes without affecting UI
        \param[in] ob an editor
     */
    virtual void commitWithoutUpdatingUI(core::Editor * ob = NULL);
    /*! Applies changes, described in command
        \param[in] ob an editor
     */
    virtual void commit(core::Editor * ob = NULL);
    /*! Reverts changes, described in command
        \param[in] ob an editor
     */
    virtual void rollback(core::Editor * ob = NULL);
protected:
    /*! Commits a changing for name
        \param[in] ob editor
        \param[in] update_ui whether we should update ui
     */
    void commit(core::Editor* ob, bool update_ui);
    /*! Updates dependent names and data
        \param[in] e editor
     */
    void updateDependent(core::Editor * e);
    /*! Describes an added scene
     */
    sad::Scene* m_scene;
    /*! Old name for a scene
     */
    sad::String m_old;
    /*! New name for a scene
     */
    sad::String m_new;
};

}

}
