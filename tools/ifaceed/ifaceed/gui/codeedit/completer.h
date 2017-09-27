/*! \file gui/codeedit/completer.h
    

    Defines a completer for javascript code
 */
#pragma once
#include <QCompleter>
#include <QHash>
#include <QSet>
#include <QString>
#include <QStandardItemModel>
#include "dukqt.h"

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
    /*! Could be inherited
     */
    virtual ~Completer();
    /*! Constructs model from engine
        \param[in] ctx context
        \param[in] firstlevelreplace an entries, which must be replaced at top level
        \param[in] commonset a set of common properties to be added to root
     */
    QAbstractItemModel* modelFromEngine(
        dukpp03::qt::Context* ctx,
        const QHash<QString, QString>& firstlevelreplace,
        const QSet<QString>& commonset
    );
    /*! Scans value, filling parent item
        \param[in, out] parent a parent item
        \param[in] ctx context, where value is stored
     */
    static void scanValue(
        QStandardItem* parent,
        dukpp03::qt::Context* ctx
    );
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
