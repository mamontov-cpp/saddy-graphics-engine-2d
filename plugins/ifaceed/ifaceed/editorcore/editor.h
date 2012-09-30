/*! \file editor.h
	\author HiddenSeeker

	Describes a global editor state
 */
#include "objects/abstractgameobject.h"
#include <QObject>
#include <QTimer>
#include <QThread>
#include <QApplication>
#include <QMainWindow>
#include <os/mutex.h>
#include <renderer.h>
#include "editoreventhandler.h"
#include <input.h>
#include "commandlineoptions.h"
#include "../history/editorhistory.h"
#pragma once

enum EditorQuitReason
{
	EditorQuitReasonNotSet = 0,
	QuitBySaddy = 1,
	QuitByQtWindow = 2
};

class Editor;

/** Interlocked scene, used to iterate while rendering
 */
class InterlockedScene: public sad::Scene
{
 private:
	      /** A parent editor
		   */
		  Editor * m_editor;
 public:
		  /** Constructs new interlocked scene
		   */
	      inline InterlockedScene(Editor * ed) { this->m_editor = ed;}
		  /** Renders a scene
		   */
		  virtual void render();
		  /** A scene
		   */ 
		  virtual ~InterlockedScene();
};

/** \class CommandArguments
	
	Command arguments, passed into editor
 */
class CommandArguments 
{
 private:
	/** Argument count
	  */
	int argc;
	/** Passed arguments
	  */
	char ** argv;
 public:
	/** Constructs a new command arguments
	    \param[in] argc amount of arguments
	    \param[in] argc arguments
	 */
	inline CommandArguments(int argc, char ** argv) {
		this->argc = argc;
		this->argv = argv;
	}
	/** Returns argument count
		\return argument count
	 */
	inline int count() const { return this->argc; }

	/** Returns argument count
		\return argument count
	 */
	inline int& mutableCount() { return this->argc; }
	/** Returns arguments
		\param[in] i number of argument
		\return arguments
	 */
	inline const char * argument(int i) const { return this->argv[i]; }
	/** Returns full arguments array
		\return arguments
	 */
	char ** fullArgv() { return this->argv; }
};

/*! \class Editor
	
	Describes a global editor state
 */
class Editor: public QObject
{
	Q_OBJECT

  private:
		 /** Thread for rendering
		  */
	     class SaddyThread: public QThread 
	     {
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
	     } * m_renderthread;
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
		 CommandArguments * m_cmdargs;
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
		 CommandLineOptions * m_cmdoptions;
		 /** History of data
		  */
		 EditorHistory * m_history;
protected:
		 /** A reason, while saddy quit
		  */
		 EditorQuitReason  m_quit_reason;
		 /** Creates a parser to parse command options
			 \return new command line options
		  */
		 virtual CommandLineOptions * createOptionParser() = 0;
		 /** Returns parsed options
			 \return parsed options
		  */
		 inline CommandLineOptions * commandLineOptions()
		 {
			 return m_cmdoptions;
		 }
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
		 inline CommandArguments * commandLineArguments() { return m_cmdargs;}
		 /** Returns a qt application
			 \return qt application
		  */
		 inline QApplication * qtApp() { return this->m_qtapp;}
		 /** Returns a qt main window
			 \return qt main window, used in program
		  */
		 inline QMainWindow * qtWindow() { return this->m_mainwindow; }
		 /** Returns a scene
			 \return used scene
		  */
		 inline sad::Scene * scene() { return this->m_scene; }
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
  public:
		/** Default constructor
		 */
		Editor();
		/** Inits an editor, loading default data if nothing specified
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
			\param[in] quits editor
		 */
		virtual void quit();
		/** Removes a command arguments data
		 */
		~Editor();
};

/** A global editor, used to do stuff
 */
extern Editor * GlobalEditor;

