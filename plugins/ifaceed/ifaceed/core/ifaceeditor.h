/** \file   ifaceeditor.h
	\author HiddenSeeker

	Defines a main interface editor class
 */
#include "../editorcore/editor.h"
#include "../mainpanel.h"
#include "ifacecmdoptions.h"



class IFaceEditor: public Editor
{
	Q_OBJECT
 protected:
	/** Creates a parser to parse command options
      */
    virtual CommandLineOptions * createOptionParser();
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
	/** Returns parsed options
		\return parsed options
	  */
	inline IFaceCmdOptions * cmdLineOptions()
	{
		return static_cast<IFaceCmdOptions *>(this->commandLineOptions());
	}
 public:
	/** Quits an editor
	 */
	void quit(const sad::Event & ev = sad::Event());
};

/** A special handler for method
 */
typedef HandlerFor<IFaceEditor>::Method<void (IFaceEditor::*)(const sad::Event &o),
										sad::Event
										> IFaceEditorHandler;