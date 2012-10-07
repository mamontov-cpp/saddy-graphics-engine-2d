/*! \file   saddythread.h
 *  \author HiddenSeeker
 *
 *  Here described a thread for running saddy
 */
#include <QThread>
#include <QObject>

/** A Thread for running saddy
 */ 
class SaddyThread: public QThread
{
 Q_OBJECT
 public:
	    void run();	
};
