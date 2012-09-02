/*!  \file fontdelegate.h
	 \author HiddenSeeker

	 A delegate, used in combo box to draw some fonts in menu items
 */
#include <QItemDelegate>

class FontDelegate: public QItemDelegate
{
	/** Paints a menu item using
		\param[in] painter painter
		\param[in] option  options for rendering
		\param[in] index   data to draw
	 */
	void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	/** Returns a hints for size for
		\param[in] option  options for drawing
		\param[in] index   index of model
		\return size hint
     */
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};