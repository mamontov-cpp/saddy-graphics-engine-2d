/*! \file   editor.h
    

    Defines a main interface editor class
 */
#pragma once
#include "../macro.h"

#include "../history/history.h"
#include "../history/batchcommand.h"

#include "../core/qttarget.h"
#include "../core/quitreason.h"


#include "../mainpanel.h"

#include "shared.h"

#include <scene.h>
#include <sadmutex.h>
#include <renderer.h>
#include <closure.h>

#include <cli/parser.h>

#include <input/controls.h>

#include <hfsm/hfsmmachine.h>
#include <hfsm/hfsmstate.h>
#include <hfsm/hfsmhandler.h>

#include <animations/animationsfactory.h>

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QApplication>
#include <QMainWindow>

class MainPanel;

namespace gui
{
class RenderWays;
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
    /*! Constructs editor with empty db
     */
    Editor();
    /*! Frees memory from db
     */
    ~Editor();
    /*! Inits an editor, loading default data if nothing specified
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
    /*! Returns way to render elements
        \return stuff for rendering ways
     */
    gui::RenderWays* renderWays() const;
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
        \param[in, out] errors a list of errors
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
signals:
    /*! Signal is emitted, when closure is arrived
        \param[in] closure data for closure
     */
    void closureArrived(sad::ClosureBasic * closure);	
protected:
    /*! Target for sending information
     */
    core::QtTarget* m_qttarget;
    /*! Thread for rendering
     */
    core::SaddyThread* m_renderthread;
    /*! A synchronization primitive for
        synchronizing threads
     */
    core::Synchronization* m_synchronization;
    /*! Main window of application
     */
    MainPanel* m_mainwindow;
    /*! Application of qt, which is used
     */
    QApplication* m_qtapp;
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
    /*! A selection border, which is used for higlighting current item
     */
    core::borders::SelectionBorder* m_selection_border;
    /*! A way to render ways
     */
    gui::RenderWays* m_renderways;
    /*! Current selection algorithm
     */
    core::Selection* m_selection;
    /*! A factory, used for creating animations
     */
    sad::animations::Factory m_animation_factory;
    /*! A current batch command for scripting
     */
    history::BatchCommand* m_current_batchcommand;
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
    /*! Сalled, when user closes last Qt window. Sets reason, why editor is quit
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
