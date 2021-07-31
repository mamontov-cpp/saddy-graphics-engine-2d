/*! \file labelactions.h
    

    Describes a group of actions, linked to label
 */
#pragma once
#include <QObject>

#include <input/events.h>

#include "abstractactions.h"

class MainPanel;

namespace history
{
class Command;    
}

namespace sad
{
class SceneNode;    
}

namespace gui
{

namespace actions
{

/*! A group of actions, linked to labels
 */	
class LabelActions: public QObject, public gui::actions::AbstractActions
{
Q_OBJECT
public:
    /*! A command maker callback
     */
    typedef history::Command* (gui::actions::LabelActions::*CommandMaker)(sad::SceneNode* node,  unsigned int old_value, unsigned int new_value);
    /*! Creates new label actions
        \param[in] parent a parent object
     */
    LabelActions(QObject* parent = nullptr);
    /*! This class could be inherited
     */
    virtual ~LabelActions();
    /*! Cancels adding label state, enters previous state
     */
    void cancelAddLabel();
    /*! Moves label to a mouse position
        \param[in] e event
     */
    void moveLabel(const sad::input::MouseMoveEvent& e);
    /*! Commits label adding to a scene
        \param[in] e event
     */
    void commitLabelAdd(const sad::input::MousePressEvent& e);
public slots:
    /*! Adds new label to a scene, if all parameters in form are valid
     */
    void addLabel();
    /*! Handles label font changes for an editable node
        \param[in] s a new resource name
     */
    void labelFontChanged(sad::String s);
    /*!
     * Handles label size changes for an editable node
     * \param[in] new_size new label size
     */
    void labelSizeChanged(unsigned int new_size);
    /*!
     * Called, when user types in label text
     */
    void labelTextChanged();
    /*!
     * Handles label line spacing changes
     * \param[in] new_value a new value
     */
    void labelLineSpacingChanged(double new_value);
    /*! Handles change of label's maximal line width
     * \param[in] new_value  a new value for property
     */
    void labelMaximalLineWidthChanged(int new_value);
    /*! Handles change of break text parameter for label
     *  \param[in] new_value  a new value for property
     */
    void labelBreakTextChanged(int new_value);
    /*! Handles change of how overflow of line should be handled
     *  \param[in] new_value a new value for property
     */
    void labelOverflowStrategyChanged(int new_value);
    /*! Handles change of where suspension sign should be placed
     *  \param[in] new_value  a new value for property
     */
    void labelTextEllipsisChanged(int new_value);
    /*! Handles change of label's maximal lines count
     *  \param[in] new_value a new value for property
     */
    void labelMaximalLinesCountChanged(int new_value);
    /*! Handles change of how overflow of text should be handled
     *  \param[in] new_value a new value for property
     */
    void labelOverflowStrategyForLinesChanged(int new_value);
    /*! Handles change of where suspension sign should be placed in case, when multiple lines overflow
     *  \param[in] new_value  a new value for property
     */
    void labelTextEllipsisForLinesChanged(int new_value);
    /*! Handles attempt to change "has formatting" property, disabling or enabling formatting in 
     *  corresponding object
     *  \param[in] new_value a new value for formatting
     */
    void labelHasFormattingChanged(bool new_value);
private:
    /*! Performs property change, related to value
        \param[in] new_value a new value for property
        \param[in] prop property value
        \param[in] maker a maker command
     */
    void unsignedIntPropertyChanged(
        int new_value,
        const sad::String& prop,
        gui::actions::LabelActions::CommandMaker maker
    );
    /*! Returns command, related to label actions
        \param[in] node a node value
        \param[in] old_value an old value for property
        \param[in] new_value
        \return command
     */
    template<
        typename T
    >
    // ReSharper disable once CppMemberFunctionMayBeStatic
    history::Command* command(sad::SceneNode* node,  unsigned int old_value, unsigned int new_value)
    {
        return new T(node, old_value, new_value);
    }

};

}

}
