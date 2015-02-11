#include "completer.h"

#include <QScriptValue>
#include <QScriptValueIterator>
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

gui::codeedit::Completer::~Completer()
{

}

QAbstractItemModel* gui::codeedit::Completer::modelFromEngine(
    QScriptEngine* e,
    const QHash<QString, QString>& firstlevelreplace,
    const QSet<QString>& commonset
)
{
    QScriptValue global = e->globalObject();
    QStandardItemModel* model = new QStandardItemModel(this);
    QStandardItem* root = model->invisibleRootItem();
    {
        for(QSet<QString>::const_iterator it = commonset.begin();
            it != commonset.end();
            ++it)
        {
            QStandardItem *item = new QStandardItem();
            item->setText(*it);
            root->appendRow(item);
        }
    }

    QScriptValueIterator it(global);
    while(it.hasNext())
    {
        it.next();
        if (it.name() != "prototype" && it.name() != "__prototype__" && it.name() != "constructor")
        {
            QString name = it.name();
            if (firstlevelreplace.contains(name))
            {
                QStandardItem *item = new QStandardItem();
                item->setText(firstlevelreplace[name]);
                root->appendRow(item);
            }
            else
            {
                QStandardItem *item = new QStandardItem();
                item->setText(it.name());
                root->appendRow(item);
                gui::codeedit::Completer::scanValue(item, it.value());
            }
        }
    }
    return model;
}


void gui::codeedit::Completer::scanValue(QStandardItem* parent, const QScriptValue& v)
{
    QScriptValueIterator it(v);
    while(it.hasNext())
    {
        it.next();
        if (it.name() != "prototype" && it.name() != "__prototype__" && it.name() != "constructor")
        {
            QString name = it.name();
            QStandardItem *item = new QStandardItem();
            item->setText(name);
            parent->appendRow(item);
            gui::codeedit::Completer::scanValue(item, it.value());
        }
    }
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


