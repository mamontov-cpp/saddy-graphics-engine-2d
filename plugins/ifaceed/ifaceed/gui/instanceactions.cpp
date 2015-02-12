#include "instanceactions.h"

#include "../blockedclosuremethodcall.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../history/instances/instancesnew.h"
#include "../history/instances/instancesremove.h"
#include "../history/instances/instanceschangename.h"
#include "../history/instances/instanceschangeanimation.h"
#include "../history/instances/instanceschangeobject.h"
#include "../history/instances/instanceschangestarttime.h"
#include "../history/instances/instanceschangeway.h"

Q_DECLARE_METATYPE(sad::animations::Animation*)
Q_DECLARE_METATYPE(sad::animations::Instance*)
Q_DECLARE_METATYPE(sad::p2d::app::Way*)
Q_DECLARE_METATYPE(sad::db::Object*)

// ===============================  PUBLIC METHODS ===============================

gui::InstanceActions::InstanceActions(QObject* parent) : QObject(parent), m_panel(NULL)
{
	m_animation = new gui::AnimationInstanceProcess();
}

gui::InstanceActions::~InstanceActions()
{
	delete m_animation;	
}

void gui::InstanceActions::setPanel(MainPanel* e)
{
	m_panel = e;
	m_animation->setEditor(e->editor());
}

MainPanel* gui::InstanceActions::panel() const
{
	return m_panel;
}

void gui::InstanceActions::updateGroupInstanceList()
{
	sad::Vector<unsigned long long> ids;
	if (m_panel->editor()->shared()->selectedGroup())
	{
		ids = m_panel->editor()->shared()->selectedGroup()->instances();
	}

	QListWidget* list = m_panel->UI()->lstAnimationInstances;
	QListWidget* destlist = m_panel->UI()->lstAnimationsGroupAllAnimations;
	int allrow = destlist->currentRow();
	destlist->clear();
	for(int i = 0; i < list->count(); i++)
	{
		QString name = list->item(i)->text();
		sad::animations::Instance* instance = list->item(i)->data(Qt::UserRole).value<sad::animations::Instance*>();

		if (std::find(ids.begin(), ids.end(), instance->MajorId) == ids.end())
		{
			QListWidgetItem* item = new QListWidgetItem(name);

			QVariant v;
			v.setValue(instance);

			item->setData(Qt::UserRole, v);
			destlist->addItem(item);
		}
	}
	destlist = m_panel->UI()->lstAnimationsGroupInGroup;
	destlist->clear();
	int grouprow = destlist->currentRow();
	for(size_t  i = 0; i < ids.size(); i++)
	{
		sad::db::Object* o = sad::Renderer::ref()->database("")->queryByMajorId(ids[i]);
		if (o)
		{
			if (o->isInstanceOf("sad::animations::Instance") || o->isInstanceOf("sad::animations::WayInstance"))
			{
				sad::animations::Instance* instance = static_cast<sad::animations::Instance*>(o);

				QString name = m_panel->nameForInstance(instance);

				QVariant v;
				v.setValue(instance);

				QListWidgetItem* item = new QListWidgetItem(name);

				item->setData(Qt::UserRole, v);
				destlist->addItem(item);
			}
		}
	}

	void (QListWidget::*f)(int) = &QListWidget::setCurrentRow;
	destlist = m_panel->UI()->lstAnimationsGroupAllAnimations;
	if (allrow >= 0 && destlist->count() > 0)
	{
		if (allrow >= destlist->count())
		{
			allrow = destlist->count() - 1;
		}
		invoke_blocked(destlist, f, allrow);
	}

	destlist = m_panel->UI()->lstAnimationsGroupInGroup;
	if (grouprow >= 0 && destlist->count() > 0)
	{
		if (grouprow >= destlist->count())
		{
			grouprow = destlist->count() - 1;
		}
		invoke_blocked(destlist, f, grouprow);
	}
}

void gui::InstanceActions::updateCurrentInstanceAnimation(sad::animations::Instance* a)
{
	core::Editor* e =  m_panel->editor();
	// Try set animation from tree
	sad::String animationname = a->getProperty<sad::String>("animation").value();
	if (animationname.length())
	{
		e->emitClosure( blocked_bind(m_panel->UI()->rbAnimationInstanceFromTree, &QRadioButton::setChecked, true));

		QString value = animationname.c_str();
		int pos = -1;
		for (int i = 1; i < m_panel->UI()->cmbAnimationInstanceAnimationFromTree->count() && pos == -1; i++)
		{
			if (value == m_panel->UI()->cmbAnimationInstanceAnimationFromTree->itemText(i))
			{
				pos = i;
			}
		}
		if (pos != -1)
		{
			e->emitClosure( blocked_bind(m_panel->UI()->cmbAnimationInstanceAnimationFromTree, &QComboBox::setCurrentIndex, pos) );
		}
		else
		{
			e->emitClosure( blocked_bind(m_panel->UI()->cmbAnimationInstanceAnimationFromTree, &QComboBox::setCurrentIndex, 0) );
		}
	}
	else
	{
		e->emitClosure( blocked_bind(m_panel->UI()->cmbAnimationInstanceAnimationFromTree, &QComboBox::setCurrentIndex, 0) );
	}

	// Try set animation from database
	unsigned long long animationid =  a->getProperty<unsigned long long>("animationmajorid").value();
	if (animationid > 0)
	{
		e->emitClosure( blocked_bind(m_panel->UI()->rbAnimationInstanceFromDatabase, &QRadioButton::setChecked, true));
		int pos = m_panel->findInComboBoxByMajorId<sad::animations::Animation*>(m_panel->UI()->cmbAnimationInstanceAnimationFromDatabase, animationid);
		if (pos > 0)
		{
			e->emitClosure( blocked_bind(m_panel->UI()->cmbAnimationInstanceAnimationFromDatabase, &QComboBox::setCurrentIndex, pos) );
		}
		else
		{
			e->emitClosure( blocked_bind(m_panel->UI()->cmbAnimationInstanceAnimationFromDatabase, &QComboBox::setCurrentIndex, 0) );
		}
	}
}


void gui::InstanceActions::removeInstanceFromDatabase(
    sad::animations::Instance* a,
    bool fromeditor,
    int row
)
{
    if (row == -1)
    {
        row = m_panel->findInList<sad::animations::Instance*>( m_panel->UI()->lstAnimationInstances, a);
    }

    if (row != -1)
    {
        sad::Vector<sad::db::Object*> objects;
        sad::Vector<sad::Pair<sad::animations::Group*, int> > dependentgroups;

        sad::Renderer::ref()->database("")->table("animationgroups")->objects(objects);
        for(size_t i = 0; i < objects.size(); i++)
        {
            sad::db::Object* tmp = objects[i];
            if (tmp->isInstanceOf("sad::animations::Group"))
            {
                sad::animations::Group* group = static_cast<sad::animations::Group*>(tmp);
                int pos = group->findInstance(a->MajorId);
                if (pos != -1)
                {
                    dependentgroups << sad::Pair<sad::animations::Group*, int>(group, pos);
                }
            }
        }

        history::instances::Remove* c  = new history::instances::Remove(a, row, dependentgroups);
        c->commit(m_panel->editor());

        if (fromeditor)
        {
            m_panel->editor()->history()->add(c);
        }
        else
        {
            m_panel->editor()->currentBatchCommand()->add(c);
        }
    }
}

void gui::InstanceActions::addInstance()
{
	if (m_panel->UI()->rbAnimationInstanceFromDatabase->isChecked()
		|| m_panel->UI()->rbAnimationInstanceFromTree->isChecked())
	{
		sad::animations::Instance* instance = NULL;
		// An default instance
		if (m_panel->UI()->cmbAddedAnimationInstanceType->currentIndex() == 0)
		{
			instance = new sad::animations::Instance();

			// Set animation from tree
			if (m_panel->UI()->rbAnimationInstanceFromTree->isChecked())
			{
				if (m_panel->UI()->cmbAnimationInstanceAnimationFromTree->currentIndex() > 0)
				{
					instance->setProperty("animation", sad::String(m_panel->UI()->cmbAnimationInstanceAnimationFromTree->currentText().toStdString()));
				}
				else
				{
					instance->setProperty("animation", sad::String(""));
				}
			}


			// Set animation from database
			if (m_panel->UI()->rbAnimationInstanceFromDatabase > 0)
			{
				if (m_panel->UI()->cmbAnimationInstanceAnimationFromDatabase->currentIndex() > 0)
				{
					QVariant v = m_panel->UI()->cmbAnimationInstanceAnimationFromDatabase->itemData( m_panel->UI()->cmbAnimationInstanceAnimationFromDatabase->currentIndex(), Qt::UserRole);
					sad::animations::Animation* a = v.value<sad::animations::Animation*>();
					instance->setProperty("animationmajorid", a->MajorId);
				}
				else
				{
					instance->setProperty("animationmajorid", 0ull);
				}
			}

		}

		// A way instance
		if (m_panel->UI()->cmbAddedAnimationInstanceType->currentIndex() == 1)
		{
			instance = new sad::animations::WayInstance();

			// Set way from database
			if (m_panel->UI()->cmbWayAnimationInstanceWay->currentIndex() > 0)
			{
				QVariant v = m_panel->UI()->cmbWayAnimationInstanceWay->itemData(m_panel->UI()->cmbWayAnimationInstanceWay->currentIndex(), Qt::UserRole);
				sad::p2d::app::Way* a = v.value<sad::p2d::app::Way*>();
				instance->setProperty("way", a->MajorId);
			}
			else
			{
				instance->setProperty("way", 0ull);
			}
		}

		// An object
		if (m_panel->UI()->cmbAnimationInstanceObject->currentIndex() > 0)
		{
			QVariant v = m_panel->UI()->cmbAnimationInstanceObject->itemData(m_panel->UI()->cmbAnimationInstanceObject->currentIndex(), Qt::UserRole);
			sad::db::Object* a = v.value<sad::db::Object*>();
			instance->setProperty("object", a->MajorId);
		}
		else
		{
			instance->setProperty("object", 0ull);
		}

		// A start time
		instance->setProperty("starttime", m_panel->UI()->dsbAnimationInstanceStartTime->value());
		instance->setObjectName(m_panel->UI()->txtAnimationInstanceName->text().toStdString());

		sad::Renderer::ref()->database("")->table("animationinstances")->add(instance);

		history::instances::New* c  = new history::instances::New(instance);
		c->commit(m_panel->editor());

		m_panel->UI()->lstAnimationInstances->setCurrentRow(m_panel->UI()->lstAnimationInstances->count() - 1);
		m_panel->editor()->history()->add(c);
	}
}

void gui::InstanceActions::removeInstance()
{
	int row = m_panel->UI()->lstAnimationInstances->currentRow();
	if (row > -1)
	{
		QVariant v = m_panel->UI()->lstAnimationInstances->item(row)->data(Qt::UserRole);
		sad::animations::Instance* a = v.value<sad::animations::Instance*>();
        removeInstanceFromDatabase(a, true, row);
	}
}

void gui::InstanceActions::currentInstanceChanged(int row)
{
	core::Editor* e = m_panel->editor();
	if (row > -1)
	{
		QVariant v = m_panel->UI()->lstAnimationInstances->item(row)->data(Qt::UserRole);
		sad::animations::Instance* a = v.value<sad::animations::Instance*>();
		e->shared()->setSelectedInstance(a);

		if (a->isInstanceOf("sad::animations::Instance"))
		{
			this->updateCurrentInstanceAnimation(a);
		}

		if (a->isInstanceOf("sad::animations::WayInstance"))
		{
			unsigned long long wayid = a->getProperty<unsigned long long>("way").value();
			int pos = m_panel->findInComboBoxByMajorId<sad::p2d::app::Way*>(m_panel->UI()->cmbWayAnimationInstanceWay, wayid);
			if (pos > -1)
			{
				e->emitClosure( blocked_bind(m_panel->UI()->cmbWayAnimationInstanceWay, &QComboBox::setCurrentIndex, pos) );
			}
			else
			{
				e->emitClosure( blocked_bind(m_panel->UI()->cmbWayAnimationInstanceWay, &QComboBox::setCurrentIndex, 0) );
			}
		}

		// Object id
		unsigned long long objectid = a->getProperty<unsigned long long>("object").value();
		int pos = m_panel->findInComboBoxByMajorId<sad::db::Object*>(m_panel->UI()->cmbAnimationInstanceObject, objectid);
		if (pos > -1)
		{
			e->emitClosure( blocked_bind(m_panel->UI()->cmbAnimationInstanceObject, &QComboBox::setCurrentIndex, pos) );
		}
		else
		{
			e->emitClosure( blocked_bind(m_panel->UI()->cmbAnimationInstanceObject, &QComboBox::setCurrentIndex, 0) );
		}

		double starttime =  a->getProperty<double>("starttime").value();
		e->emitClosure( blocked_bind(m_panel->UI()->dsbAnimationInstanceStartTime, &QDoubleSpinBox::setValue, starttime) );

		sad::String name = a->objectName();
		e->emitClosure( blocked_bind(m_panel->UI()->txtAnimationInstanceName, &QLineEdit::setText, QString(name.c_str())) );
	}
	else
	{
		if (e)
		{
			e->shared()->setSelectedInstance(NULL);
		}
	}
}

void gui::InstanceActions::nameChanged(const QString& name)
{
	int row = m_panel->UI()->lstAnimationInstances->currentRow();
	if (row > -1)
	{
		QVariant v = m_panel->UI()->lstAnimationInstances->item(row)->data(Qt::UserRole);
		sad::animations::Instance* a = v.value<sad::animations::Instance*>();

		sad::String oldvalue = a->objectName();
		sad::String newvalue = name.toStdString();
		
		if (oldvalue != newvalue)
		{
			history::instances::ChangeName* c = new history::instances::ChangeName(a, row, oldvalue, newvalue);
			c->commit(m_panel->editor());
			m_panel->editor()->history()->add(c);
		}
	}
}

void gui::InstanceActions::treeLinkStateChanged(bool state)
{
	if (state)
	{
		core::Editor* e = m_panel->editor();
		int row = m_panel->UI()->lstAnimationInstances->currentRow();
		if (row > -1)
		{
			QVariant v = m_panel->UI()->lstAnimationInstances->item(row)->data(Qt::UserRole);
			sad::animations::Instance* a = v.value<sad::animations::Instance*>();

			if (a->isInstanceOf("sad::animations::Instance"))
			{
				unsigned long long oldvalue = a->animationMajorId();
				sad::String oldname  = a->animationName();

				unsigned long long newvalue = 0;
				sad::String newname = m_panel->UI()->cmbAnimationInstanceAnimationFromTree->currentText().toStdString();
				if (m_panel->UI()->cmbAnimationInstanceAnimationFromTree->currentIndex() <= 0)
				{
					newname = "";
				}

				QRadioButton* oldbutton = m_panel->UI()->rbAnimationInstanceFromDatabase;
				QRadioButton* button = m_panel->UI()->rbAnimationInstanceFromTree;

				history::Command* c = new history::instances::ChangeAnimation(
					a,
					button,
					oldbutton,
					oldname,
					oldvalue,
					newname,
					newvalue
				);
				c->commit(e);
				m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::InstanceActions::databaseLinkStateChanged(bool state)
{
	if (state)
	{
		core::Editor* e = m_panel->editor();
		int row = m_panel->UI()->lstAnimationInstances->currentRow();
		if (row > -1)
		{
			QVariant v = m_panel->UI()->lstAnimationInstances->item(row)->data(Qt::UserRole);
			sad::animations::Instance* a = v.value<sad::animations::Instance*>();

			if (a->isInstanceOf("sad::animations::Instance"))
			{
				unsigned long long oldvalue = a->animationMajorId();
				sad::String oldname  = a->animationName();

				unsigned long long newvalue = 0;
				QComboBox* combo = m_panel->UI()->cmbAnimationInstanceAnimationFromDatabase;
				if (combo->currentIndex() > 0)
				{
					int krow = combo->currentIndex();
					sad::animations::Animation* kv = combo->itemData(krow, Qt::UserRole).value<sad::animations::Animation*>();
					newvalue = kv->MajorId;
				}				
				sad::String newname = "";

				QRadioButton* oldbutton = m_panel->UI()->rbAnimationInstanceFromTree;
				QRadioButton* button = m_panel->UI()->rbAnimationInstanceFromDatabase;
				
				history::Command* c = new history::instances::ChangeAnimation(
					a,
					button,
					oldbutton,
					oldname,
					oldvalue,
					newname,
					newvalue
				);
				c->commit(e);
				m_panel->editor()->history()->add(c);
			}
		}
	}
}


void  gui::InstanceActions::treeElementChanged(int newrow)
{
	if (m_panel->UI()->rbAnimationInstanceFromTree->isChecked())
	{
		core::Editor* e = m_panel->editor();
		int row = m_panel->UI()->lstAnimationInstances->currentRow();
		if (row > -1)
		{
			QVariant v = m_panel->UI()->lstAnimationInstances->item(row)->data(Qt::UserRole);
			sad::animations::Instance* a = v.value<sad::animations::Instance*>();

			if (a->isInstanceOf("sad::animations::Instance"))
			{
				sad::String oldname  = a->animationName();
				sad::String newname = "";
				if (newrow > 0)
				{
					newname = m_panel->UI()->cmbAnimationInstanceAnimationFromTree->itemText(newrow).toStdString();
				}

				history::Command* c = new history::instances::ChangeAnimation(
					a,
					oldname,
					newname
				);
				c->commit(e);
				m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::InstanceActions::databaseElementChanged(int newrow)
{
	if (m_panel->UI()->rbAnimationInstanceFromDatabase->isChecked())
	{
		core::Editor* e = m_panel->editor();
		int row = m_panel->UI()->lstAnimationInstances->currentRow();
		if (row > -1)
		{
			QVariant v = m_panel->UI()->lstAnimationInstances->item(row)->data(Qt::UserRole);
			sad::animations::Instance* a = v.value<sad::animations::Instance*>();

			if (a->isInstanceOf("sad::animations::Instance"))
			{
				unsigned long long oldid  = a->animationMajorId();
				unsigned long long newid = 0;
				if (newrow > 0)
				{
					newid = m_panel->UI()->cmbAnimationInstanceAnimationFromDatabase->itemData(newrow, Qt::UserRole).value<sad::animations::Animation*>()->MajorId;
				}

				history::Command* c = new history::instances::ChangeAnimation(
					a,
					oldid,
					newid
				);
				c->commit(e);
				m_panel->editor()->history()->add(c);
			}
		}
	}
}


void  gui::InstanceActions::objectChanged(int newrow)
{
	core::Editor* e = m_panel->editor();
	int row = m_panel->UI()->lstAnimationInstances->currentRow();
	if (row > -1)
	{
		QVariant v = m_panel->UI()->lstAnimationInstances->item(row)->data(Qt::UserRole);
		sad::animations::Instance* a = v.value<sad::animations::Instance*>();

		sad::db::Object* obj = m_panel->UI()->cmbAnimationInstanceObject->itemData(newrow, Qt::UserRole).value<sad::db::Object*>();
		unsigned long long newvalue = 0;
		if (obj)
		{
			newvalue = obj->MajorId;
		}

		unsigned long long oldvalue = a->objectId();
		if (oldvalue != newvalue)
		{
			history::Command* c = new history::instances::ChangeObject(
					a,
					oldvalue,
					newvalue
			);
			c->commit(e);
			m_panel->editor()->history()->add(c);
		}
	}
}

void  gui::InstanceActions::startTimeChanged(double newvalue)
{
	core::Editor* e = m_panel->editor();
	int row = m_panel->UI()->lstAnimationInstances->currentRow();
	if (row > -1)
	{
		QVariant v = m_panel->UI()->lstAnimationInstances->item(row)->data(Qt::UserRole);
		sad::animations::Instance* a = v.value<sad::animations::Instance*>();

		double oldvalue = a->startTime();
		if (sad::is_fuzzy_equal(oldvalue, newvalue) == false)
		{
			history::Command* c = new history::instances::ChangeStartTime(
					a,
					oldvalue,
					newvalue
			);
			c->commit(e);
			m_panel->editor()->history()->add(c);
		}
	}
}

void  gui::InstanceActions::wayChanged(int newrow)
{
	core::Editor* e = m_panel->editor();
	int row = m_panel->UI()->lstAnimationInstances->currentRow();
	if (row > -1)
	{
		QVariant v = m_panel->UI()->lstAnimationInstances->item(row)->data(Qt::UserRole);
		sad::animations::Instance* a = v.value<sad::animations::Instance*>();

		if (a->isInstanceOf("sad::animations::WayInstance"))
		{
			sad::p2d::app::Way* obj = m_panel->UI()->cmbWayAnimationInstanceWay->itemData(newrow, Qt::UserRole).value<sad::p2d::app::Way*>();
			unsigned long long newvalue = 0;
			if (obj)
			{
				newvalue = obj->MajorId;
			}

			unsigned long long oldvalue = static_cast<sad::animations::WayInstance*>(a)->wayMajorId();
			if (oldvalue != newvalue)
			{
				history::Command* c = new history::instances::ChangeWay(
						a,
						oldvalue,
						newvalue
				);
				c->commit(e);
				m_panel->editor()->history()->add(c);
			}
		}
	}
}

void gui::InstanceActions::start()
{
	core::Shared* s = m_panel->editor()->shared();
	if (s->isAnyKindOfAnimationIsRunning() == false 
		&& s->selectedInstance() != NULL)
	{
		s->selectedInstance()->restart(sad::Renderer::ref()->animations());
		m_animation->setEditor(m_panel->editor());
		m_animation->start(s->selectedInstance() );
	}
}

void gui::InstanceActions::stop()
{
	core::Shared* s = m_panel->editor()->shared();
	if (s->isAnyKindOfAnimationIsRunning() == true)
	{
		m_animation->setEditor(m_panel->editor());
		m_animation->stop();
	}
}
