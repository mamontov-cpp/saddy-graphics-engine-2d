#include <QMessageBox>

#include <animations/animationsanimation.h>
#include <animations/animationsinstance.h>

#include <p2d/app/way.h>

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
#include "../history/animations/animationschangerotateangle.h"
#include "../history/animations/animationschangewaymovingway.h"
#include "../history/animations/animationschangefontlistfonts.h"
#include "../history/animations/animationschangefontsizesize.h"


Q_DECLARE_METATYPE(sad::animations::Animation*)
Q_DECLARE_METATYPE(sad::p2d::app::Way*)

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

			if (a->isInstanceOf("sad::animations::Rotate"))
			{
				double minangle = m_panel->UI()->dsbRotateStartingAngle->value();
				double maxangle = m_panel->UI()->dsbRotateEndingAngle->value();
				minangle = minangle / 180.0 * M_PI;
				maxangle = maxangle / 180.0 * M_PI;

				a->setProperty("min_angle", minangle);
				a->setProperty("max_angle", maxangle);
			}

			if (a->isInstanceOf("sad::animations::WayMoving"))
			{
				unsigned long long selectedid = 0;
				int index = m_panel->UI()->cmbWayAnimationWay->currentIndex();
				if (index >= 0)
				{
					QVariant v = m_panel->UI()->cmbWayAnimationWay->itemData(index, Qt::UserRole);
					sad::p2d::app::Way* w = v.value<sad::p2d::app::Way*>();
					if (w)
					{
						selectedid = w->MajorId;
					}
				}
				a->setProperty("way", selectedid);
			}

			if (a->isInstanceOf("sad::animations::FontList"))
			{
				QStringList list = m_panel->UI()->txtFontListList->toPlainText().split("\n", QString::SkipEmptyParts);
				sad::Vector<sad::String> nlist;
				for(size_t i = 0; i < list.size(); i++)
				{
					QString tmp = list[i].trimmed();
					if (tmp.length())
					{
						nlist << tmp.toStdString();
					}
				}
				a->setProperty("fonts", nlist);
			}

			if (a->isInstanceOf("sad::animations::FontSize"))
			{
				unsigned int minsize = static_cast<unsigned int>(m_panel->UI()->sbFontSizeStartingSize->value());
				unsigned int maxsize = static_cast<unsigned int>(m_panel->UI()->sbFontSizeEndingSize->value());

				a->setProperty("min_size", minsize);
				a->setProperty("max_size", maxsize);
			}

			if (a->isInstanceOf("sad::animations::OptionList"))
			{
				QStringList list = m_panel->UI()->txtOptionListList->toPlainText().split("\n", QString::SkipEmptyParts);
				sad::Vector<sad::String> nlist;
				for(size_t i = 0; i < list.size(); i++)
				{
					QString tmp = list[i].trimmed();
					if (tmp.length())
					{
						nlist << tmp.toStdString();
					}
				}
				a->setProperty("list", nlist);
			}

			if (a->isInstanceOf("sad::animations::TextureCoordinatesList"))
			{
				QStringList list = m_panel->UI()->txtTextureCoordinatesList->toPlainText().split("\n", QString::SkipEmptyParts);
				sad::Vector<sad::String> nlist;
				for(size_t i = 0; i < list.size(); i++)
				{
					QString tmp = list[i].trimmed();
					if (tmp.length())
					{
						nlist << tmp.toStdString();
					}
				}
				a->setProperty("list", nlist);
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

		if (a->isInstanceOf("sad::animations::Rotate"))
		{
			double minangle = a->getProperty<double>("min_angle").value();
			double maxangle = a->getProperty<double>("max_angle").value();

			minangle =  minangle / M_PI * 180.0;
			maxangle =  maxangle / M_PI * 180.0;

			e->emitClosure( blocked_bind(m_panel->UI()->dsbRotateStartingAngle, &QDoubleSpinBox::setValue, minangle) );
			e->emitClosure( blocked_bind(m_panel->UI()->dsbRotateEndingAngle, &QDoubleSpinBox::setValue, maxangle) );
		}

		if (a->isInstanceOf("sad::animations::WayMoving"))
		{
		    unsigned long long way = a->getProperty<unsigned long long>("way").value();
			int pos = m_panel->findInComboBoxByMajorId<sad::p2d::app::Way*>(m_panel->UI()->cmbWayAnimationWay, way);
			if (pos >= 0)
			{
				e->emitClosure( blocked_bind(m_panel->UI()->cmbWayAnimationWay, &QComboBox::setCurrentIndex, pos) );
			}
		}

		if (a->isInstanceOf("sad::animations::FontList"))
		{
			QStringList list;
			sad::Vector<sad::String> nlist = a->getProperty<sad::Vector<sad::String> >("fonts").value();
			for(size_t i = 0; i < nlist.size(); i++)
			{
				list << nlist[i].c_str();
			}
			e->emitClosure( blocked_bind(m_panel->UI()->txtFontListList, &QTextEdit::setPlainText, list.join("\n")));
		}

		if (a->isInstanceOf("sad::animations::FontSize"))
		{
			int minsize = a->getProperty<unsigned int>("min_size").value();
			int maxsize = a->getProperty<unsigned int>("max_size").value();

			e->emitClosure( blocked_bind(m_panel->UI()->sbFontSizeStartingSize, &QSpinBox::setValue, minsize) );
			e->emitClosure( blocked_bind(m_panel->UI()->sbFontSizeEndingSize, &QSpinBox::setValue, maxsize) );
		}

		if (a->isInstanceOf("sad::animations::OptionList"))
		{
			QStringList list;
			sad::Vector<sad::String> nlist = a->getProperty<sad::Vector<sad::String> >("list").value();
			for(size_t i = 0; i < nlist.size(); i++)
			{
				list << nlist[i].c_str();
			}
			e->emitClosure( blocked_bind(m_panel->UI()->txtOptionListList, &QTextEdit::setPlainText, list.join("\n")));
		}

		if (a->isInstanceOf("sad::animations::TextureCoordinatesList"))
		{
			QStringList list;
			sad::Vector<sad::String> nlist = a->getProperty<sad::Vector<sad::String> >("list").value();
			for(size_t i = 0; i < nlist.size(); i++)
			{
				list << nlist[i].c_str();
			}
			e->emitClosure( blocked_bind(m_panel->UI()->txtTextureCoordinatesList, &QTextEdit::setPlainText, list.join("\n")));
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

void gui::AnimationActions::rotateChangeStartingAngle(double newvalue)
{
	newvalue = newvalue / 180.0 * M_PI;
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a)
	{
		if (a->isInstanceOf("sad::animations::Rotate"))
		{
			double oldvalue = a->getProperty<double>("min_angle").value();
			if (sad::is_fuzzy_equal(oldvalue, newvalue) == false)
			{				
				history::animations::ChangeRotateAngle* c = new history::animations::ChangeRotateAngle(
					a,
					"min_angle",
					m_panel->UI()->dsbRotateStartingAngle,
					oldvalue,
					newvalue
				);
				c->commit(this->m_panel->editor());

				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::rotateChangeEndingAngle(double newvalue)
{
	newvalue = newvalue / 180.0 * M_PI;
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a)
	{
		if (a->isInstanceOf("sad::animations::Rotate"))
		{
			double oldvalue = a->getProperty<double>("max_angle").value();
			if (sad::is_fuzzy_equal(oldvalue, newvalue) == false)
			{				
				history::animations::ChangeRotateAngle* c = new history::animations::ChangeRotateAngle(
					a,
					"max_angle",
					m_panel->UI()->dsbRotateEndingAngle,
					oldvalue,
					newvalue
				);
				c->commit(this->m_panel->editor());

				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}


void gui::AnimationActions::wayMovingChangeWay(int row)
{
	if (row != -1)
	{
		QVariant v = m_panel->UI()->cmbWayAnimationWay->itemData(row, Qt::UserRole);
		sad::p2d::app::Way* w = v.value<sad::p2d::app::Way*>();
		unsigned long long newvalue = 0;
		if (w)
		{
			newvalue = w->MajorId;
		}
		sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
		if (a)
		{
			if (a->isInstanceOf("sad::animations::WayMoving"))
			{
				unsigned long long oldvalue = a->getProperty<unsigned long long>("way").value();
				if (oldvalue != newvalue)
				{
					history::Command* c =new history::animations::ChangeWayMovingWay(a, oldvalue, newvalue);
					c->commit(m_panel->editor());
					this->m_panel->editor()->history()->add(c);
				}
			}
		}
	}
}

void gui::AnimationActions::fontListEditingFinished()
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::FontList"))
		{
			QStringList list = m_panel->UI()->txtFontListList->toPlainText().split("\n", QString::SkipEmptyParts);
			sad::Vector<sad::String> newvalue;
			for(size_t i = 0; i < list.size(); i++)
			{
				newvalue << list[i].toStdString();
			}

			sad::Vector<sad::String> oldvalue = a->getProperty<sad::Vector<sad::String> >("fonts").value();
			if (oldvalue != newvalue)
			{
				history::Command* c = new history::animations::ChangeFontListFonts(a, oldvalue, newvalue);
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::fontSizeChangeStartingSize(int newvalue)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::FontSize"))
		{
			unsigned int oldvalue = a->getProperty<unsigned int>("min_size").value();
			if (static_cast<unsigned int>(newvalue) != oldvalue)
			{
				history::Command* c = new history::animations::ChangeFontSizeSize(
					a, 
					"min_size",
					m_panel->UI()->sbFontSizeStartingSize,
					oldvalue,
					newvalue
				);

				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::fontSizeChangeEndingSize(int newvalue)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::FontSize"))
		{
			unsigned int oldvalue = a->getProperty<unsigned int>("max_size").value();
			if (static_cast<unsigned int>(newvalue) != oldvalue)
			{
				history::Command* c = new history::animations::ChangeFontSizeSize(
					a, 
					"max_size",
					m_panel->UI()->sbFontSizeEndingSize,
					oldvalue,
					newvalue
				);

				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::optionListEditingFinished()
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::OptionList"))
		{
			QTextEdit* widget = m_panel->UI()->txtOptionListList;
			sad::String prop = "list";

			QStringList list = widget->toPlainText().split("\n", QString::SkipEmptyParts);
			sad::Vector<sad::String> newvalue;
			for(size_t i = 0; i < list.size(); i++)
			{
				newvalue << list[i].toStdString();
			}

			sad::Vector<sad::String> oldvalue = a->getProperty<sad::Vector<sad::String> >(prop).value();
			if (oldvalue != newvalue)
			{
				history::Command* c = new history::animations::ChangeList(a, prop, widget, oldvalue, newvalue);
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}


void gui::AnimationActions::textureCoordinatesListEditingFinished()
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::TextureCoordinatesList"))
		{
			QTextEdit* widget = m_panel->UI()->txtTextureCoordinatesList;
			sad::String prop = "list";

			QStringList list = widget->toPlainText().split("\n", QString::SkipEmptyParts);
			sad::Vector<sad::String> newvalue;
			for(size_t i = 0; i < list.size(); i++)
			{
				newvalue << list[i].toStdString();
			}

			sad::Vector<sad::String> oldvalue = a->getProperty<sad::Vector<sad::String> >(prop).value();
			if (oldvalue != newvalue)
			{
				history::Command* c = new history::animations::ChangeList(a, prop, widget, oldvalue, newvalue);
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}