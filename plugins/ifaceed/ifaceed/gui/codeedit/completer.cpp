#include "completer.h"

#include <QScriptValue>
#include <QStandardItemModel>
#include <QStandardItem>

gui::codeedit::Completer::Completer(QObject *parent)
: QCompleter(parent)
{
}

gui::codeedit::Completer::Completer(QAbstractItemModel *model, QObject *parent)
: QCompleter(model, parent)
{
}


 QStringList gui::codeedit::Completer::splitPath(const QString &path) const
 {
     return path.split(".");
 }

 QString gui::codeedit::Completer::pathFromIndex(const QModelIndex &index) const
 {
     QStringList dataList;
     for (QModelIndex i = index; i.isValid(); i = i.parent()) {
         dataList.prepend(model()->data(i, completionRole()).toString());
     }

     return dataList.join(".");
 }