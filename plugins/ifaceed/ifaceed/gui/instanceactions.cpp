#include "instanceactions.h"

#include "../gui/animationprocess.h"

#include "../blockedclosuremethodcall.h"
#include "../closuremethodcall.h"

#include "../mainpanel.h"

#include "../core/editor.h"

#include "../history/instances/instancesnew.h"
#include "../history/instances/instancesremove.h"

Q_DECLARE_METATYPE(sad::animations::Animation*)
Q_DECLARE_METATYPE(sad::animations::Instance*)
Q_DECLARE_METATYPE(sad::p2d::app::Way*)
Q_DECLARE_METATYPE(sad::db::Object*)

// ===============================  PUBLIC METHODS ===============================

gui::InstanceActions::InstanceActions(QObject* parent) : QObject(parent), m_panel(NULL)
{
	
}

gui::InstanceActions::~InstanceActions()
{
	
}

void gui::InstanceActions::setPanel(MainPanel* e)
{
	m_panel = e;
	//m_animation->setEditor(e->editor());
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

		m_panel->editor()->history()->add(c);
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

