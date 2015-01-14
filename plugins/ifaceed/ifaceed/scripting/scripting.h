/*! \file scripting.h
	\author HiddenSeeker

	Describes a main class for scripting objects
 */
#pragma once
#include <QObject>
#include <QScriptEngine>

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
    /*! Registers function in object
       \param name name of function
       \param v a value representation of function
     */
    void registerFunction(const QString& name, QScriptValue& v);
public slots:
	/*! Run script in console
	 */
	void runScript();
protected:
	/*! A panel for scripting
	 */
	MainPanel* m_panel;
	/*! An engine to be run
	 */
    QScriptEngine* m_engine;
};

}
