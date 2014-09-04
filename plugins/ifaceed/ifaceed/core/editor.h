/*! \file   core::Editor.h
	\author HiddenSeeker

	Defines a main interface editor class
 */
#include "../macro.h"

#include "../history/history.h"

#include "../core/qttarget.h"
#include "../core/editorbehaviour.h"
#include "../core/quitreason.h"


#include "../mainpanel.h"

#include "shared.h"
#include "objectxmlwriter.h"

#include <scene.h>
#include <sadmutex.h>
#include <renderer.h>
#include <closure.h>

#include <cli/parser.h>

#include <input/controls.h>

#include <config/sprite2dconfig.h>

#include <hfsm/hfsmmachine.h>
#include <hfsm/hfsmstate.h>
#include <hfsm/hfsmhandler.h>

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QApplication>
#include <QMainWindow>


#pragma once

class ScreenTemplate;
class FontTemplateDatabase;
class SelectedObjectBorder;
class IFaceSharedData;

namespace core
{
class EditorBehaviour;
class Shared;
class SaddyThread;

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
	MainPanel* panel();
	/*! Returns a machine for editor
		\return machine editor
	 */
	sad::hfsm::Machine* machine();
	/*! Returns a database fwith all of resources
	 */
	FontTemplateDatabase * database();
	/*! Tries to render active object, if any
	 */ 
	void tryRenderActiveObject();
	/*! Returns current resulting in-game screen
		\return result
	 */
	inline ScreenTemplate * result()
	{
		return m_result;
	}
	inline SelectedObjectBorder * selectionBorder() 
	{
		 return m_selection_border;
	}
	/*! Returns shared data
		\return shared data information
	*/
	core::Shared * shdata();
	/*! Quits an editor
	 */
	void quit();
	/*! Highlights a state of editor in GUI. Override this for custom output
		\param[in] hint hint of state
	  */
	virtual void highlightState(const sad::String & hint);
	/*! Shows objects stats for selected object
		Implemented in \\core\\states\\changingselection.h
		\param[in] o object, which must be selected
	 */
	virtual void showObjectStats(AbstractScreenObject * o);
	/*! Tries to select some object
		Implemented in \\core\\states\\changingselection.h
		\param[in] p point
		\param[in] enterSelected whether we should enter selected states if found
	 */
	virtual void trySelectObject(sad::Point2D p, bool enterSelected);
	/*! Returns parsed arguments
		\return parsed arguments
	*/
	sad::cli::Parser * parsedArgs() const;
	/*! Returns a scene
		\return used scene
	*/
	inline sad::Scene * scene() { return this->m_scene; }
	/*! Returns a behaviour shated data, needed to implement some stuff
		\return shared data for behaviour
	 */
	inline core::Shared * behaviourSharedData() 
	{
		return m_behavioursharedata;
	}
	/*! Returns a history
		\return history
	*/
	inline history::History * history() { return m_history; }

	/*! Returns a behaviour hash
		\return behaviour hash
	*/
	inline sad::Hash<sad::String, core::EditorBehaviour *> & behaviours() 
	{
		return m_behaviours;
	}
	/*! Erases a current behaviour for work and sets for nothing
	*/
	void eraseBehaviour();

	/*! Deactivates old behaviour and enters new behaviour
		\param[in]  name name of new behaviour
	*/
	virtual void setBehaviour(const sad::String & name);
	/*! Enters a behaviour of editor
		\return editor behaviour
	*/
	core::EditorBehaviour * currentBehaviour();
	/*! Emits a closure signal closureArrived()
		\param[in] closure closure signal arrived
	*/
	void emitClosure(sad::ClosureBasic * closure);
	/*! Casts a current behaviour state identified by state to needed
		\param[in] s string name of state
		\return state
	*/
	template<typename T> T * cbStateAs(const sad::String & s) {
		return static_cast<T *>(this->currentBehaviour()->getState(s));
	}
	/*! Returns an icon container
	 */
	sad::Sprite2DConfig & icons();
	 /*! Returns a qt application
		 \return qt application
	*/
	inline QApplication * qtApp() { return this->m_qtapp;}	
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
	/*! Tries erasing object, depending on current object state
	 */
	virtual void tryEraseObject();
	/*! Updates a list from event
	 */
	virtual void submitEvent(const sad::String & eventType, const sad::db::Variant & v);
	/*! Appends a rotation command to a history
		Used for deferred rotation appendance
	 */
	void appendRotationCommand();
	/*! Performs full texture reload from a data
		Uses current parsed data from reload, all object schemes and stuff.
		Reload must be hit only from Qt code, otherwise 
		everything would fail. 
     */
	void reload();
	/*!  Saves a screen template into a file
	 */ 
	void save();
	/*!  Loads an editor data from a file
	 */
	void load();
signals:
	/*! Signal is emitted, when closure is arrived
		\param[in] closure data for closure
	*/
	void closureArrived(sad::ClosureBasic * closure);	
protected:
	/*! A selection border with capabilities spots to edit item
	*/
    SelectedObjectBorder * m_selection_border;
	/*!	Counter for loading all of dbs
	*/
    int m_counter;
	/*! A template database with fonts
	*/
	FontTemplateDatabase * m_db;
	/*! Screen template data
	*/
	ScreenTemplate * m_result;
	/*! Determines, whether we are already handling an event
	*/
	bool m_handling_event;
	/*! A defines editor behaviours
	*/
	sad::Hash<sad::String, core::EditorBehaviour *> m_behaviours;
	/*! A current behaviour
	*/
	sad::String m_current_behaviour;
	/*! A reason, while editor must be quit
	*/
	core::QuitReason  m_quit_reason;
	/*! Reports errors to log
		\param[in, out] errors a list of errors
		\param[in] name a name of file, which has been loading
	*/
	void reportResourceLoadingErrors(
		sad::Vector<sad::resource::Error *> & errors,
		const sad::String& name
	);
	/*! Initializes conversion table with all conversion table
	 */
	void initConversionTable();
	/*! Sets a database for templates
		\param[in] db database
	*/
	void setDatabase(FontTemplateDatabase * db);
	/*! Returns a command line arguments
		\return command line arguments
	 */ 
	inline sad::cli::Args * commandLineArguments() { return m_cmdargs;}
	/*! A quit slot for saddy, which is run when user runs saddy
	*/
	void saddyQuitSlot();
protected slots:
	/*! Runs qt event loop (qt app)
	*/
	virtual void runQtEventLoop();
	/*! Runs saddy renderer's event loop. Ran inside SaddyThread::run, sets quit flag,
		when saddy quits working
	*/
	virtual void runSaddyEventLoop();
	/*! Contains a different actions, which is runned from main thread
		use variable @see m_quit_reason for reason of exit
	*/
	void onQuitActions();
	/*! Runs, when qt quits 
	*/
	void qtQuitSlot();
	/*! Runs a closure. Used by qt thread to work with closure.
		\param[in] closure closure data
	*/
	virtual void onClosureArrived(sad::ClosureBasic * closure);
private:
	/*! Target for sending information
	*/
	core::QtTarget* m_qttarget;
	/*! Thread for rendering
	*/
	core::SaddyThread* m_renderthread; 
	/*! Main window of application
	*/
	MainPanel* m_mainwindow;
	/*! Application of qt, which is used
	*/
	QApplication* m_qtapp;
	/*! A hierarchical state machine
	 */
	sad::hfsm::Machine* m_machine;
	/*! A scene used for output
	*/
	sad::Scene* m_scene;
	/*! Command line arguments
	*/
	sad::cli::Args* m_cmdargs;
	/*! Mutex, that is used in initialize. DO NOT USE on other intensions
	*/
	sad::Mutex* m_initmutex;
	/*! Mutex, that is used in waiting of saddy thread. DO NOT use on other intensions
	*/
	sad::Mutex* m_saddywaitmutex;
	/*! Whether saddy thread must wait for qt thread
	*/
	bool m_waitforqt;
	/*! Whether main thread should wait for saddy thread
	*/
	bool m_waitforsaddy;
	/*! Command line options data
	*/
	sad::cli::Parser* m_cmdoptions;
	/*! History of data
	*/
	history::History* m_history;
	/*! Describes a behaviour shared data
	*/
	core::Shared* m_behavioursharedata;
	/*! An icons container
	*/
	sad::Sprite2DConfig m_icons;
	/*! Tests, whether saddy thread wait for qt
		\return should saddy awake or not
	*/
	inline bool shouldSaddyThreadWaitForQt() 
	{
		bool result;
		m_initmutex->lock();
		result = m_waitforqt;
		m_initmutex->unlock();
		return result;
	}
	/*! Tests, whether main thread wait for saddy
		\return should saddy awake or not
	*/
	inline bool shouldMainThreadWaitForSaddy() 
	{
		bool result;
		m_initmutex->lock();
		result = m_waitforsaddy;
		m_initmutex->unlock();
		return result;
	}
	/*! Awakes main thread
	*/
	inline void awakeMainThread() 
	{
		m_saddywaitmutex->lock();
		m_waitforsaddy = false;
		m_saddywaitmutex->unlock();
	}
	/*! Awakes  a saddy thread
	*/
	inline void awakeSaddyThread() 
	{
		m_initmutex->lock();
		m_waitforqt = false;
		m_initmutex->unlock();
	}
	/*! Makes thread wait for saddy thread
	*/
	void waitForSaddyThread();
};

}
