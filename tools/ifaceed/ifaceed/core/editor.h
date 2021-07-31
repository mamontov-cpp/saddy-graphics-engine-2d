/*! \file   editor.h
    

    Defines a main interface editor class
 */
#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include "../macro.h"

#include "../history/history.h"
#include "../history/batchcommand.h"

#include "../core/qttarget.h"
#include "../core/quitreason.h"

#include "shared.h"

// ReSharper disable once CppUnusedIncludeDirective
#include <scene.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <sadmutex.h>
#include <renderer.h>
#include <closure.h>

#include <cli/parser.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <input/controls.h>

#include <hfsm/hfsmmachine.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <hfsm/hfsmstate.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <hfsm/hfsmhandler.h>

#include <animations/animationsfactory.h>

#include <QObject>
// ReSharper disable once CppUnusedIncludeDirective
#include <QTimer>
#include <QPair>
// ReSharper disable once CppUnusedIncludeDirective
#include <QDialog>
#include <QListWidget>
#include <QThread>
#include <QApplication>
#include <QMainWindow>

class MainPanel;

namespace gui
{
class RenderWays;
class RenderEditorGrid;
class RenderGrids;
class MainPanelProxy;

namespace uiblocks
{
class UIBlocks;	
}

namespace actions
{
class Actions;	
}

}

namespace core
{
class Selection;
class Shared;
class SaddyThread;
class Synchronization;

namespace borders
{
class ActiveBorder;
class SelectionBorder;
}

/*! A main editor class, which works as event receiver and data container
 */
class Editor: public QObject
{
Q_OBJECT

friend class core::SaddyThread;
friend class core::QtTarget;
public:
    /*! A node picking dialog for editor
     */
    struct NodePickingDialog
    {
        /*! A dialog
         */
        QDialog* Dialog;
        /*! An inner list for dialog
         */
        QListWidget* NodeList;
    };
    /*! Constructs editor with empty db
     */
    Editor();
    /*! Frees memory from db
     */
    ~Editor();
    /*! Initializes an editor, loading default data if nothing specified
        \param[in] argc count of arguments
        \param[in] argv arguments
     */
    virtual void init(int argc,char ** argv);
    /*! Returns editor's window, used by Qt part of application
        \return editor's window
     */
    MainPanel* panel() const;
    /*! Returns a machine for editor
        \return machine editor
     */
    sad::hfsm::Machine* machine() const;
    /*! Returns shared state data
        \return shared state data information
     */
    core::Shared* shared() const;
    /*! Returns parsed arguments
        \return parsed arguments
     */
    sad::cli::Parser* parsedArgs() const;
    /*! Returns a history
        \return history
    */
    history::History * history() const;
    /*! Returns main QApplication instance
        \return qt application
     */
    QApplication* app() const;
    /*! A synonym for sad::Renderer::ref()
        \return current renderer
     */
    sad::Renderer* renderer() const;
    /*! Returns synchronization primitive
        \return synchronization primitive
     */
    core::Synchronization* synchronization() const;
    /*! Returns active border, which renders borders of current active object
        \return active border
     */
    core::borders::ActiveBorder* activeBorder() const;
    /*! Returns selection border, which is used for highlighting current item
        \return selection border
     */
    core::borders::SelectionBorder* selectionBorder() const;
    /*! Returns process for rendering ways
        \return process for rendering ways
     */
    gui::RenderWays* renderWays() const;
    /*! Returns process for rendering grids
        \return process for rendering grids
     */
    gui::RenderGrids* renderGrids() const;
    /*! Returns process for rendering editor's grid
        \return process for rendering editor's grid
     */
    gui::RenderEditorGrid* renderEditorGrid() const;
    /*! Returns current selection algorithm for editor
        \return selection
     */
    core::Selection* selection() const;
    /*! Returns current animation factory
        \return animation factory
     */
    sad::animations::Factory* animationFactory() const;
    /*! Quits an editor
     */
    void quit();
    /*! Emits a signal core::Editor::closureArrived()
        with specified arguments
        \param[in] closure closure signal argument
     */
    void emitClosure(sad::ClosureBasic* closure);
    /*! Cleans up if we were adding stuff before clicking "Add XXX", like "Add label" or "Add sprite"
     */
    void cleanupBeforeAdding();
    /*!
     * Tests whether editor is in selected state and current selected node equals passed
     * \param[in] node checked node
     * \return whether node is selected
     */
    bool isNodeSelected(sad::SceneNode* node) const;
    /*! Performed, when editor entered idle state 
     */
    void enteredIdleState();
    /*! Tests, whether editor is in state of adding or editing mode
        \return whether editor is in state of adding or editing mode
     */
    bool isInEditingState() const;
    /*! Sets idle state, cleans database, erasing all objects and history
     */
    void cleanDatabase();
    /*! Reports errors to log
        \param[in,out] errors a list of errors
        \param[in] name a name of file, which has been loading
     */
    void reportResourceLoadingErrors(
        sad::Vector<sad::resource::Error *> & errors,
        const sad::String& name
    );
    /*! Returns true, if current tab of main tab widget is for editing objects
        \return whether editor could be used for editing objects
     */
    bool isInObjectEditingState() const;
    /*! Returns true, if current tab of main tab widget is for editing states
        \return whether editor could be used for editing objects
     */
    bool isInWaysEditingState() const;
    /*! Returns true, if current tab configuration matches editing grid editing
        \return whether grid editing state
     */
    bool isInGridEditingState() const;
    /*! Tries to enter object editing state
     */
    void tryEnterObjectEditingState();
    /*! Tries to enter way editing state
     */ 
    void tryEnterWayEditingState();
    /*! Sets current batch command for editor scripting
        \param[in] c a command
     */
    void setCurrentBatchCommand(history::BatchCommand* c);
    /*! Returns current batch command for editor
        \return current batch command
     */
    history::BatchCommand* currentBatchCommand() const;
    /*! Tests, whether database is empty
        \return whether database is empty
     */
    bool isDatabaseEmpty() const;
    /*! Returns a UI blocks for editor
     */
    gui::uiblocks::UIBlocks* uiBlocks() const;
    /*! Returns different actions for gui
     */
    gui::actions::Actions* actions() const;
    /*! Returns a proxy for main panel methods
        \return proxy
     */
    gui::MainPanelProxy* panelProxy() const;
    /*! Returns main panel as widget
        \return main panel as widget
     */
    QWidget* panelAsWidget() const;
    /*! Adds a command to history
        \param[in] c command
        \param[in] from_editor whether it was added from editor or from script
     */
    void addToHistory(history::Command* c, bool from_editor);
    /*! Takes current value for fast mode counter
        \return current value for fast mode counter
     */
    unsigned int fastModeCounter() const;
    /*! Increments fast mode counter
     */
    void incrementFastModeCounter();
    /*! Tries to update parent grid if any for specified node
        \param[in] node a node
     */
    void tryUpdateParentGridForNode(sad::SceneNode* node) const;
    /*! Makes a dialog for selecting node
        \param[in] parent a parent widget for dialog
        \param[in] window_title a window title
        \param[in] pairs a pairs of nodes names
     */
    static core::Editor::NodePickingDialog dialogForSelectingNode(
        QWidget* parent,
        const QString& window_title,
        const QVector<QPair<QString, unsigned long long> >& pairs
    );
public slots:
    /*! Called, when Qt Event Loop is started. Used to load default resources and pre-set
        default behaviour
     */
    void start();
    /*! Undoes history action 
     */
    void undo();
    /*! Redoes history action
     */
    void redo();
    /*! Clears fast mode counter
     */
    void clearFastModeCounter();
signals:
    /*! Signal is emitted, when closure is arrived
        \param[in] closure data for closure
     */
    void closureArrived(sad::ClosureBasic * closure);	
protected:
    /*! Target for sending information
     */
    core::QtTarget* m_qt_target;
    /*! Thread for rendering
     */
    core::SaddyThread* m_render_thread;
    /*! A synchronization primitive for
        synchronizing threads
     */
    core::Synchronization* m_synchronization;
    /*! Main window of application
     */
    MainPanel* m_main_window;
    /*! Application of qt, which is used
     */
    QApplication* m_qt_app;
    /*! A hierarchical state machine
     */
    sad::hfsm::Machine* m_machine;
    /*! Command line arguments
     */
    sad::cli::Args* m_args;
    /*! A parsed command-line arguments
     */
    sad::cli::Parser* m_parsed_args;
    /*! History of data
     */
    history::History* m_history;
    /*! Describes a behaviour shared data
     */
    core::Shared* m_shared;
    /*! A reason, while editor must be quit
     */
    core::QuitReason  m_quit_reason;
    /*! An active border, which renders current active object
     */
    core::borders::ActiveBorder* m_active_border;
    /*! A selection border, which is used for highlighting current item
     */
    core::borders::SelectionBorder* m_selection_border;
    /*! A way to render ways
     */
    gui::RenderWays* m_render_ways;
    /*! A way to render grids
     */ 
    gui::RenderGrids* m_render_grids;
    /*!  A way for render editor grids
     */
    gui::RenderEditorGrid* m_render_editor_grid;
    /*! Current selection algorithm
     */
    core::Selection* m_selection;
    /*! A factory, used for creating animations
     */
    sad::animations::Factory m_animation_factory;
    /*! A current batch command for scripting
     */
    history::BatchCommand* m_current_batch_command;
    /*! A UI blocks for editor
     */
    gui::uiblocks::UIBlocks* m_ui_blocks;
    /*! An actions, related to editor
     */
    gui::actions::Actions* m_actions;
    /*! A panel proxy
     */
    gui::MainPanelProxy* m_panel_proxy;
    /*! A counter, whose value will be appended to name in fast mode
     */
    unsigned int m_fast_mode_counter;
    /*! Initializes conversion table with all conversion table
     */
    void initConversionTable();
    /*! Called, when core::SaddyThread finishes working. Sets
        reason, why editor to quit as initiated by Saddy's renderer
        and calls cleanup actions
     */
    void saddyQuitSlot(); 
protected slots:
    /*! Creates QApplication and window,
        sets up global key handlers and starts event loop,
        calling core::Editor::start(), after event loop is started
     */
    virtual void runQtEventLoop();
    /*! Runs saddy renderer's event loop. Called inside SaddyThread::run,
        sets quit flag,
        when saddy quits working
     */
    virtual void runSaddyEventLoop();
    /*! Called, when user closes last Qt window. Sets reason, why editor is quit
        and calls quit actions
     */
    void qtQuitSlot();
    /*! Depending on reasons, why editor is quit closes saddy renderer's window
     *  or panel window
     */
    void onQuitActions();
    /*! Runs a closure. Used by main thread to work
        with user-defined actions.
        \param[in] closure closure data
     */
    virtual void runClosure(sad::ClosureBasic * closure);
};

}
