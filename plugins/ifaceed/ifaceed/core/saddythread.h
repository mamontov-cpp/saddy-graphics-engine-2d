/*! \file saddythread.h
	\author HiddenSeeker

	Defines a thread object, where renderer of Saddy is being run
 */
#pragma once
#include <QThread>


namespace core
{
class Editor;

/*! Defines a thread object, where renderer of Saddy is being run
 */
class SaddyThread: public QThread 
{
Q_OBJECT
friend class Editor;
private:
	/*! Editor to work with
	 */
	core::Editor * m_editor;
public:
	/*! Constructs a children rendering thread. Editor is needed to
	    send a signal to run Qt event loop
	    \param[in] editor editor to run
	 */
	SaddyThread(core::Editor * editor);
	/*! Awaits for qt thread to do his job
	 */
	void waitForQtThread();
	/*! Runs a thread to do stuff
	 */
	virtual void run();	
};

}
