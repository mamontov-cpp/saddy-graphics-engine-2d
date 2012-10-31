/*! \file dbcriticallogger.h
	\author HiddenSeeker

	Class, that can emit critical log messages
 */
#pragma once
#include <QString>
/*! Class,that can emit critical logging from other objects
 */
class DBCriticalLogger
{
 public:
	/*! Emits a critical message for str
		\param[in] str string
	 */
	virtual void  critical( const QString & str)=0;
	/*! Destroys a logger
	 */
	virtual ~DBCriticalLogger();
};

class Editor;

/*! Special editor's logger, for logging some real data
 */
class EditorCriticalLogger: public DBCriticalLogger
{
 private:
         Editor * m_ed; //!< Editor to work with
 public:
        inline EditorCriticalLogger(Editor * ed)
        { m_ed = ed; }
        virtual void critical( const QString & str);
        virtual ~EditorCriticalLogger();
};