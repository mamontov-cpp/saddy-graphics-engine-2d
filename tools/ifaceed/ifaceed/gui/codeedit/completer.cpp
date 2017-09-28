#include "completer.h"

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
    dukpp03::qt::Context* ctx,
    const QHash<QString, QString>& firstlevelreplace,
    const QSet<QString>& commonset
)
{
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

    duk_context* c = ctx->context();
    duk_push_global_object(c);
    duk_enum(c, -1, DUK_ENUM_OWN_PROPERTIES_ONLY);
    while(duk_next(c, -1, 1))
    {
        QString name = duk_get_string(c, -2);
        if (name != "prototype" && name != "__prototype__" && name != "constructor")
        {
            if (firstlevelreplace.contains(name))
            {
                QStandardItem *item = new QStandardItem();
                item->setText(firstlevelreplace[name]);
                root->appendRow(item);
            }
            else
            {
                QStandardItem *item = new QStandardItem();
                item->setText(name);
                root->appendRow(item);
                if (duk_is_object(c, -1))
                {
                    gui::codeedit::Completer::scanValue(item, ctx);
                }
            }
        }
        duk_pop_2(c);
    }
    duk_pop(c);
    duk_pop(c);
    return model;
}


void gui::codeedit::Completer::scanValue(QStandardItem* parent, dukpp03::qt::Context* ctx)
{
    duk_context* c = ctx->context();
    if (duk_is_object(c, -1))
    {
        duk_enum(c, -1, DUK_ENUM_OWN_PROPERTIES_ONLY);
        while(duk_next(c, -1, 1))
        {
            QString name = duk_get_string(c, -2);
            if (name != "prototype" && name != "__prototype__" && name != "constructor")
            {
                QStandardItem *item = new QStandardItem();
                item->setText(name);
                parent->appendRow(item);
                if (duk_is_object(c, -1))
                {
                    gui::codeedit::Completer::scanValue(item, ctx);
                }
            }
            duk_pop_2(c);
        }
        duk_pop(c);
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


