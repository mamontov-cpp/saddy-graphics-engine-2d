/*! \file gui/codeedit/completer.h
	\author HiddenSeeker

	Defines a completer for javascript code
 */
#pragma once
#include <QCompleter>
#include <QScriptEngine>

namespace gui
{
	
namespace codeedit
{
	
class Completer: public QCompleter
{
Q_OBJECT

public:
/*! Constructs new completer
	\param[in] parent a parent object
 */
Completer(QObject *parent = 0);
/*! Constructs new completer from moded
	\param[in] model a model part
	\param[in] parent a parent object
 */
Completer(QAbstractItemModel *model, QObject *parent = 0);
protected:
/*! Splits path
	\param[in] path path
	\return path part
 */ 
QStringList splitPath(const QString &path) const;
/*! Builds path from index
	\param[in] index model index
	\return path
 */
QString pathFromIndex(const QModelIndex &index) const;
};

}

}
