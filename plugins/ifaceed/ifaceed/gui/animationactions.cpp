#include <QMessageBox>

#include <animations/animationsanimation.h>
#include <animations/animationsinstance.h>

#include "../gui/animationactions.h"
#include "../gui/animationprocess.h"

#include "../blockedclosuremethodcall.h"

#include "../acolordialog.h"
#include "../mainpanel.h"

#include "../core/editor.h"

#include "../core/typeconverters/qcolortosadacolor.h"
#include "../core/typeconverters/sadacolortoqcolor.h"

#include "../history/animations/animationsnew.h"
#include "../history/animations/animationschangename.h"
#include "../history/animations/animationschangetime.h"
#include "../history/animations/animationschangelooped.h"
#include "../history/animations/animationschangeblinkingfrequency.h"
#include "../history/animations/animationschangecolorcolor.h"
#include "../history/animations/animationschangeresizevector.h"


Q_DECLARE_METATYPE(sad::animations::Animation*)

// ===============================  PUBLIC METHODS ===============================

gui::AnimationActions::AnimationActions(QObject* parent) : QObject(parent), m_panel(NULL)
{
	m_animation = new gui::AnimationProcess();
}

gui::AnimationActions::~AnimationActions()
{
	delete m_animation;	
}

void gui::AnimationActions::setPanel(MainPanel* e)
{
	m_panel = e;
	m_animation->setEditor(e->editor());
}

MainPanel* gui::AnimationActions::panel() const
{
	return m_panel;
}

// ===============================  PUBLIC SLOTS METHODS ===============================

void gui::AnimationActions::addAnimation()
{
	QComboBox * cmbtype = m_panel->UI()->cmbAddedAnimationType;
	if (cmbtype->currentIndex() != -1)
	{
		QString animationtypename = cmbtype->currentText();
		animationtypename = QString("sad::animations::") + animationtypename;

		sad::animations::Animation* a = m_panel->editor()->animationFactory()->create(animationtypename.toStdString());

		if (a)
		{
			a->setLooped(m_panel->UI()->cbAnimationLooped->checkState() == Qt::Checked);
			a->setTime(m_panel->UI()->dsbAnimationTime->value());
			a->setObjectName(m_panel->UI()->txtAnimationName->text().toStdString());
			if (a->isInstanceOf("sad::animations::Blinking"))
			{
				unsigned int frequency = static_cast<unsigned int>(m_panel->UI()->sbBlinkingFrequency->value());
				a->setProperty("frequency", frequency);
			}
			if (a->isInstanceOf("sad::animations::Color"))
			{
				QColor sourcemincolor = m_panel->UI()->cwColorStartingColor->backgroundColor();
				QColor sourcemaxcolor = m_panel->UI()->cwColorEndingColor->backgroundColor();

				sad::AColor mincolor;
				sad::AColor maxcolor;

				core::typeconverters::QColorToSadAColor::convert(sourcemincolor, mincolor);
				core::typeconverters::QColorToSadAColor::convert(sourcemaxcolor, maxcolor);

				a->setProperty("min_color", mincolor);
				a->setProperty("max_color", maxcolor);
			}

			if (a->isInstanceOf("sad::animations::Resize"))
			{
				sad::Point2D p(
					m_panel->UI()->dabResizeVectorX->value(),
					m_panel->UI()->dabResizeVectorY->value()
				);
				a->setProperty("vector", p);
			}

			sad::Renderer::ref()->database("")->table("animations")->add(a);

			history::animations::New* c = new history::animations::New(a);
			c->commit(this->m_panel->editor());

			m_panel->UI()->lstAnimations->setCurrentRow(m_panel->UI()->lstAnimations->count() - 1);
			this->m_panel->editor()->history()->add(c);			
		}
		else
		{
			QMessageBox::critical(NULL, "Coding error", animationtypename + " is not defined");
		}
	}
}

void gui::AnimationActions::currentAnimationChanged(int row)
{
	core::Editor* e = m_panel->editor();
	if (row > -1)
	{
		QVariant v = m_panel->UI()->lstAnimations->item(row)->data(Qt::UserRole);
		sad::animations::Animation* a = v.value<sad::animations::Animation*>();
		e->shared()->setSelectedAnimation(a);

		e->emitClosure( blocked_bind(m_panel->UI()->txtAnimationName, &QLineEdit::setText, a->objectName().c_str()) );
		e->emitClosure( blocked_bind(m_panel->UI()->dsbAnimationTime, &QDoubleSpinBox::setValue, a->time()) );
		Qt::CheckState cs = (a->looped()) ? Qt::Checked : Qt::Unchecked;
		e->emitClosure( blocked_bind(m_panel->UI()->cbAnimationLooped, &QCheckBox::setCheckState, cs) );

		if (a->isInstanceOf("sad::animations::Blinking"))
		{
			unsigned int frequency = a->getProperty<unsigned int>("frequency").value();
			e->emitClosure( blocked_bind(m_panel->UI()->sbBlinkingFrequency, &QSpinBox::setValue, frequency) );
		}
		if (a->isInstanceOf("sad::animations::Color"))
		{
			sad::AColor sourcemincolor = a->getProperty<sad::AColor>("min_color").value();
			sad::AColor sourcemaxcolor = a->getProperty<sad::AColor>("max_color").value();
			
			QColor mincolor;
			QColor maxcolor;

			core::typeconverters::SadAColorToQColor::convert(sourcemincolor, mincolor);
			core::typeconverters::SadAColorToQColor::convert(sourcemaxcolor, maxcolor);

			m_panel->UI()->cwColorStartingColor->setBackgroundColor(mincolor);
			m_panel->UI()->cwColorEndingColor->setBackgroundColor(maxcolor);
		}

		if (a->isInstanceOf("sad::animations::Resize"))
		{
			sad::Point2D p = a->getProperty<sad::Point2D>("vector").value();

			e->emitClosure( blocked_bind(m_panel->UI()->dabResizeVectorX, &QDoubleSpinBox::setValue, p.x()) );
			e->emitClosure( blocked_bind(m_panel->UI()->dabResizeVectorY, &QDoubleSpinBox::setValue, p.y()) );
		}

	}
	else
	{
		e->shared()->setSelectedAnimation(NULL);
	}
}

void gui::AnimationActions::nameChanged(const QString& name)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		const sad::String oldname = a->objectName();
		if (oldname != name.toStdString())
		{
			history::animations::ChangeName* c = new history::animations::ChangeName(a, oldname, name.toStdString());
			c->commit(this->m_panel->editor());

			this->m_panel->editor()->history()->add(c);
		}
	}
}


void gui::AnimationActions::timeChanged(double time)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		double oldtime = a->time();
		if (sad::is_fuzzy_equal(oldtime, time) == false)
		{
			history::animations::ChangeTime* c = new history::animations::ChangeTime(a, oldtime, time);
			c->commit(this->m_panel->editor());

			this->m_panel->editor()->history()->add(c);
		}
	}
}


void gui::AnimationActions::loopedChanged(bool newvalue)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		bool oldvalue = a->looped();
		if (oldvalue != newvalue)
		{
			history::animations::ChangeLooped* c = new history::animations::ChangeLooped(a, oldvalue, newvalue);
			c->commit(this->m_panel->editor());

			this->m_panel->editor()->history()->add(c);
		}
	}
}


void gui::AnimationActions::blinkingFrequencyChanged(int nvalue)
{
	unsigned int newvalue = static_cast<unsigned int>(nvalue);
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::Blinking")) 
		{
			unsigned int oldvalue = a->getProperty<unsigned int>("frequency").value();
			if (oldvalue != newvalue)
			{
				history::animations::ChangeBlinkingFrequency* c = new history::animations::ChangeBlinkingFrequency(a, oldvalue, newvalue);
				c->commit(this->m_panel->editor());

				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}


void gui::AnimationActions::startOnObject()
{
	core::Shared* s = m_panel->editor()->shared();
	if (s->isAnyKindOfAnimationIsRunning() == false 
		&& s->selectedObject() != NULL
		&& s->selectedAnimation() != NULL)
	{
		sad::animations::Instance* i = new sad::animations::Instance();
		i->setAnimation(s->selectedAnimation());
		i->setObject(s->selectedObject());
		i->setStartTime(0.0);
		m_animation->setEditor(m_panel->editor());
		m_animation->start(i);
	}
}

void gui::AnimationActions::stopOnObject()
{
	core::Shared* s = m_panel->editor()->shared();
	if (s->isAnyKindOfAnimationIsRunning() == true)
	{
		m_animation->setEditor(m_panel->editor());
		m_animation->stop();
	}
}


void gui::AnimationActions::colorChangeStartingColor()
{
	gui::colorview::ColorView* view = m_panel->UI()->cwColorStartingColor; 
	QColor oldvalue = view->backgroundColor();
	AColorDialog dlg;
	dlg.setColorPalette(m_panel->colorPalette());
	dlg.setSelectedColor(oldvalue);
	
	if (dlg.exec() == QDialog::Accepted)
	{
		QColor i = dlg.selectedColor();
		m_panel->setColorPalette(dlg.colorPalette());
		view->setBackgroundColor(i);
		sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
		if (oldvalue != i && a != NULL)
		{
			if (a->isInstanceOf("sad::animations::Color"))
			{
				sad::AColor oldcolor;
				sad::AColor newcolor;

				core::typeconverters::QColorToSadAColor::convert(oldvalue,oldcolor);
				core::typeconverters::QColorToSadAColor::convert(i,newcolor);

				history::animations::ChangeColorColor* c = new history::animations::ChangeColorColor(
					a,
					"min_color",
					view,
					oldcolor,
					newcolor
				);
				c->commit(this->m_panel->editor());

				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::colorChangeEndingColor()
{
	gui::colorview::ColorView* view = m_panel->UI()->cwColorEndingColor; 
	QColor oldvalue = view->backgroundColor();
	AColorDialog dlg;
	dlg.setColorPalette(m_panel->colorPalette());
	dlg.setSelectedColor(oldvalue);
	
	if (dlg.exec() == QDialog::Accepted)
	{
		QColor i = dlg.selectedColor();
		m_panel->setColorPalette(dlg.colorPalette());
		view->setBackgroundColor(i);
		sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
		if (oldvalue != i && a != NULL)
		{
			if (a->isInstanceOf("sad::animations::Color"))
			{
				sad::AColor oldcolor;
				sad::AColor newcolor;

				core::typeconverters::QColorToSadAColor::convert(oldvalue,oldcolor);
				core::typeconverters::QColorToSadAColor::convert(i,newcolor);

				history::animations::ChangeColorColor* c = new history::animations::ChangeColorColor(
					a,
					"max_color",
					view,
					oldcolor,
					newcolor
				);
				c->commit(this->m_panel->editor());

				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}


void gui::AnimationActions::resizeChangeVectorX(double newvalue)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a)
	{
		if (a->isInstanceOf("sad::animations::Resize"))
		{
			sad::Point2D oldvalue = a->getProperty<sad::Point2D>("vector").value();
			if (sad::is_fuzzy_equal(oldvalue.x(), newvalue) == false)
			{
				sad::Point2D nvalue = oldvalue;
				nvalue.setX(newvalue);

				history::animations::ChangeResizeVector* c = new history::animations::ChangeResizeVector(
					a,
					oldvalue,
					nvalue
				);
				c->commit(this->m_panel->editor());

				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::resizeChangeVectorY(double newvalue)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a)
	{
		if (a->isInstanceOf("sad::animations::Resize"))
		{
			sad::Point2D oldvalue = a->getProperty<sad::Point2D>("vector").value();
			if (sad::is_fuzzy_equal(oldvalue.y(), newvalue) == false)
			{
				sad::Point2D nvalue = oldvalue;
				nvalue.setY(newvalue);

				history::animations::ChangeResizeVector* c = new history::animations::ChangeResizeVector(
					a,
					oldvalue,
					nvalue
				);
				c->commit(this->m_panel->editor());

				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}
