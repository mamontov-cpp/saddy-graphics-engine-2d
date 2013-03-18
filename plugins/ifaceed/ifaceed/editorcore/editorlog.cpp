#include "editor.h"
#include "editorlog.h"
#include <QMessageBox>
#include <log.h>

EditorLog::~EditorLog() 
{
}


QString EditorLog::logMessage(int level) 
{
	QString result = "";
	switch(level) {
		case ELL_ERROR:   {result="ERROR"; break;   }
		case ELL_WARNING: {result="WARNING"; break; }
		case ELL_MESSAGE: {result="MESSAGE"; break; }
		case ELL_DEBUG:   {result="DEBUG"; break;}
	};
	return result;
}

QString EditorLog::contextTemplateMessage() 
{
	if (m_actions_stack.count() == 0) {
		return QString();
	}
	return QString(m_actions_stack[m_actions_stack.count()-1].data()) + QString(":");
}

void EditorLog::log(EditorLogLevel level, const QString & str, bool show_ui)
{
	if (level <= m_maxlevel)
	{
		QString logMessageFormat("[%1]: %2 %3\n");
		QString logMessage = logMessageFormat.arg(this->logMessage(level))
			                                 .arg(this->contextTemplateMessage())
                                             .arg(str);
		
		CLOSURE
		CLOSURE_DATA( 
			QString windowMessage; 
		    QString logClosureMessage;
			QString closureProgramName;
			EditorLogLevel closureLevel;
			bool closureShowUI;
		)
		CLOSURE_CODE( 
			if (closureLevel == ELL_ERROR) {
				QMessageBox::critical(NULL, closureProgramName, windowMessage);
			}
			if (closureLevel == ELL_WARNING) {
				QMessageBox::warning(NULL, closureProgramName, windowMessage);
			}
			if (closureLevel == ELL_MESSAGE && closureShowUI) {
				QMessageBox::information(NULL, closureProgramName, windowMessage);
			}
			hst::log::inst()->owrite(logClosureMessage.toStdString());
		)
		INITCLOSURE( 
			CLSET(windowMessage, str); 
			CLSET(logClosureMessage, logMessage);
			CLSET(closureProgramName, m_programname);
			CLSET(closureLevel, level);
			CLSET(closureShowUI, show_ui);
		) 
			if (QThread::currentThread() == QCoreApplication::instance()->thread()) 
			{
				ClosureBasic * a = ______.___();
				a->run();
				delete a;
			}
			else 
			{
				if (m_ed) 
					m_ed->emitClosure(______.___() );
			}
		}	
	}
}

void EditorLog::error( const QString & str) 
{
	this->log(ELL_ERROR, str);
}


void EditorLog::warning( const QString & str) 
{
	this->log(ELL_WARNING, str);
}

void EditorLog::message( const QString & str, bool show_ui )
{
	this->log(ELL_MESSAGE, str, show_ui);
}

void EditorLog::debug( const QString & str)
{
	this->log(ELL_DEBUG, str);
}

void  EditorLog::pushAction(const hst::string & str)
{
	this->debug(QString("Entering %1.").arg(str.data()));
	this->ActionContext::pushAction(str);
}

void EditorLog::popAction()
{
	this->debug("Leaving.");
	this->ActionContext::popAction();	
}

void EditorLog::save()
{
	hst::log::inst()->save(m_logfilename.toStdString().c_str());
}

LogActionMonitor::LogActionMonitor(EditorLog * l, const QString & a)
{
	m_log = l;
	m_log->pushAction(a.toStdString().c_str());
}

LogActionMonitor::~LogActionMonitor() 
{
	m_log->popAction();
}