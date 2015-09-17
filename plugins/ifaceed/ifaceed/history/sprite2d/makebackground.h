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
        \param[in] oldarea old area of sprite
        \param[in] oldangle old angle
        \param[in] oldlayer old layer of sprite
        \param[in] newarea new area of sprite
        \param[in] newangle new angle
        \param[in] newlayer new layer of sprite
      */
    MakeBackground(
        sad::SceneNode* d, 
        const sad::Rect2D& oldarea,
        float oldangle,
        unsigned int oldlayer,
        const sad::Rect2D& newarea,
        float newangle,
        unsigned int newlayer
    );
    /*! Erases link to a node
      */
    virtual ~MakeBackground();
    /*! Commits changed
        \param[in] e editor
     */
    virtual void commit(core::Editor* e = NULL);
    /*! Rolls back command
        \param[in] e editor
     */
    virtual void rollback(core::Editor* e = NULL);
protected:
    /*! An action node for making background
     */
    sad::SceneNode* m_node;
    /*! Old area for making background
     */
    sad::Rect2D m_oldarea;
    /*! New area for making background
     */
    sad::Rect2D m_newarea;
    /*! An old angle
     */
    float m_oldangle;
    /*! A new angle
     */
    float m_newangle;
    /*! An old layer for item
     */
    unsigned int m_oldlayer;
    /*! A new layer for item
     */
    unsigned int m_newlayer;

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
