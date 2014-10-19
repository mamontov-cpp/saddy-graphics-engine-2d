#include "gui/anglewidget/anglewidget.h"
#include <qmath.h>

gui::anglewidget::AngleWidget::AngleWidget(QWidget * parent) : QWidget(parent)
{
	m_layout = new QHBoxLayout(this);
	
	m_label = new QLabel(this);
	m_label->setText("Angle: ");
	m_label->setMinimumSize(m_label->sizeHint());	

	m_angle = new QSpinBox(this);
	m_angle->setMinimum(0);
	m_angle->setMaximum(360);
	m_angle->setValue(0);
	m_angle->setMinimumSize(m_angle->sizeHint());	

	
	m_layout->addWidget(m_label,1);
	m_layout->addWidget(m_angle,1);

	this->setLayout(m_layout);

	connect(m_angle, SIGNAL(valueChanged(int)), this, SLOT(angleChanged(int)));
}

void gui::anglewidget::AngleWidget::setValue(double value)
{
	int angle = (int)gui::anglewidget::AngleWidget::rad2deg(value);
	bool blocked = m_angle->blockSignals(true);
	m_angle->setValue(angle);
	m_angle->blockSignals(false);
}

double gui::anglewidget::AngleWidget::value() const
{
	return gui::anglewidget::AngleWidget::deg2rad((double)(m_angle->value()));
}

double gui::anglewidget::AngleWidget::incrementInDegrees(double angle, int increment)
{
	int degrees = (int)(gui::anglewidget::AngleWidget::rad2deg(angle));
	degrees += increment;
	if (degrees < 0)
	{
		degrees += 360;
	}
	if (degrees > 360)
	{
		degrees -= 360;
	}
	double radians = gui::anglewidget::AngleWidget::deg2rad((double) degrees);
	setValue(radians);
	return radians;
}


void gui::anglewidget::AngleWidget::angleChanged(int a)
{
	emit valueChanged(gui::anglewidget::AngleWidget::deg2rad((double)a));
}

double gui::anglewidget::AngleWidget::deg2rad(double v)
{
	return v / 180.0 * M_PI;
}

double gui::anglewidget::AngleWidget::rad2deg(double v)
{
	return v / M_PI * 180.0;
}
