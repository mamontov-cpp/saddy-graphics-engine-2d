/*! \file classwrapper.h
	

	Describes a base class for all class wrappers in projects
 */
#pragma once
#include <QScriptable>
#include <QObject>

namespace scripting
{
	
/*! A basic wrapper for object classes
 */
class ClassWrapper: public QObject, public QScriptable
{
Q_OBJECT
public:
	/*! Constructs new wrapper
	 */
	ClassWrapper();
	/*! Could be inherited
	 */
	virtual ~ClassWrapper();
	/*! Converts object to string
	 */
	virtual QString toString()  const = 0;
};


}