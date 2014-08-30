#include "colorview.h"

gui::colorview::ColorView::ColorView(QWidget* parent) : QLabel(parent)
{
	// A padding to make default size bigger
	this->setText(QString(" ").repeated(20));
}

gui::colorview::ColorView::~ColorView()
{
	
}

void gui::colorview::ColorView::setBackgroundColor(const QColor& c)
{
	 QPalette palette = this->palette();
	 palette.setColor(this->backgroundRole(), c);
	 this->setPalette(palette);
}

const QColor& gui::colorview::ColorView::backgroundColor() const
{
	QPalette palette = this->palette();
	return palette.color(this->backgroundRole());
}

void gui::colorview::ColorView::mousePressEvent(QMouseEvent * e)
{
	emit clicked();
}
