#include "dbcriticallogger.h"
#include "editor.h"
#include <QMessageBox>

DBCriticalLogger::~DBCriticalLogger()
{

}

void EditorCriticalLogger::critical( const QString & str)
{ 
	CLOSURE
    CLOSURE_DATA( QString str; )
    CLOSURE_CODE( QMessageBox::critical(NULL, "IFace Editor", str); )
    INITCLOSURE( CLSET(str, str); )
    SUBMITCLOSURE( m_ed->emitClosure );              
}

EditorCriticalLogger::~EditorCriticalLogger()
{

}