/*! \file multimethod.h
	\author HiddenSeeker

	Defines a multimethod for scripting
 */
#pragma once
#include <QVector>

#include "callable.h"

namespace scripting
{

/*! A callable multimethod, which represents a choice between several signatures
 */ 
class MultiMethod: public scripting::Callable
{
public:
	/*! Represents a call for a function with two arguments
		\param[in] e engine
		\param[in] name name of function call
	 */
	MultiMethod(QScriptEngine* e, const QString& name);
	/*! Can be inherited
	 */
	virtual ~MultiMethod();
	/*! Determines, whether it can be called with this context
		\param[in] ctx context
	 */
	virtual scripting::MatchResult canBeCalled(QScriptContext* ctx);
	/*! Calls actually a function
		\param[in] ctx context
		\param[in] engine engine
	 */
	virtual QScriptValue call(QScriptContext* ctx, QScriptEngine* engine);
	/*! Adds new callable
		\param[in] c class
	 */
	void add(scripting::Callable* c);
protected:
	/*! A callable methods
	 */
	QVector<scripting::Callable*> m_methods;
};

}
