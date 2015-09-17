/*! \file labelactions.h
    

    Describes a group of actions, linked to label
 */
#pragma once
#include <QObject>

#include <input/events.h>

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
/*! A group of actions, linked to labels
 */	
class LabelActions: public QObject
{
Q_OBJECT
public:
    /*! A command maker callback
     */
    typedef history::Command* (gui::LabelActions::*CommandMaker)(sad::SceneNode* node,  unsigned int oldvalue, unsigned int newvalue);
    /*! Creates new label actions
        \param[in] parent a parent object
     */
    LabelActions(QObject* parent = NULL);
    /*! This class could be inherited
     */
    virtual ~LabelActions();
    /*! Sets panel, where actions belong to
     */
    void setPanel(MainPanel* e);
    /*! Returns panel, where actions belong to
     */
    MainPanel* panel() const;
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
     * \param[in] newsize new label size
     */
    void labelSizeChanged(unsigned int newsize);
    /*!
     * Called, when user types in label text
     */
    void labelTextChanged();
    /*!
     * Handles label line spacing changes
     * \param[in] newvalue a new value
     */
    void labelLineSpacingChanged(double newvalue);
    /*! Handles change of label's maximal line width
     * \param[in] newvalue  a new value for property
     */
    void labelMaximalLineWidthChanged(int newvalue);
    /*! Handles change of break text parameter for label
     *  \param[in] newvalue  a new value for property
     */
    void labelBreakTextChanged(int newvalue);
    /*! Handles change of how overflow of line should be handled
     *  \param[in] newvalue a new value for property
     */
    void labelOverflowStrategyChanged(int newvalue);
    /*! Handles change of where suspension sign should be placed
     *  \param[in] newvalue  a new value for property
     */
    void labelTextEllipsisChanged(int newvalue);
    /*! Handles change of label's maximal lines count
     *  \param[in] newvalue a new value for property
     */
    void labelMaximalLinesCountChanged(int newvalue);
    /*! Handles change of how overflow of text should be handled
     *  \param[in] newvalue a new value for property
     */
    void labelOverflowStrategyForLinesChanged(int newvalue);
    /*! Handles change of where suspension sign should be placed in case, when multiple lines overflow
     *  \param[in] newvalue  a new value for property
     */
    void labelTextEllipsisForLinesChanged(int newvalue);
private:
    /*! Performs property change, related to value
        \param[in] newvalue a new value for property
        \param[in] prop property value
        \param[in] maker a maker command
     */
    void unsignedIntPropertyChanged(
        int newvalue,
        const sad::String& prop,
        gui::LabelActions::CommandMaker maker
    );
    /*! Returns command, related to label actions
        \param[in] node a node value
        \param[in] oldvalue an old value for property
        \param[in] newvalue
     */
    template<
        typename T
    >
    history::Command* command(sad::SceneNode* node,  unsigned int oldvalue, unsigned int newvalue)
    {
        return new T(node, oldvalue, newvalue);
    }
    /*! An panel, which actions are belong to
     */
    MainPanel* m_panel;
};

}
