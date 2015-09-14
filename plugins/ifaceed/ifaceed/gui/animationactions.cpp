#include <QMessageBox>

#include <animations/animationsanimation.h>
#include <animations/animationsinstance.h>

#include <p2d/app/way.h>

#include "../gui/animationactions.h"
#include "../gui/animationprocess.h"

#include "../blockedclosuremethodcall.h"

#include "../acolordialog.h"
#include "../mainpanel.h"
#include "../qstdstring.h"

#include "../core/editor.h"

#include "../core/typeconverters/qcolortosadacolor.h"
#include "../core/typeconverters/sadacolortoqcolor.h"
#include "../core/typeconverters/qrectftosadrect2d.h"
#include "../core/typeconverters/sadrect2dtoqrectf.h"


#include "../history/animations/animationsnew.h"
#include "../history/animations/animationsremove.h"
#include "../history/animations/animationschangename.h"
#include "../history/animations/animationschangetime.h"
#include "../history/animations/animationschangelooped.h"
#include "../history/animations/animationschangeblinkingfrequency.h"
#include "../history/animations/animationschangecolorcolor.h"
#include "../history/animations/animationschangeresizestartingsize.h"
#include "../history/animations/animationschangeresizeendingsize.h"
#include "../history/animations/animationschangerotateangle.h"
#include "../history/animations/animationschangewaymovingway.h"
#include "../history/animations/animationschangefontlistfonts.h"
#include "../history/animations/animationschangefontsizesize.h"
#include "../history/animations/animationschangerect.h"
#include "../history/animations/animationschangecamerapivot.h"
#include "../history/animations/animationschangecameraangle.h"
#include "../history/animations/animationschangecameraoffset.h"
#include "../history/animations/animationschangeshakingfrequency.h"
#include "../history/animations/animationsaddtocomposite.h"
#include "../history/animations/animationsremovefromcomposite.h"
#include "../history/animations/animationsswapincomposite.h"
#include "../history/animations/animationschangepropertyaspoint2displayedintwospinboxes.h"

Q_DECLARE_METATYPE(sad::animations::Animation*) //-V566
Q_DECLARE_METATYPE(sad::p2d::app::Way*) //-V566

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


bool gui::AnimationActions::producesLoop(sad::animations::Animation* first, sad::animations::Animation* second)
{
	bool result = false;
	sad::Vector<sad::db::Object*> objects;
	sad::Renderer::ref()->database("")->table("animations")->objects(objects);
	sad::Vector<sad::animations::Composite*> animations;
	for(size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->isInstanceOf("sad::animations::Sequential") || 
			objects[i]->isInstanceOf("sad::animations::Parallel"))
		{
			animations << static_cast<sad::animations::Composite*>(objects[i]);
		}
	}
	for(size_t i = 0; (i < animations.size()) && result == false; i++)
	{
		sad::Hash<unsigned long long, unsigned long long> tmp;
		result = result || producesLoop(animations[i], tmp, first, second);
	}
	return result;
}

bool gui::AnimationActions::producesLoop(
	sad::animations::Composite* current,
	const sad::Hash<unsigned long long, unsigned long long>& visited,
	sad::animations::Animation* first,
	sad::animations::Animation* second
)
{
	// If already visited this node, then we must have returned here by loop
	if (visited.contains(current->MajorId))
	{
		return true;
	}
	sad::Hash<unsigned long long, unsigned long long> local = visited;	
	local.insert(current->MajorId, 1);
	if (current == first)
	{
		// If second node has been visited already, than we must been here
		if (local.contains(second->MajorId))
		{
			return true;
		}
		if (second->isInstanceOf("sad::animations::Parallel") || second->isInstanceOf("sad::animations::Sequential"))
		{
			sad::animations::Composite* c = static_cast<sad::animations::Composite*>(second);
			// Check if loop can be found just by jumping from second animation
			if (producesLoop(c, local, first, second))
			{
				return true;
			}
		}
	}

	bool result = false;
	for(size_t i = 0; i < current->size() && result == false; i++)
	{
		sad::animations::Animation* a  = current->animation(i);
		if (a)
		{
			if (a->isInstanceOf("sad::animations::Parallel") || a->isInstanceOf("sad::animations::Sequential"))
			{
				sad::animations::Composite* c = static_cast<sad::animations::Composite*>(a);
				result = result || producesLoop(c, local, first, second);
			}
		}
	}
	return result;
}


void gui::AnimationActions::removeAnimationFromDatabase(
	sad::animations::Animation* a,
	bool fromeditor
)
{
	int posinmainlist = m_panel->findInList<sad::animations::Animation*>(m_panel->UI()->lstAnimations, a);
	int posininstances = m_panel->findInComboBox<sad::animations::Animation*>(m_panel->UI()->cmbAnimationInstanceAnimationFromDatabase, a);

	sad::Vector< sad::Pair<sad::animations::Composite*, sad::Vector<int> > > list;

	sad::Vector<sad::db::Object*> animations;
	sad::Renderer::ref()->database("")->table("animations")->objects(animations);
	for(size_t i = 0; i < animations.size(); i++)
	{
		sad::db::Object* object = animations[i];
		if (object->isInstanceOf("sad::animations::Parallel") || object->isInstanceOf("sad::animations::Sequential"))
		{
			sad::animations::Composite* c = static_cast<sad::animations::Composite*>(object);
			sad::Vector<unsigned long long> majorids = c->animationMajorIds();
			sad::Vector<int> positions;
			for(size_t j = 0; j < majorids.size(); j++)
			{
				if (majorids[j] == a->MajorId)
				{
					positions << j;
				}
			}
			if (positions.size() != 0)
			{
				list << sad::Pair<sad::animations::Composite*, sad::Vector<int> >(c, positions);				
			}
		}
	}

	sad::Vector<sad::db::Object*> animationinstances;
	sad::Vector<sad::animations::Instance*> dependentinstances;
	sad::Renderer::ref()->database("")->table("animationinstances")->objects(animationinstances);
	for(size_t  i = 0; i < animationinstances.size(); i++)
	{
		sad::db::Object* object = animationinstances[i];
		if (object->isInstanceOf("sad::animations::Instance") || object->isInstanceOf("sad::animations::WayInstance"))
		{
			sad::animations::Instance* ainstance = static_cast<sad::animations::Instance*>(object);
			if (ainstance->animationMajorId() == a->MajorId)
			{
				dependentinstances << ainstance;
			}
		}
	}

	history::animations::Remove* command = new history::animations::Remove(a);
	command->set(posinmainlist, posininstances, list);
	command->set(dependentinstances);
	command->commit(this->m_panel->editor());

	if (fromeditor)
	{
		this->m_panel->editor()->history()->add(command);
	}
	else
	{
		this->m_panel->editor()->currentBatchCommand()->add(command);
	}
}

bool gui::AnimationActions::addAnimationToCompositeList(
	sad::animations::Composite* a,
	sad::animations::Animation* addedanimation,
	bool fromeditor
)
{
	bool result = false;
	if (producesLoop(a, addedanimation) == false)
	{
		sad::animations::Composite* co = static_cast<sad::animations::Composite*>(a);
		history::Command* c = new history::animations::AddToComposite(co, addedanimation->MajorId);
		c->commit(m_panel->editor());
		if (fromeditor)
		{
			this->m_panel->editor()->history()->add(c);
		}
		else
		{
			this->m_panel->editor()->currentBatchCommand()->add(c);
		}
		result = true;
	}
	return result;
}


bool gui::AnimationActions::removeAnimationFromCompositeList(
		sad::animations::Composite* a,
		sad::animations::Animation* animation,
		bool fromeditor,
		int row
)
{
	unsigned long long majorid = animation->MajorId;
	if (row == -1)
	{
		for(size_t i = 0; i < a->animationMajorIds().size() && row == -1; i++)
		{
			if (a->animationMajorIds()[i] == majorid)
			{
				row =  i;
			}
		}
	}
	bool result = false;
	if (row != -1)
	{
		result = true;
		history::Command* c = new history::animations::RemoveFromComposite(a, majorid, row);
		c->commit(m_panel->editor());
		if (fromeditor)
		{
			this->m_panel->editor()->history()->add(c);
		}
		else
		{
			this->m_panel->editor()->currentBatchCommand()->add(c);
		}
	}
	return result;
}

// ===============================  PUBLIC SLOTS METHODS ===============================

void gui::AnimationActions::addAnimation()
{
	QComboBox * cmbtype = m_panel->UI()->cmbAddedAnimationType;
	if (cmbtype->currentIndex() != -1)
	{
		QString animationtypename = cmbtype->currentText();
		animationtypename = QString("sad::animations::") + animationtypename;

		sad::animations::Animation* a = m_panel->editor()->animationFactory()->create(Q2STDSTRING(animationtypename));

		if (a)
		{
			a->setLooped(m_panel->UI()->cbAnimationLooped->checkState() == Qt::Checked);
			a->setTime(m_panel->UI()->dsbAnimationTime->value());
			a->setObjectName(Q2STDSTRING(m_panel->UI()->txtAnimationName->text()));
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
				sad::Point2D startsize(
					m_panel->UI()->dabResizeStartingSizeX->value(),
					m_panel->UI()->dabResizeStartingSizeY->value()
				);
				a->setProperty("start_size", startsize);

                sad::Point2D endsize(
					m_panel->UI()->dabResizeEndingSizeX->value(),
					m_panel->UI()->dabResizeEndingSizeY->value()
				);
				a->setProperty("end_size", endsize);
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
						nlist << Q2STDSTRING(tmp);
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

            if (a->isInstanceOf("sad::animations::SimpleMovement"))
            {
                double sx = m_panel->UI()->dabSimpleMovementStartingPointX->value();
                double sy = m_panel->UI()->dabSimpleMovementStartingPointY->value();

                double ex = m_panel->UI()->dabSimpleMovementEndingPointX->value();
                double ey = m_panel->UI()->dabSimpleMovementEndingPointY->value();

                a->setProperty("start_point", sad::Point2D(sx, sy));
                a->setProperty("end_point", sad::Point2D(ex, ey));
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
						nlist << Q2STDSTRING(tmp);
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
						nlist << Q2STDSTRING(tmp);
					}
				}
				a->setProperty("list", nlist);
			}

			if (a->isInstanceOf("sad::animations::TextureCoordinatesContinuous"))
			{
				QRectF start = m_panel->UI()->rctTCCStartingRect->value();
				QRectF end = m_panel->UI()->rctTCCEndingRect->value();

				sad::Rect2D kstart;
				sad::Rect2D kend;

				core::typeconverters::QRectFToSadRect2D::convert(start, kstart);
				core::typeconverters::QRectFToSadRect2D::convert(end, kend);

				a->setProperty("start_rect", kstart);
				a->setProperty("end_rect", kend);
			}

			if (a->isInstanceOf("sad::animations::CameraRotation"))
			{
				sad::Point3D pivot(
					m_panel->UI()->dsbCameraRotationPivotX->value(),
					m_panel->UI()->dsbCameraRotationPivotY->value(),
					0.0
				);

				double startangle = m_panel->UI()->dsbCameraRotationStartingAngle->value();
				double endangle = m_panel->UI()->dsbCameraRotationEndingAngle->value();
				a->setProperty("min_angle", startangle);
				a->setProperty("max_angle", endangle);
				a->setProperty("pivot", pivot);
			}

			if (a->isInstanceOf("sad::animations::CameraShaking"))
			{
				sad::Point2D offset(
					m_panel->UI()->dsbCameraShakingOffsetX->value(),
					m_panel->UI()->dsbCameraShakingOffsetY->value()
				);

				int freq = m_panel->UI()->sbCameraShakingFrequency->value();
				a->setProperty("frequency", freq);
				a->setProperty("offset", offset);
			}

			if (a->isInstanceOf("sad::animations::Parallel") || a->isInstanceOf("sad::animations::Sequential"))
			{
				sad::Vector<unsigned long long> majorids;
				QListWidget* w = m_panel->UI()->lstCompositeList;
				for(size_t i = 0; i < w->count(); i++)
				{
					sad::animations::Animation* ka = w->item(i)->data(Qt::UserRole).value<sad::animations::Animation*>();
					majorids << ka->MajorId;
				}
				a->setProperty("list", majorids);
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

void gui::AnimationActions::removeAnimation()
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		removeAnimationFromDatabase(a, true);
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

		e->emitClosure( blocked_bind(m_panel->UI()->txtAnimationName, &QLineEdit::setText, STD2QSTRING(a->objectName())) );
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
			sad::Point2D startsize = a->getProperty<sad::Point2D>("start_size").value();
			sad::Point2D end_size = a->getProperty<sad::Point2D>("end_size").value();

			e->emitClosure( blocked_bind(m_panel->UI()->dabResizeStartingSizeX, &QDoubleSpinBox::setValue, startsize.x()) );
			e->emitClosure( blocked_bind(m_panel->UI()->dabResizeStartingSizeY, &QDoubleSpinBox::setValue, startsize.y()) );

            e->emitClosure( blocked_bind(m_panel->UI()->dabResizeEndingSizeX, &QDoubleSpinBox::setValue, end_size.x()) );
			e->emitClosure( blocked_bind(m_panel->UI()->dabResizeEndingSizeY, &QDoubleSpinBox::setValue, end_size.y()) );
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

        if (a->isInstanceOf("sad::animations::SimpleMovement"))
		{
		    sad::Point2D startpos = a->getProperty<sad::Point2D>("start_point").value();
			sad::Point2D endpos = a->getProperty<sad::Point2D>("end_point").value();

            e->emitClosure( blocked_bind(m_panel->UI()->dabSimpleMovementStartingPointX, &QDoubleSpinBox::setValue, startpos.x()) );
			e->emitClosure( blocked_bind(m_panel->UI()->dabSimpleMovementStartingPointY, &QDoubleSpinBox::setValue, startpos.y()) );

            e->emitClosure( blocked_bind(m_panel->UI()->dabSimpleMovementEndingPointX, &QDoubleSpinBox::setValue, endpos.x()) );
			e->emitClosure( blocked_bind(m_panel->UI()->dabSimpleMovementEndingPointY, &QDoubleSpinBox::setValue, endpos.y()) );
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
				list << STD2QSTRING(nlist[i]);
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
				list << STD2QSTRING(nlist[i]);
			}
			e->emitClosure( blocked_bind(m_panel->UI()->txtOptionListList, &QTextEdit::setPlainText, list.join("\n")));
		}

		if (a->isInstanceOf("sad::animations::TextureCoordinatesList"))
		{
			QStringList list;
			sad::Vector<sad::String> nlist = a->getProperty<sad::Vector<sad::String> >("list").value();
			for(size_t i = 0; i < nlist.size(); i++)
			{
				list << STD2QSTRING(nlist[i]);
			}
			e->emitClosure( blocked_bind(m_panel->UI()->txtTextureCoordinatesList, &QTextEdit::setPlainText, list.join("\n")));
		}

		if (a->isInstanceOf("sad::animations::TextureCoordinatesContinuous"))
		{
			sad::Rect2D nstartrect = a->getProperty<sad::Rect2D>("start_rect").value();
			sad::Rect2D nendrect = a->getProperty<sad::Rect2D>("end_rect").value();

			QRectF startrect;
			QRectF endrect;

			core::typeconverters::SadRect2DToQRectF::convert(nstartrect, startrect);
			core::typeconverters::SadRect2DToQRectF::convert(nendrect, endrect);

			e->emitClosure( blocked_bind(m_panel->UI()->rctTCCStartingRect, &gui::rectwidget::RectWidget::setValue, startrect) );
			e->emitClosure( blocked_bind(m_panel->UI()->rctTCCEndingRect, &gui::rectwidget::RectWidget::setValue, endrect) );
		}

		if (a->isInstanceOf("sad::animations::CameraRotation"))
		{
			sad::Point3D pivot = a->getProperty<sad::Point3D>("pivot").value();

			e->emitClosure( blocked_bind(m_panel->UI()->dsbCameraRotationPivotX, &QDoubleSpinBox::setValue, pivot.x()) );
			e->emitClosure( blocked_bind(m_panel->UI()->dsbCameraRotationPivotY, &QDoubleSpinBox::setValue, pivot.y()) );

			double startangle = a->getProperty<double>("min_angle").value();
			double endangle = a->getProperty<double>("max_angle").value();

			e->emitClosure( blocked_bind(m_panel->UI()->dsbCameraRotationStartingAngle, &QDoubleSpinBox::setValue, startangle) );
			e->emitClosure( blocked_bind(m_panel->UI()->dsbCameraRotationEndingAngle, &QDoubleSpinBox::setValue, endangle) );
		}

		if (a->isInstanceOf("sad::animations::CameraShaking"))
		{
			sad::Point2D offset = a->getProperty<sad::Point2D>("offset").value();

			e->emitClosure( blocked_bind(m_panel->UI()->dsbCameraShakingOffsetX, &QDoubleSpinBox::setValue, offset.x()) );
			e->emitClosure( blocked_bind(m_panel->UI()->dsbCameraShakingOffsetY, &QDoubleSpinBox::setValue, offset.y()) );

			int frequency = a->getProperty<int>("frequency").value();
			e->emitClosure( blocked_bind(m_panel->UI()->sbCameraShakingFrequency, &QSpinBox::setValue, frequency) );
		}
		
		this->updateCompositeList();		
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
		if (oldname != Q2STDSTRING(name))
		{
			history::animations::ChangeName* c = new history::animations::ChangeName(a, oldname, Q2STDSTRING(name));
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

		sad::animations::Animation* a = s->selectedAnimation();
		sad::SceneNode* node = s->selectedObject();
		sad::db::Object* obj = node;
		if (a->isInstanceOf("sad::animations::CameraShaking") || a->isInstanceOf("sad::animations::CameraRotation"))
		{
			obj = node->scene();
		}
		i->setAnimation(a);
		i->setObject(obj);
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
    QList<QList<QColor> > palette = m_panel->colorPalette();
 	dlg.setColorPalette(palette);
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


void gui::AnimationActions::resizeChangeStartingSizeX(double newvalue)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a)
	{
		if (a->isInstanceOf("sad::animations::Resize"))
		{
			sad::Point2D oldvalue = a->getProperty<sad::Point2D>("start_size").value();
			if (sad::is_fuzzy_equal(oldvalue.x(), newvalue) == false)
			{
				sad::Point2D nvalue = oldvalue;
				nvalue.setX(newvalue);

				history::animations::ChangeResizeStartingSize* c = new history::animations::ChangeResizeStartingSize(
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

void gui::AnimationActions::resizeChangeStartingSizeY(double newvalue)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a)
	{
		if (a->isInstanceOf("sad::animations::Resize"))
		{
			sad::Point2D oldvalue = a->getProperty<sad::Point2D>("start_size").value();
			if (sad::is_fuzzy_equal(oldvalue.y(), newvalue) == false)
			{
				sad::Point2D nvalue = oldvalue;
				nvalue.setY(newvalue);

				history::animations::ChangeResizeStartingSize* c = new history::animations::ChangeResizeStartingSize(
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

void gui::AnimationActions::resizeChangeEndingSizeX(double newvalue)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a)
	{
		if (a->isInstanceOf("sad::animations::Resize"))
		{
			sad::Point2D oldvalue = a->getProperty<sad::Point2D>("end_size").value();
			if (sad::is_fuzzy_equal(oldvalue.x(), newvalue) == false)
			{
				sad::Point2D nvalue = oldvalue;
				nvalue.setX(newvalue);

				history::animations::ChangeResizeEndingSize* c = new history::animations::ChangeResizeEndingSize(
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

void gui::AnimationActions::resizeChangeEndingSizeY(double newvalue)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a)
	{
		if (a->isInstanceOf("sad::animations::Resize"))
		{
			sad::Point2D oldvalue = a->getProperty<sad::Point2D>("end_size").value();
			if (sad::is_fuzzy_equal(oldvalue.y(), newvalue) == false)
			{
				sad::Point2D nvalue = oldvalue;
				nvalue.setY(newvalue);

				history::animations::ChangeResizeEndingSize* c = new history::animations::ChangeResizeEndingSize(
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
				newvalue << Q2STDSTRING(list[i]);
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
				newvalue << Q2STDSTRING(list[i]);
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
				newvalue << Q2STDSTRING(list[i]);
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

void gui::AnimationActions::textureCoordinatesChangeStartRect(QRectF value)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::TextureCoordinatesContinuous"))
		{
			gui::rectwidget::RectWidget* widget = m_panel->UI()->rctTCCStartingRect;
			sad::String prop = "start_rect";

			sad::Rect2D newvalue;
			core::typeconverters::QRectFToSadRect2D::convert(value, newvalue);

			sad::Rect2D oldvalue = a->getProperty< sad::Rect2D >(prop).value();
			if (sad::equal(oldvalue, newvalue) == false)
			{
				history::Command* c = new history::animations::ChangeRect(a, prop, widget, oldvalue, newvalue);
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::textureCoordinatesChangeEndRect(QRectF value)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::TextureCoordinatesContinuous"))
		{
			gui::rectwidget::RectWidget* widget = m_panel->UI()->rctTCCEndingRect;
			sad::String prop = "end_rect";

			sad::Rect2D newvalue;
			core::typeconverters::QRectFToSadRect2D::convert(value, newvalue);

			sad::Rect2D oldvalue = a->getProperty< sad::Rect2D >(prop).value();
			if (sad::equal(oldvalue, newvalue) == false)
			{
				history::Command* c = new history::animations::ChangeRect(a, prop, widget, oldvalue, newvalue);
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::cameraRotationChangePivotX(double newx)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::CameraRotation"))
		{
			sad::Point3D newvalue(newx, m_panel->UI()->dsbCameraRotationPivotY->value(), 0.0);

			sad::Point3D oldvalue = a->getProperty< sad::Point3D >("pivot").value();
			if (sad::equal(oldvalue, newvalue) == false)
			{
				history::Command* c = new history::animations::ChangeCameraPivot(a, oldvalue, newvalue);
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::cameraRotationChangePivotY(double newy)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::CameraRotation"))
		{
			sad::Point3D newvalue(m_panel->UI()->dsbCameraRotationPivotX->value(), newy, 0.0);

			sad::Point3D oldvalue = a->getProperty< sad::Point3D >("pivot").value();
			if (sad::equal(oldvalue, newvalue) == false)
			{
				history::Command* c = new history::animations::ChangeCameraPivot(a, oldvalue, newvalue);
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::cameraRotationChangeStartingAngle(double newvalue)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::CameraRotation"))
		{
			sad::String prop = "min_angle";
			QDoubleSpinBox* widget = m_panel->UI()->dsbCameraRotationStartingAngle;
			double oldvalue = a->getProperty< double >(prop).value();
			if (sad::is_fuzzy_equal(oldvalue, newvalue) == false)
			{
				history::Command* c = new history::animations::ChangeCameraAngle(a, prop, widget, oldvalue, newvalue);
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::cameraRotationChangeEndingAngle(double newvalue)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::CameraRotation"))
		{
			sad::String prop = "max_angle";
			QDoubleSpinBox* widget = m_panel->UI()->dsbCameraRotationEndingAngle;
			double oldvalue = a->getProperty< double >(prop).value();
			if (sad::is_fuzzy_equal(oldvalue, newvalue) == false)
			{
				history::Command* c = new history::animations::ChangeCameraAngle(a, prop, widget, oldvalue, newvalue);
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::cameraShakingChangeOffsetX(double newx)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::CameraShaking"))
		{
			sad::Point2D newvalue(newx, m_panel->UI()->dsbCameraShakingOffsetY->value());

			sad::Point2D oldvalue = a->getProperty< sad::Point2D >("offset").value();
			if (sad::equal(oldvalue, newvalue) == false)
			{
				history::Command* c = new history::animations::ChangeCameraOffset(a, oldvalue, newvalue);
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::cameraShakingChangeOffsetY(double newy)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::CameraShaking"))
		{
			sad::Point2D newvalue(m_panel->UI()->dsbCameraShakingOffsetX->value(), newy);

			sad::Point2D oldvalue = a->getProperty< sad::Point2D >("offset").value();
			if (sad::equal(oldvalue, newvalue) == false)
			{
				history::Command* c = new history::animations::ChangeCameraOffset(a, oldvalue, newvalue);
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::cameraShakingChangeFrequency(int newvalue)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::CameraShaking"))
		{
			int oldvalue = a->getProperty< int >("frequency").value();
			if (oldvalue != newvalue)
			{
				history::Command* c = new history::animations::ChangeShakingFrequency(a, oldvalue, newvalue);
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::updateCompositeList()
{
	sad::Vector<unsigned long long> majorids;
	sad::animations::Animation* sa = m_panel->editor()->shared()->selectedAnimation();
	if (sa)
	{
		if (sa->isInstanceOf("sad::animations::Parallel") || sa->isInstanceOf("sad::animations::Sequential"))
		{
			sad::animations::Composite* c = static_cast<sad::animations::Composite*>(sa);
			majorids = c->animationMajorIds();
		}
	}
	QListWidget* candidatelist = m_panel->UI()->lstCompositeCandidates;
	int candidaterow = candidatelist->currentRow();
	candidatelist->clear();
	QListWidget* sourcelist = m_panel->UI()->lstAnimations;
	for(size_t i = 0; i < sourcelist->count(); i++)
	{
		QString text = sourcelist->item(i)->text();
		QVariant v = sourcelist->item(i)->data(Qt::UserRole);
		sad::animations::Animation* a = v.value<sad::animations::Animation*>();
		candidatelist->addItem(text);
		candidatelist->item(candidatelist->count() - 1)->setData(Qt::UserRole,  v);
	}

	QListWidget* ownlist = m_panel->UI()->lstCompositeList;
	int ownrow = ownlist->currentRow();
	ownlist->clear();
	for(size_t i = 0; i < majorids.count(); i++)
	{
		sad::animations::Animation* a = static_cast<sad::animations::Animation*>(sad::Renderer::ref()->database("")->queryByMajorId(majorids[i]));
		QString name = m_panel->nameForAnimation(a);
		QVariant v;
		v.setValue(a);
		ownlist->addItem(name);
		ownlist->item(ownlist->count() - 1)->setData(Qt::UserRole, v);
	}

	void (QListWidget::*f)(int) = &QListWidget::setCurrentRow;

	if (candidaterow >= 0 && candidatelist->count() > 0)
	{
		if (candidaterow >= candidatelist->count())
		{
			candidaterow = candidatelist->count() - 1;
		}
		invoke_blocked(candidatelist, f, candidaterow);
	}

	if (ownrow >= 0 && ownlist->count() > 0)
	{
		if (ownrow >= ownlist->count())
		{
			ownrow = ownlist->count() - 1;
		}
		invoke_blocked(ownlist, f, ownrow);
	}
}

void gui::AnimationActions::addAnimationToComposite()
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::Parallel") || a->isInstanceOf("sad::animations::Sequential"))
		{
			QListWidget* candidatelist = m_panel->UI()->lstCompositeCandidates;
			int pos = candidatelist->currentRow();
			if (pos > -1 && pos < candidatelist->count())
			{
				sad::animations::Animation* addedanimation = candidatelist->item(pos)->data(Qt::UserRole).value<sad::animations::Animation*>();
				sad::animations::Composite* co = static_cast<sad::animations::Composite*>(a);
				addAnimationToCompositeList(co, addedanimation, true);
			}
		}
	}
}

void gui::AnimationActions::removeAnimationFromComposite()
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::Parallel") || a->isInstanceOf("sad::animations::Sequential"))
		{
			QListWidget* list = m_panel->UI()->lstCompositeList;
			int pos = list->currentRow();
			if (pos > -1 && pos < list->count())
			{				
				sad::animations::Animation* animation = list->item(pos)->data(Qt::UserRole).value<sad::animations::Animation*>();
				sad::animations::Composite* co = static_cast<sad::animations::Composite*>(a);
				removeAnimationFromCompositeList(co, animation, true, pos);
			}
		}
	}
}

void gui::AnimationActions::moveBackInCompositeList()
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::Parallel") || a->isInstanceOf("sad::animations::Sequential"))
		{
			QListWidget* list = m_panel->UI()->lstCompositeList;
			int pos = list->currentRow();
			if (pos > -1 && pos < list->count() - 1)
			{
				sad::animations::Composite* co = static_cast<sad::animations::Composite*>(a);
				history::Command* c = new history::animations::SwapInComposite(co, pos, pos + 1);
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::moveFrontInCompositeList()
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::Parallel") || a->isInstanceOf("sad::animations::Sequential"))
		{
			QListWidget* list = m_panel->UI()->lstCompositeList;
			int pos = list->currentRow();
			if (pos > 0 && pos < list->count())
			{
				sad::animations::Composite* co = static_cast<sad::animations::Composite*>(a);
				history::Command* c = new history::animations::SwapInComposite(co, pos - 1, pos);
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::simpleMovementChangeStartingPointX(double newx)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::SimpleMovement"))
		{
			sad::Point2D newvalue(newx, m_panel->UI()->dabSimpleMovementStartingPointY->value());

			sad::Point2D oldvalue = a->getProperty< sad::Point2D >("start_point").value();
			if (sad::equal(oldvalue, newvalue) == false)
			{
				history::Command* c = new history::animations::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes(
                    a, 
                    "start_point",
                    oldvalue, 
                    newvalue,
                    m_panel->UI()->dabSimpleMovementStartingPointX,
                    m_panel->UI()->dabSimpleMovementStartingPointY
                );
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::simpleMovementChangeStartingPointY(double newy)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::SimpleMovement"))
		{
			sad::Point2D newvalue(m_panel->UI()->dabSimpleMovementStartingPointX->value(), newy);

			sad::Point2D oldvalue = a->getProperty< sad::Point2D >("start_point").value();
			if (sad::equal(oldvalue, newvalue) == false)
			{
				history::Command* c = new history::animations::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes(
                    a, 
                    "start_point",
                    oldvalue, 
                    newvalue,
                    m_panel->UI()->dabSimpleMovementStartingPointX,
                    m_panel->UI()->dabSimpleMovementStartingPointY
                );
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::simpleMovementChangeEndingPointX(double newx)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::SimpleMovement"))
		{
			sad::Point2D newvalue(newx, m_panel->UI()->dabSimpleMovementEndingPointY->value());

			sad::Point2D oldvalue = a->getProperty< sad::Point2D >("end_point").value();
			if (sad::equal(oldvalue, newvalue) == false)
			{
				history::Command* c = new history::animations::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes(
                    a, 
                    "end_point",
                    oldvalue, 
                    newvalue,
                    m_panel->UI()->dabSimpleMovementEndingPointX,
                    m_panel->UI()->dabSimpleMovementEndingPointY
                );
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::simpleMovementChangeEndingPointY(double newy)
{
	sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::SimpleMovement"))
		{
			sad::Point2D newvalue(m_panel->UI()->dabSimpleMovementEndingPointX->value(), newy);

			sad::Point2D oldvalue = a->getProperty< sad::Point2D >("end_point").value();
			if (sad::equal(oldvalue, newvalue) == false)
			{
				history::Command* c = new history::animations::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes(
                    a, 
                    "end_point",
                    oldvalue, 
                    newvalue,
                    m_panel->UI()->dabSimpleMovementEndingPointX,
                    m_panel->UI()->dabSimpleMovementEndingPointY
                );
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::AnimationActions::startPickingStartingPointForSimpleMovement()
{
    sad::hfsm::Machine* m =  this->m_panel->editor()->machine();
    if (this->m_panel->editor()->isInEditingState())
    {
        return;
    }
    sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::SimpleMovement"))
		{
            m_panel->editor()->shared()->setEditingSimpleMovementProperty("start_point");
            m->enterState("picking_simple_movement_point");
            this->m_panel->lockTypesTab(true);
            this->m_panel->highlightState("Please select starting point");
        }
    }
}

void gui::AnimationActions::startPickingEndingPointForSimpleMovement()
{
    sad::hfsm::Machine* m =  this->m_panel->editor()->machine();
    if (this->m_panel->editor()->isInEditingState())
    {
        return;
    }
    sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::SimpleMovement"))
		{
            m_panel->editor()->shared()->setEditingSimpleMovementProperty("end_point");
            m->enterState("picking_simple_movement_point");
            this->m_panel->lockTypesTab(true);
            this->m_panel->highlightState("Please select ending point");
        }
    }
}

void gui::AnimationActions::cancelPickingPointForSimpleMovement()
{
    this->m_panel->editor()->machine()->enterState(this->m_panel->editor()->machine()->previousState());
    this->m_panel->lockTypesTab(false);
}

void gui::AnimationActions::pickedPointForSimpleMovement(const sad::input::MousePressEvent& e)
{
    sad::animations::Animation* a = m_panel->editor()->shared()->selectedAnimation();
	if (a != NULL)
	{
		if (a->isInstanceOf("sad::animations::SimpleMovement"))
		{
			sad::Point2D newvalue(e.pos2D());
            sad::String propertyName = this->m_panel->editor()->shared()->editingSimpleMovementProperty();
            QDoubleSpinBox* xwidget =  m_panel->UI()->dabSimpleMovementEndingPointX;
            QDoubleSpinBox* ywidget =  m_panel->UI()->dabSimpleMovementEndingPointY;
            if (propertyName != "end_point")
            {
                xwidget =  m_panel->UI()->dabSimpleMovementStartingPointX;
                ywidget =  m_panel->UI()->dabSimpleMovementStartingPointY;
            }
			sad::Point2D oldvalue = a->getProperty< sad::Point2D >(propertyName).value();
			if (sad::equal(oldvalue, newvalue) == false)
			{
				history::Command* c = new history::animations::ChangePropertyAsPoint2DDisplayedInTwoSpinboxes(
                    a, 
                    propertyName,
                    oldvalue, 
                    newvalue,
                    xwidget,
                    ywidget
                );
				c->commit(m_panel->editor());
				this->m_panel->editor()->history()->add(c);
			}
		}
	}
    sad::String state = this->m_panel->editor()->machine()->previousState();
    this->m_panel->editor()->machine()->enterState(state);
    this->m_panel->lockTypesTab(false);
}
