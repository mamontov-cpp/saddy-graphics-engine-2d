/*! \file scenenodeslayerswap.h
    

    Used to swap layers on scenes.
 */
#pragma once
#include "../command.h"

namespace sad
{
class SceneNode;
}

namespace history
{

namespace scenenodes
{

class LayerSwap: public history::Command
{
public: 
    /*! Adds a scene, which was added. Command links to scene
        \param[in] s1 first node
        \param[in] s2 second node
        \param[in] l1 first position
        \param[in] l2 second position
     */
    LayerSwap(sad::SceneNode* s1, sad::SceneNode* s2, int l1, int l2);
    /*! Deletes reference to a scene
     */
    ~LayerSwap() override;
    /*! Applies changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Reverts changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! A first scene
     */
    sad::SceneNode* m_n1;
    /*! A second scene
     */
    sad::SceneNode* m_n2;
    /*! A list position for first scene
     */
    int m_list_position1;
    /*! A list position for second scene
     */
    int m_list_position2;
};

}

}
