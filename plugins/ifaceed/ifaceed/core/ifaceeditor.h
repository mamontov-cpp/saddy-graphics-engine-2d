/** \file   ifaceeditor.h
	\author HiddenSeeker

	Defines a main interface editor class
 */
#include "../editorcore/editor.h"
#include "../mainpanel.h"

class IFaceEditor: public Editor
{
	Q_OBJECT
 protected:
	/** Sets my options and my data
	 */
	void initSaddyRendererOptions();
	/** Returns new main window
	    \return MainPanel
	 */
	QMainWindow * createQtWindow();
	/** Returns editor qt window
		\return MainPanel
	 */
	MainPanel * panel();
	/** Creates a new Qt slots for working
	  */
    void bindQtSlots();
	 /** REIMPLEMENT. the action, when user closes window of qt
	   */
	void onQtWindowDestroy();
	/** REIMPLEMENT. the action, when user closes window of qt
	  */
	void onSaddyWindowDestroy();
	/** REIMPLEMENT this function to do work, when qt window quit 
	  */
	void quitQtActions();
	/** REIMPLEMENT this function to do work, when saddy window quit 
	  */
	void quitSaddyActions();
};
