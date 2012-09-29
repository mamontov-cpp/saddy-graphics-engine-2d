/** \file mockspritetablewidget.h
	\author HiddenSeeker

	SpriteTableWidget used to simulate all of the stuff to work with, due
	to currently nothing is presented on SpriteTableWidget
 */
#include <QComboBox>
#include "qspritetablewidgetselection.h"

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
		clear();
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
	  */
	 QSpriteTableWidgetSelection selection();
 signals:
	 /** Emitted, when sprite is selected
		 \param[in] config config data
		 \param[in] group group data
		 \param[in] index index data
	  */
	 void spriteSelected(QString config, QString group, int index);
};