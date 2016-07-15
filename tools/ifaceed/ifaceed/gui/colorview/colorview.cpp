#include "colorview.h"

#include <QPainter>

gui::colorview::ColorView::ColorView(QWidget* parent) : QWidget(parent)
{

}

gui::colorview::ColorView::~ColorView()
{
    
}

void gui::colorview::ColorView::setBackgroundColor(const QColor& c)
{
     m_background_color = c;
     this->update();
}

const QColor& gui::colorview::ColorView::backgroundColor() const
{
    return m_background_color;
}

void gui::colorview::ColorView::paintEvent(QPaintEvent * e)
{
    QColor c = this->backgroundColor();

    this->QWidget::paintEvent(e);

    QPainter painter(this);
    painter.setBrush(c);
    painter.setPen(c);
    painter.fillRect(QRect(0, 0, this->width(), this->height()), c);
}

void gui::colorview::ColorView::mousePressEvent(QMouseEvent * e)
{
    emit clicked();
}
