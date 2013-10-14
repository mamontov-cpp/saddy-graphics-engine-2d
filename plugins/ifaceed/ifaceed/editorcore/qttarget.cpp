#include "qttarget.h"
#include "editor.h"

QtTarget::~QtTarget()
{

}


void QtTarget::receive(const sad::log::Message & message)
{
	if (m_enabled)
	{
		if (message.priority() == sad::log::FATAL)
		{
			this->critical(message.message());
		}
		if (message.priority() == sad::log::WARNING)
		{
			this->warning(message.message());
		}

	}
}

void QtTarget::critical(const sad::String & m)
{
	
	CLOSURE
	CLOSURE_DATA(Editor * ed; QString w;)
	CLOSURE_CODE(
		QMessageBox::critical(ed->qtWindow(), "", w);
	)
	INITCLOSURE( CLSET(ed, m_editor); CLSET(w, m.data()); )
	SUBMITCLOSURE( m_editor->emitClosure );

}

void QtTarget::warning(const sad::String & m)
{
	
	CLOSURE
	CLOSURE_DATA(Editor * ed; QString w;)
	CLOSURE_CODE(
		QMessageBox::warning(ed->qtWindow(), "", w);
	)
	INITCLOSURE( CLSET(ed, m_editor); CLSET(w, m.data()); )
	SUBMITCLOSURE( m_editor->emitClosure );

}