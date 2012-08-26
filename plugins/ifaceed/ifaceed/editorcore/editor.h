/*! \file editor.h
	\author HiddenSeeker

	Describes a global editor state
 */
#include "objects/abstractgameobject.h"
#include "os/mutex.h"
#include <QObject>
#include <QTimer>
#include <QThread>
#pragma once

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
	const char ** argv;
 public:
	/** Constructs a new command arguments
	    \param[in] argc amount of arguments
	    \param[in] argc arguments
	 */
	inline CommandArguments(int argc, const char ** argv) {
		this->argc = argc;
		this->argv = argv;
	}
	/** Returns argument count
		\return argument count
	 */
	inline int count() const { return this->argc; }
	/** Returns arguments
		\param[in] i number of argument
		\return arguments
	 */
	inline const char * argument(int i) const { return this->argv[i]; }

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
		 /** Command line arguments
		  */
		 CommandArguments * m_cmdargs;
		 /** Mutex, used to render all stuff
		  */
		 os::mutex * m_rendermutex;
		 /** Mutex, that is used in initialize. DO NOT USE on other intensions
		  */
		 os::mutex * m_initmutex;
		 /** Whether saddy thread must wait for qt thread
		  */
		 bool m_waitforqt;
	     /** Returns a command line arguments
			 \return command line arguments
		  */ 
		 inline CommandArguments * commandLineArguments() { return m_cmdargs;}

		 /** Tests, whether saddy thread wait for qt
			 \return should saddy awake or not
		  */
		 bool shouldSaddyThreadWaitForQt() 
		 {
			bool result = false;
			m_initmutex->lock();
			result = m_waitforqt;
			m_initmutex->unlock();
			return result;
		 }
		 /** Awakes  a saddy thread
		  */
		 void awakeSaddyThread() 
		 {
			m_initmutex->lock();
			m_waitforqt = false;
			m_initmutex->unlock();
		 }
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
  public:
		/** Default constructor
		 */
		Editor();
		/** Inits an editor, loading default data if nothing specified
			\param[in] argc count of arguments
			\param[in] argv arguments
		 */
		virtual void init(int argc,const char ** argv);

		/** Locks rendering for current thread to synchronize scene-vulnerable actions
		 */
		inline void lockRendering() { m_rendermutex->lock(); }
		/** Unlocks rendering
		 */
		inline void unlockRendering() { m_rendermutex->unlock();}
		
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

