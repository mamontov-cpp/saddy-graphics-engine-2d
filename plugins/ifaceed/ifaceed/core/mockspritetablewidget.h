/** \file mockspritetablewidget.h
	\author HiddenSeeker

	SpriteTableWidget used to simulate all of the stuff to work with, due
	to currently nothing is presented on SpriteTableWidget
 */
#include <QComboBox>
#include <QHash>
#include "qspritetablewidgetselection.h"
#pragma once

class  AbstractSpriteDatabaseIterator;
/** A table widget, used to work with all of data
 */
class MockSpriteTableWidget: public QObject
{
 Q_OBJECT
 protected:
	QComboBox * m_configcombo;   //!< Config combo
	QComboBox * m_groupcombo;    //!< Group combo
	QComboBox * m_indexcombo;    //!< Index combo
	QHash<QString, QHash<QString, QVector<int> > > m_values; //!< Values of data

	bool m_emit_event; //!< Whether we should emit event of changing on changing
 private:
	  /** Connect slots of data
	   */
	  void connectSlots();
	  /** Refills a combo with actual data
	   */
	  void rebuildWidget();
	  /** Returns an index of item in combo box by text. -1 if not found
		  \param[in] cmb combo box
		  \param[in] text text of item
		  \return index of data
	   */
	  int indexByText(QComboBox * cmb, const QString & text);
 public:
	 /** Constructs new widget
		\param[in] configCombo config combo box
		\param[in] groupCombo group combo box
		\param[in] indexCombo index combo box
	  */
	 inline MockSpriteTableWidget(QComboBox * configCombo, 
								  QComboBox * groupCombo,
								  QComboBox * indexCombo)
	 {
		m_configcombo = configCombo;
		m_groupcombo = groupCombo;
		m_indexcombo = indexCombo;
		m_emit_event = true;
		clear();
		connectSlots();
	 }
	 /** Clears a widget
	  */
	 void clear();
	 /** Adds new data  to iterator
		 \param[in] iterator data
	  */
	 void add(const AbstractSpriteDatabaseIterator & i ) ;
	 /** Sets a selection for data
		 \param[in] selection of data
	  */
	 void setSelection( const QSpriteTableWidgetSelection & selection);

	 
	 /** Returns a selection of data
		 \return selection of data
	  */
	 QSpriteTableWidgetSelection selection();
 private slots:
	 /** Emitted if config changed
	  */
	 void configChanged(int index);
	 /** Emitted if group changed
		 \param[in] index data
	  */
	 void groupChanged(int index);
	 /** Emitted if index changed
		 \param[in] index index data
	  */
	 void indexChanged(int index);
 signals:
	 /** Emitted, when sprite is selected
		 \param[in] config config data
		 \param[in] group group data
		 \param[in] index index data
	  */
	 void spriteSelected(QString config, QString group, int index);
};