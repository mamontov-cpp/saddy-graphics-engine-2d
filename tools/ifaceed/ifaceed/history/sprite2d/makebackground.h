/*! \file makebackground.h
    

    Describe a command, which sets sprite as background
 */
#pragma once
#include "../scenenodes/scenenodeschangeproperty.h"

namespace history
{
    
namespace sprite2d
{
/*! A command, which change scene sprite's options
 */
class MakeBackground: public history::Command
{
public:
     /*! Constructs new command for node
        \param[in] d a node
        \param[in] old_area old area of sprite
        \param[in] old_angle old angle
        \param[in] old_layer old layer of sprite
        \param[in] new_area new area of sprite
        \param[in] new_angle new angle
        \param[in] new_layer new layer of sprite
      */
    MakeBackground(
        sad::SceneNode* d, 
        const sad::Rect2D& old_area,
        float old_angle,
        unsigned int old_layer,
        const sad::Rect2D& new_area,
        float new_angle,
        unsigned int new_layer
    );
    /*! Erases link to a node
      */
    virtual ~MakeBackground();
    /*! Commits changed
        \param[in] e editor
     */
    virtual void commit(core::Editor* e = nullptr) override;
    /*! Rolls back command
        \param[in] e editor
     */
    virtual void rollback(core::Editor* e = nullptr) override;
protected:
    /*! An action node for making background
     */
    sad::SceneNode* m_node;
    /*! Old area for making background
     */
    sad::Rect2D m_old_area;
    /*! New area for making background
     */
    sad::Rect2D m_new_area;
    /*! An old angle
     */
    float m_old_angle;
    /*! A new angle
     */
    float m_new_angle;
    /*! An old layer for item
     */
    unsigned int m_old_layer;
    /*! A new layer for item
     */
    unsigned int m_new_layer;

    /*! Updates a background
        \param[in] e area
        \param[in] area area for background
        \param[in] angle angle
        \param[in] layer a new layer for sprite
     */
    void update(
        core::Editor* e, 
        const sad::Rect2D& area, 
        float angle, 
        unsigned int layer
    );
};

}

}
