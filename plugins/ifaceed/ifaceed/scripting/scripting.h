/*! \file scripting.h
	\author HiddenSeeker

	Describes a main class for scripting objects
 */
#pragma once
#include <QObject>
#include <QScriptEngine>
#include <QVector>
#include <QThread>
#include <QSet>

class MainPanel;

namespace scripting
{
	
/*! A group of actions, linked to animations
 */	
class Scripting: public QObject
{
Q_OBJECT
public:

/*! A polling thread, that polls engine and forces it to quit if need to
 */
class Thread: public QThread
{
public:
	/*! A timeout for quitting a thread
	 */
	static int TIMEOUT;
	/*! Determines, how often we should poll thread
	 */
	static int POLLINGTIME;
	/*! Creates new thread
		\param[in] me a thread
		\param[in] script a scripting element
	 */
	Thread(scripting::Scripting* me);
	/*! A linked thread
	 */
	virtual ~Thread();
	/*! Forces thread to quit
	 */
	void forceQuit();
	/*! Runs a thread, running script
	 */
	virtual void run();	
protected:
	/*! Whether we should quit 
	 */
	bool m_should_i_quit;
	/*! A scripting part
	 */
	scripting::Scripting* m_s;
};
	/*! Creates new label actions
		\param[in] parent a parent object
	 */
	Scripting(QObject* parent = NULL);
	/*! This class could be inherited
	 */
	virtual ~Scripting();
	/*! Sets a main panel for scripting
		\param[in] panel a panel
	 */
	void setPanel(MainPanel* panel);
	/*! Returns panel
		\return panel
	 */
	MainPanel* panel() const;
	/*! Returns an engine
		\return engine
	 */
	QScriptEngine* engine() const;
    /*! Registers function in object
       \param name name of function
       \param v a value representation of function
     */
    void registerFunction(const QString& name, QScriptValue& v);
	/*! Registerd script class as global function
	 */
	void registerScriptClass(const QString& name, QScriptClass* c);
    /*! Returns common properties, which all classes could have
        \return common properties
     */
    QSet<QString> commonProperties();
    /*! Returns all functions and properties, defined in system
        \param[out] properties list of properties
        \param[out] functions list of functions
     */
    void propertiesAndFunctions(
        QStringList& properties,
        QStringList& functions
    );
    /*! Appends all functions and properties, defined in system for given value
        \param[out] properties list of properties
        \param[out] functions list of functions
     */
    void propertiesAndFunctions(
        QSet<QString>& properties,
        QSet<QString>& functions,
        const QScriptValue& v
    );
public slots:
	/*! Run script in console
	 */
	void runScript();
	/*! Shows help
	 */
	void showHelp();
	/*! Cancels execution of script
	 */
	void cancelExecution();
	/*! Saves script to a file
	 */
	void saveScript();
	/*! Loads script from a file
	 */
	void loadScript();
protected:
	/*! Inits inner script with constructors for common types
	 */
	void initSadTypeConstructors();
	/*! Inits all bindings for scenes
        \param[out] v a global value (E)
	*/
	void initDatabasePropertyBindings(QScriptValue& v);
	/*! Inits scenes bindings for scenes
        \param[out] v a global value (E)
	 */
	void initSceneBindings(QScriptValue& v);
    /*! Inits scene nodes bindings
        \param[out] v a global value (E)
	 */
	void initSceneNodesBindings(QScriptValue& v);
    /*! Inits way bindings
        \param[out] v a global value (E)
     */
    void initWaysBindings(QScriptValue& v);
	/*! Inits dialogues bindings
        \param[out] v a global value (E)
     */
    void initDialoguesBindings(QScriptValue& v);
	/*! Inits animations bindings
        \param[out] v a global value (E)
     */
    void initAnimationsBindings(QScriptValue& v);
	/*! A panel for scripting
	 */
	MainPanel* m_panel;
	/*! An engine to be run
	 */
    QScriptEngine* m_engine;
	/*! A value, where all bindings are stored
	 */
	QScriptValue m_value;
	/*! A list of registered classes in engine
	 */
	QVector<QScriptClass*> m_registered_classes;
};

}
