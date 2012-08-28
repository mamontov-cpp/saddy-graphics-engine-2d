/** \file   ifaceeditor.h
	\author HiddenSeeker

	Defines a main interface editor class
 */
#include "../editorcore/editor.h"
#include "../mainpanel.h"

class IFaceEditor: public Editor
{
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
};
