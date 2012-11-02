#ifndef QSPRITETABLEWIDGET_H
#define QSPRITETABLEWIDGET_H

#include <QtCore>
#include <QtGui>
#include "../../core/spritedatabase.h"
#include "../../core/qspritetablewidgetselection.h"
#include "celldelegate.h"



class QSpriteTableWidget : public QWidget
{

	Q_OBJECT

	QComboBox*     m_configCombo;

	QTableWidget*  m_viewer;
	QList<CellInfo> m_values;


	int m_curRow;
	int m_curCol;

	QList<QString> getGroups(QString configName);
	QList<QString> getIndexes(QString configName, QString group);
	bool isCellExists(CellInfo& cellInfo);
	void buildCombo();
	void rebuildTable();


public:
	QSpriteTableWidget(QComboBox * configCombo, QLayout* layout);
	/** Sets current selection
	\param[in] sel selection object
	*/
	void setSelection(const QSpriteTableWidgetSelection & sel);
	/** Returns current selection
	\return selection object
	*/
	QSpriteTableWidgetSelection selection();
	/** Adds new sprite to spriteviewer
	\param[in] it iterator of the sprite DB
	*/
	void add(const AbstractSpriteDatabaseIterator & i);

	void addToForm(QLayout* layout);

	void clear();

	void finishSyncronizing();
signals:
	void spriteSelected(QString config, QString group, int index);

private slots:
	void on_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn );
	void configChanged(int index);
};

#endif