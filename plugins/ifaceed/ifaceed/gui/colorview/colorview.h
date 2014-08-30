/*! \file colorview.h
	\author HiddenSeeker

	Implements a simple widget for displaying custom color. Used in cells of main panel
	for displaying color
 */
#pragma once
#include <QLabel>

namespace  gui
{
	
namespace colorview
{
	
class ColorView: public QLabel
{
Q_OBJECT
public:
	/*! Creates new view with default color
	 */
	ColorView(QWidget * parent = NULL);
	/*! Could be inherited
	 */
	virtual ~ColorView();
	/*! Sets background color for color view
		\param[in] c color
	 */
	void setBackgroundColor(const QColor& c);
	/*! Returns background color of color view
		\return color
	 */
	const QColor& backgroundColor() const;
signals:
	/*! Emitted, when user is clicked on item
	 */
	void clicked();
protected:
	/*! Used to emit clicked to on click
		\param[in] e event object
	 */
	virtual void mousePressEvent( QMouseEvent * e );
};

}

}
