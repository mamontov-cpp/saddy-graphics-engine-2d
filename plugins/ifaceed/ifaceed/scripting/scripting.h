/*! \file scripting.h
	\author HiddenSeeker

	Describes a main class for scripting objects
 */
#pragma once
#include <QObject>
#include <QScriptEngine>
#include <QVector>

#include "constructorcall.h"

class MainPanel;

namespace scripting
{
	
/*! A group of actions, linked to animations
 */	
class Scripting: public QObject
{
Q_OBJECT
public:
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
public slots:
	/*! Run script in console
	 */
	void runScript();
	/*! Shows help
	 */
	void showHelp();
protected:
	/*! Inits inner script with constructors for common types
	 */
	void initSadTypeConstructors();
	/*! Inits all bindings for scenes
	 */
	void initSceneBindings(QScriptValue& v);
	/*! A panel for scripting
	 */
	MainPanel* m_panel;
	/*! An engine to be run
	 */
    QScriptEngine* m_engine;
	/*! A list of registered classes in engine
	 */
	QVector<QScriptClass*> m_registered_classes;
};

}
