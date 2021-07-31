/* \file scenenodespan.h
   
   Describes a command, which is responsible for spanning object between two points
 */
#pragma once
#include "../command.h"
#include "scenenode.h"

#include <sadrect.h>

namespace history
{
    
namespace scenenodes
{
/*! A command, which is responsible for spanning object between two points
 */
class Span: public history::Command
{
public:
    /*! Constructs new command for node
        \param[in] d a node
        \param[in] old_area old area of node
        \param[in] old_angle old angle of node
        \param[in] new_area new area of node
        \param[in] new_angle new angle of node
     */
    Span(
        sad::SceneNode* d,
        const sad::Rect2D& old_area,
        double old_angle,
        const sad::Rect2D& new_area,
        double new_angle
    );
    /*! Erases link to a node
     */
    virtual ~Span();
    /*! Applies changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void commit(core::Editor * ob = nullptr) override;
    /*! Reverts changes, described in command
        \param[in] ob an observer for looking for command
     */
    virtual void rollback(core::Editor * ob = nullptr) override;
protected:
    /*! A node, which is being changed
     */
    sad::SceneNode * m_node;
    /*! Old area of node
     */
    sad::Rect2D m_old_area;
    /*! Old angle of node
     */
    double m_old_angle;
    /*! New area of node
     */
    sad::Rect2D m_new_area;
    /*! New angle of node
     */
    double m_new_angle;
    /*! Performs changing rectangle and angle with scene node
        \param[in] e editor
        \param[in] rect rectangle
        \param[in] angle angle
     */
    void performUpdate(
        core::Editor* e,
        const sad::Rect2D& rect,
        double angle
    );
};

}

}

