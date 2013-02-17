/*! \file editorlogger.h
	\author HiddenSeeker

	A logger, for working with editor message. It is an advanced logger, for showing various messages
	with data.
 */
#pragma once
#include <QString>
#include <QStack>
#include <marshal/actioncontext.h>

class Editor;

enum EditorLogLevel 
{
	ELL_ERROR = 1,
	ELL_WARNING = 2,
	ELL_MESSAGE = 3,
	ELL_DEBUG = 4
};

#ifndef DEFAULT_LOGGER_LEVEL
	#define DEFAULT_LOGGER_LEVEL ELL_MESSAGE
#endif
/** Threas-safe editor log, which logs all events of editors.
	Provided, as useful replacement to all other logging interfaces, including
	QMessage::critical and QMessage::warning
 */
class EditorLog: public ActionContext {
 protected:
         Editor * m_ed; //!< Editor to work with
		 EditorLogLevel m_maxlevel; //!< Maximum level, which can be logged
		 QString        m_programname; //!< Name of program
		 QString        m_logfilename;  //!< Name of log file
		 /** Returns  a log message for level
		    \param[in] level level for message
			\return message
		 */
		QString logMessage(int level);
		/** Returns a context template message as a mesage for top level action + ":"
			\return message
		 */
		QString contextTemplateMessage();
 public:
		/*! Creates a new logger
		    \param[in] ed editor to log to
			\param[in] level maximum level which can be logged, Messages with this level are logged to
			\param[in] progtamname name of program, which is shown in message boxes
		 */
        inline EditorLog(Editor * ed, 
						 EditorLogLevel level = DEFAULT_LOGGER_LEVEL,
						 const QString & programname = "Editor",
						 const QString & logname = "log.txt")
        { 
	      m_ed = ed; 
	      m_maxlevel = level; 
		  m_programname = programname; 
		  m_logfilename = logname; 
		}
		/** Sets maximum logged level
			\param[in] level used level
		 */
		inline void setMaxLevel(EditorLogLevel level) 
		{
			m_maxlevel = level;
		}
		/** Sets program name
			\param[in] name used level
		 */
		inline void setProgramName(const QString & name) 
		{
			m_programname = name;
		}
		/** Sets log file name
			\param[in] name used level
		 */
		inline void setLogFile(const QString & name) 
		{
			m_logfilename = name;
		}


		/*! Logs a message into log	
			\param[in] log level for data
			\param[in] str string to be in
			\param[in] show_ui Whether message stuff should be shown on some line
		 */
		virtual void log(EditorLogLevel level, const QString & str, bool show_ui = false);
        /*! Shows a visual message on UI and logs it
			\param[in] st string data
		 */
		virtual void error( const QString & str);
		/*! Shows a visual message on UI and logs it
			\param[in] st string data
		 */
		virtual void warning( const QString & str);
		/*! Logs a special message
			\param[in] st string data
			\param[in] show_ui shows message in ui
		 */
		virtual void message( const QString & str, bool show_ui = false);
		/*! Logs a debug data
			\param[in] st string data
		 */
		virtual void debug( const QString & str);
		/*! Sets current action
			\param[in] str string
		 */
	    virtual void  pushAction(const hst::string & str);
		/*!  Pops an actions
		 */
		virtual void popAction();
		/*!  Saves log to file
		 */
		virtual void save();
		/*! Destroys a log
		 */
		virtual ~EditorLog();
};

class LogActionMonitor 
{
 protected:
	  EditorLog * m_log;
 public:
	  /** Marks an editor in action
		  \param[in] l log
		  \param[in] a action name
	   */
	  LogActionMonitor(EditorLog * l, const QString & a);
	  /** Pushes action from log
	   */
	  ~LogActionMonitor();
};