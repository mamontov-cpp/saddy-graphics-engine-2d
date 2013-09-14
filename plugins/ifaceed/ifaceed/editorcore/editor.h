/*! \file editor.h
	\author HiddenSeeker

	Describes a global editor state
 */
#include <scene.h>
#include <QObject>
#include <QTimer>
#include <QThread>
#include <QApplication>
#include <QMainWindow>
#include <os/mutex.h>
#include <renderer.h>
#include <marshal/actioncontext.h>
#include <input.h>
#include <config/sprite2dconfig.h>
#include <cmdoptions.h>
#include "editoreventhandler.h"
#include "../history/editorhistory.h"
#include "templates/closure.h"
#include "qttarget.h"
#pragma once


#define WINDOW_WIDTH 800

#define WINDOW_HEIGHT 600

/** Time, all rotation wheel events within can be counted as one (in milliseconds)
 */
#define MAX_ROTATION_TIME 1500
/** Rotation angle step
 */
#define ROTATION_ANGLE_STEP 0.07
/**	Icons xml data
 */
#define ICONS_XML  "resources/icons.xml"

enum EditorQuitReason
{
	EditorQuitReasonNotSet = 0,
	QuitBySaddy = 1,
	QuitByQtWindow = 2
};

class Editor;
class EditogLog;
class EditorBehaviour;
class EditorBehaviourSharedData;
class AbstractScreenObject;
/** Interlocked scene, used to iterate while rendering
 */
class InterlockedScene: public sad::Scene
{
 private:
	      /** A parent editor
		   */
		  Editor * m_editor;
		  /*! Deletes node on default, removes reference if AbstractScreenObject
			  \param[in] node node to be removed
		   */
		  virtual void onNodeRemoval(sad::BasicNode * node);
 public:
		  /** Constructs new interlocked scene
		   */
	      inline InterlockedScene(Editor * ed) { this->m_editor = ed;}
		  /** Adds an object to scene
			  \param[in] obj object to be added
		   */
		  virtual void add(AbstractScreenObject * obj);
		  /** Removes an object from scene
			  \param[in] rem removed object
		   */
		  virtual void remove(AbstractScreenObject * rem);
		  /** Renders a scene
		   */
		  virtual void render();
		  /** A scene
		   */ 
		  virtual ~InterlockedScene();
};

/** Thread for rendering
  */
class SaddyThread: public QThread 
{
Q_OBJECT
friend class Editor;
private:
	/** Editor to work with
	  */
	Editor * m_editor;
public:
	/** Constructs a children rendering thread. Editor is needed to
	    send a signal to run Qt event loop
	    \param[in] editor editor to run
	  */
	inline SaddyThread(Editor * editor) { m_editor = editor;}
	/** Awaits for qt thread to do his job
	  */
	void waitForQtThread();
	/** Runs a thread to do stuff
	  */
	virtual void run();	
};


/*! \class Editor
	
	Describes a global editor state
 */
class Editor: public QObject
{
	Q_OBJECT
  friend class SaddyThread;
  friend class QtTarget;
  private:
	     /** Target for sending information
		  */
		 QtTarget * m_target;
	     /** Thread for rendering
		  */
		 SaddyThread * m_renderthread; 
		 /** Main window of application
		  */
		 QMainWindow  * m_mainwindow;
	     /** Application of qt, which is used
		  */
		 QApplication * m_qtapp;
		 /** A scene used for output
		  */
		 sad::Scene * m_scene;
		 /** Command line arguments
		  */
		 sad::cmd::Args * m_cmdargs;
		 /** Mutex, used to render all stuff
		  */
		 os::mutex * m_rendermutex;
		 /** Mutex, that is used in initialize. DO NOT USE on other intensions
		  */
		 os::mutex * m_initmutex;
		 /** Mutex, that is used in waiting of saddy thread. DO NOT use on other intensions
		  */
		 os::mutex * m_saddywaitmutex;
		 /** Whether saddy thread must wait for qt thread
		  */
		 bool m_waitforqt;
		 /** Whether main thread should wait for saddy thread
		  */
		 bool m_waitforsaddy;
		 /** Whether saddy initialization was successfull
		  */
		 bool m_saddystartedok;
		 /** Command line options data
		  */
		 sad::cmd::Parser * m_cmdoptions;
		 /** History of data
		  */
		 EditorHistory * m_history;
		 /** Describes a behaviour shared data
		  */
		 EditorBehaviourSharedData * m_behavioursharedata;
		 /** An icons container
		  */
		 Sprite2DConfig m_icons;
protected:
		/** A defines editor behaviours
		 */
	    hst::hash<hst::string, EditorBehaviour *> m_behaviours;
	    /** A current behaviour
		 */
		hst::string m_current_behaviour;
		 /** A reason, while saddy quit
		  */
		 EditorQuitReason  m_quit_reason;
		 /** Creates a parser to parse command options
			 \return new command line options
		  */
		 virtual sad::cmd::Parser * createOptionParser() = 0;
private:
		 /** Tests, whether saddy thread wait for qt
			 \return should saddy awake or not
		  */
		 inline bool shouldSaddyThreadWaitForQt() 
		 {
			bool result = false;
			m_initmutex->lock();
			result = m_waitforqt;
			m_initmutex->unlock();
			return result;
		 }
		 /** Tests, whether main thread wait for saddy
			 \return should saddy awake or not
		  */
		 inline bool shouldMainThreadWaitForSaddy() 
		 {
			bool result = false;
			m_initmutex->lock();
			result = m_waitforsaddy;
			m_initmutex->unlock();
			return result;
		 }
		 /** Awakes main thread
		  */
		 inline void awakeMainThread() 
		 {
			m_saddywaitmutex->lock();
			m_waitforsaddy = false;
			m_saddywaitmutex->unlock();
		 }
		 /** Awakes  a saddy thread
		  */
		 inline void awakeSaddyThread() 
		 {
			m_initmutex->lock();
			m_waitforqt = false;
			m_initmutex->unlock();
		 }
		 void waitForSaddyThread();
		 /** Default saddy options
		  */
		 void initDefaultSaddyOptions();
	     /** Whether saddy init was successfukl
		  */
		 inline bool saddyInitSuccessfull() { return this->m_saddystartedok; }
  protected:
		 /** Assert that saddy initted succesed or failed
		  */
		 inline void assertSaddyInit( bool success) { this->m_saddystartedok = success;}
		 /** Returns a command line arguments
		 	 \return command line arguments
		  */ 
		 inline sad::cmd::Args * commandLineArguments() { return m_cmdargs;}
		 /** Returns a qt application
			 \return qt application
		  */
		 inline QApplication * qtApp() { return this->m_qtapp;}
		 /** Returns a qt main window
			 \return qt main window, used in program
		  */
		 inline QMainWindow * qtWindow() { return this->m_mainwindow; }

		 /** REIMPLEMENT this function to pass tour own shared data
		  */
		 virtual EditorBehaviourSharedData * createBehaviourData();
		 /** REIMPLEMENT this function to pass your own renderer settings into saddy
		  */
		 virtual void initSaddyRendererOptions();
		 /** REIMPLEMENT this function to create a qt window
			 \return null
		  */
		 virtual QMainWindow * createQtWindow();
		 /** REIMPLEMENT this function to create a slots to a qt windows
		  */
		 virtual void bindQtSlots();
		 /** REIMPLEMENT. the action, when user closes window of qt
		  */
		 virtual void onQtWindowDestroy();
		 /** REIMPLEMENT. the action, when user closes window of qt
		  */
		 virtual void onSaddyWindowDestroy();

		 /** REIMPLEMENT this function to do work, when qt window quit 
		  */
		 virtual void quitQtActions();
		/** REIMPLEMENT this function to do work, when saddy window quit 
		  */
		 virtual void quitSaddyActions();

		 
		 /** A quit slot for saddy, which is run when user runs saddy
		  */
		 void saddyQuitSlot();
		 /** Saddy destruction slot
		  */
		 void onSaddyWindowDestroySlot(const sad::Event & ev);
  protected slots:
		 /** A method to init all saddy actiona
		  */
	     virtual void initSaddyActions();
		 /** A method to init all qt actions
		  */
		 virtual void initQtActions();
		 /** Runs qt event loop (qt app)
		  */
		 virtual void runQtEventLoop();
		 /** Runs saddy event loop saddy event loop
		  */
		 virtual void runSaddyEventLoop();
		 /** Contains a different actions, which is runned from main thread
			  use variable @see m_quit_reason for reason of exit
		  */
		 void onQuitActions();
		 /** A function, which is called, when app fully started from main start
		  */
		 virtual void onFullAppStart();
		 /** Runs, when qt quits 
		  */
		 void qtQuitSlot();
		 /** Runs a closure. Used by qt thread to work with closure.
			 \param[in] closure closure data
		  */
		 virtual void onClosureArrived(sad::ClosureBasic * closure);		 
  public:
		/*! Default constructor
		 */
		Editor();
		/*! Returns parsed arguments
			\return parsed arguments
		 */
		sad::cmd::Parser * parsedArgs() const;
		 /** Returns a scene
			 \return used scene
		  */
		inline sad::Scene * scene() { return this->m_scene; }
	    /*! Returns a behaviour shated data, needed to implement some stuff
			\return shared data for behaviour
		 */
		inline EditorBehaviourSharedData * behaviourSharedData() 
		{
			return m_behavioursharedata;
		}
		/*! Returns a log for utils
		 */
		inline sad::Log * log() { return sad::Log::ref(); }
		/*! Inits an editor, loading default data if nothing specified
			\param[in] argc count of arguments
			\param[in] argv arguments
		 */
		virtual void init(int argc,char ** argv);

		/** Locks rendering for current thread to synchronize scene-vulnerable actions
		 */
		inline void lockRendering() { m_rendermutex->lock(); }
		/** Unlocks rendering
		 */
		inline void unlockRendering() { m_rendermutex->unlock();}
		/** Returns a history
			\return history
		 */
		inline EditorHistory * history() { return m_history; }
		/** Quits an editor
		 */
		virtual void quit();
		/** Returns a behaviour data
		 */
		inline hst::hash<hst::string, EditorBehaviour *> & behaviours() 
		{
			return m_behaviours;
		}
	    /** Erases a current behaviour for work and sets for nothing
		 */
		void eraseBehaviour();
		/** Highlights a state of editor in GUI. Override this for custom output
			\param[in] hint hint of state
		 */
		virtual void highlightState(const hst::string & hint);
	    /** Deactivates old behaviour and enters new behaviour
			\param[in]  name name of new behaviour
		 */
		virtual void setBehaviour(const hst::string & name);
		/** Enters a behaviour of editor
			\return editor behaviour
		 */
		EditorBehaviour * currentBehaviour();
		/** Posts a behaviour callback event for data
		 */
		void postBehaviourCallback( void (EditorBehaviour::*cb)(const sad::Event & ev), const sad::Event & ev);
		/** Removes a command arguments data
		 */
		~Editor();
		/** Emits a closure signal closureArrived()
			\param[in] closure closure signal arrived
		 */
		void emitClosure(sad::ClosureBasic * closure);
		/** Casts a current behaviour state identified by state to needed
			\param[in] s string name of state
			\return state
		 */
		template<typename T> T * cbStateAs(const hst::string & s) {
			return static_cast<T *>(this->currentBehaviour()->getState(s));
		}
		/** Returns an icon container
		 */
		Sprite2DConfig & icons();
  signals:
		/** Signal is emitted, when closure is arrived
			\param[in] closure data for closure
		 */
		void closureArrived(sad::ClosureBasic * closure);
		
};



