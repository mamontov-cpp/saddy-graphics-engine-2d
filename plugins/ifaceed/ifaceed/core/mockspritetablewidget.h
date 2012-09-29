/** \file mockspritetablewidget.h
	\author HiddenSeeker

	SpriteTableWidget used to simulate all of the stuff to work with, due
	to currently nothing is presented on SpriteTableWidget
 */
#include <QComboBox>

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
	 }
	 /** Clears a widget
	  */
	 void clear();
	 /** Adds new data  to iterator
		 \param[in] iterator data
	  */
	 void add(const AbstractSpriteDatabaseIterator & i ) ;
};