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
	     /** Returns a command line arguments
			 \return command line arguments
		  */ 
		 inline CommandArguments * commandLineArguments() { return m_cmdargs;}
  private slots:
		 /** A method to init all saddy actiona
		  */
	     void initSaddyActions();
		 /** A method to init all qt actions
		  */
		 void initQtActions();
		 /** Runs qt event loop (qt app)
		  */
		 void runQtEventLoop();
		 /** Runs saddy event loop saddy event loop
		  */
		 void runSaddyEventLoop();
  public:
		/** Default constructor
		 */
		Editor();
		/** Inits an editor, loading default data if nothing specified
			\param[in] argc count of arguments
			\param[in] argv arguments
		 */
		void init(int argc,const char ** argv);

		/** Locks rendering for current thread to synchronize scene-vulnerable actions
		 */
		inline void lockRendering() { m_rendermutex->lock(); }
		/** Unlocks rendering
		 */
		inline void unlockRendering() { m_rendermutex->unlock();}
		
		/** Quits an editor
			\param[in] quits editor
		 */
		void quit();
		/** Removes a command arguments data
		 */
		~Editor();
};

/** A global editor, used to do stuff
 */
extern Editor * GlobalEditor;

